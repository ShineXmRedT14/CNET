# Examples

Complete, compilable-shaped examples. Error handling is trimmed to keep the
packet logic visible.

---

## 1. Linux: Ethernet + IPv4 + TCP (Layer 2)

Sends one TCP SYN as a full Ethernet frame. Requires root / `CAP_NET_RAW`.

```c
#include <cnet.h>

int main(void)
{
    struct cnet_route route;
    CNET_MEMSET(&route, sizeof(route));
    if (CNET_CONFIGURE(&route) != CNET_SCFL)
        return 1;

    int fd = CNET_SOCK(&route, CNET_LAYER_ETHER, 0);
    if (fd < 0)
        return 1;

    struct cnet_buffer buf;
    CNET_MEMSET(&buf, sizeof(buf));

    int offset = 0;
    uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);

    struct cnet_ether *eth = CNET_EXTRA_LAYER(struct cnet_ether, pkt, &offset);
    struct cnet_ip    *ip  = CNET_EXTRA_LAYER(struct cnet_ip,    pkt, &offset);
    struct cnet_tcp   *tcp = CNET_EXTRA_LAYER(struct cnet_tcp,   pkt, &offset);

    /* --- Ethernet --- */
    struct cnet_ether_bytes dst_mac, src_mac;
    CNET_MAC_BYTES(&dst_mac, "02:11:22:33:44:55");
    CNET_MAC_BYTES(&src_mac, "02:00:00:00:00:01");
    CNET_SET_DMAC(eth, &dst_mac);
    CNET_SET_SMAC(eth, &src_mac);
    eth->next = CNET_BIG16(CNET_ETH_IP);

    /* --- IPv4 --- */
    struct cnet_ip_bytes src_ip, dst_ip;
    CNET_IP_BYTES(&src_ip, "192.168.1.10");
    CNET_IP_BYTES(&dst_ip, "192.168.1.1");

    ip->version = CNET_IP_VERSION;
    ip->ihl     = IP_IHL_DECODE(sizeof(*ip));
    ip->type    = CNET_IP_TYPE;
    ip->len     = CNET_BIG16(sizeof(*ip) + sizeof(*tcp));
    ip->id      = CNET_BIG16(0x4242);
    ip->frag    = CNET_BIG16(0);
    ip->ttl     = CNET_IP_TTL;
    ip->proto   = CNET_IP_TCP;
    ip->src.CNET_IP_MACROS_SRC32 = src_ip.bytes.b32its;
    ip->dst.CNET_IP_MACROS_DST32 = dst_ip.bytes.b32its;
    ip->sum     = CNET_L3_CSUM(ip, sizeof(*ip));

    /* --- TCP --- */
    tcp->src   = CNET_BIG16(40000);
    tcp->dst   = CNET_BIG16(CNET_PORT_HTTP);
    tcp->seq   = CNET_BIG32(0x1000);
    tcp->aseq  = CNET_BIG32(0);
    tcp->doff  = CNET_TCP_DOFF;
    tcp->flags = CNET_TCP_SYN;
    tcp->win   = CNET_BIG16(CNET_TCP_WIN);
    tcp->uptr  = 0;
    tcp->sum   = CNET_L4_CSUM(CNET_ETH_IP, ip, tcp, sizeof(*tcp));

    CNET_SIZE_SET(&buf, 0, offset);

    struct sockaddr_ll addr;
    CNET_MEMSET(&addr, sizeof(addr));
    addr.sll_family   = AF_PACKET;
    addr.sll_ifindex  = if_nametoindex(route.active);
    addr.sll_halen    = 6;
    memcpy(addr.sll_addr, eth->dst, 6);

    sendto(fd, pkt, offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(fd);
    return 0;
}
```

---

## 2. Linux: raw IPv4 + UDP with a payload (Layer 3)

```c
#include <cnet.h>

int main(void)
{
    struct cnet_route route;
    CNET_MEMSET(&route, sizeof(route));
    CNET_CONFIGURE(&route);

    int fd = CNET_SOCK(&route, CNET_LAYER_IP, CNET_IP_UDP);
    if (fd < 0) return 1;

    struct cnet_buffer buf;
    CNET_MEMSET(&buf, sizeof(buf));

    int offset = 0;
    uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);

    struct cnet_ip  *ip  = CNET_EXTRA_LAYER(struct cnet_ip,  pkt, &offset);
    struct cnet_udp *udp = CNET_EXTRA_LAYER(struct cnet_udp, pkt, &offset);

    const char *msg = "hello";
    size_t msg_len = 5;
    uint8_t *payload = CNET_EXTRA_LAYER(uint8_t, pkt, &offset);
    memcpy(payload, msg, msg_len);

    struct cnet_ip_bytes src, dst;
    CNET_IP_BYTES(&src, "192.168.1.10");
    CNET_IP_BYTES(&dst, "192.168.1.1");

    ip->version = CNET_IP_VERSION;
    ip->ihl     = IP_IHL_DECODE(sizeof(*ip));
    ip->len     = CNET_BIG16(sizeof(*ip) + sizeof(*udp) + msg_len);
    ip->ttl     = CNET_IP_TTL;
    ip->proto   = CNET_IP_UDP;
    ip->src.CNET_IP_MACROS_SRC32 = src.bytes.b32its;
    ip->dst.CNET_IP_MACROS_DST32 = dst.bytes.b32its;
    ip->sum     = CNET_L3_CSUM(ip, sizeof(*ip));

    udp->src = CNET_BIG16(5353);
    udp->dst = CNET_BIG16(CNET_PORT_DNS);
    udp->len = CNET_BIG16(sizeof(*udp) + msg_len);
    udp->sum = CNET_L4_CSUM(CNET_ETH_IP, ip, udp, sizeof(*udp) + msg_len);

    CNET_SIZE_SET(&buf, 0, offset);

    struct sockaddr_in dst_addr = CNET_SOCK_ADDR_IN(ip);
    sendto(fd, pkt, offset, 0,
           (struct sockaddr *)&dst_addr, sizeof(dst_addr));

    close(fd);
    return 0;
}
```

---

## 3. Linux: fast packet formation with `CNET_PACKET_*_FAST_SET()`

`CNET_PACKET_*_FAST_SET()` is the fast path: fill one
`union cnet_packet_fast_config`, then let CNET write the headers, convert byte
order and compute checksums. A complete Linux send:

```c
#include <cnet.h>

int main(void)
{
    struct cnet_route route;
    CNET_MEMSET(&route, sizeof(route));
    if (CNET_CONFIGURE(&route) != CNET_SCFL)
        return 1;

    int fd = CNET_SOCK(&route, CNET_LAYER_ETHER, 0);
    if (fd < 0)
        return 1;

    struct cnet_buffer buf;
    CNET_MEMSET(&buf, sizeof(buf));

    /* --- 1. Describe the packet (host order) --- */
    union cnet_packet_fast_config cfg;
    CNET_MEMSET(&cfg, sizeof(cfg));

    CNET_HELPER_SET_ETH_SRC(&cfg, "02:00:00:00:00:01");
    CNET_HELPER_SET_ETH_DST(&cfg, "02:11:22:33:44:55");
    cfg.eth.next = CNET_ETH_IP;

    CNET_HELPER_SET_IP_SRC(&cfg, "192.168.1.10");
    CNET_HELPER_SET_IP_DST(&cfg, "192.168.1.1");
    cfg.ip.type  = CNET_IP_TYPE;
    cfg.ip.ttl   = CNET_IP_TTL;
    cfg.ip.proto = CNET_IP_TCP;
    cfg.ip.id    = 0x1234;
    cfg.ip.frag  = 0;

    cfg.tcp.src   = 40000;
    cfg.tcp.dst   = CNET_PORT_HTTP;
    cfg.tcp.seq   = 0x1000;
    cfg.tcp.aseq  = 0;
    cfg.tcp.doff  = CNET_TCP_DOFF;
    cfg.tcp.flags = CNET_TCP_SYN;
    cfg.tcp.win   = CNET_TCP_WIN;
    cfg.tcp.uptr  = 0;

    /* --- 2. Reserve the layers in the buffer --- */
    int offset = 0;
    uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);

    struct cnet_ether *eth = CNET_EXTRA_LAYER(struct cnet_ether, pkt, &offset);
    struct cnet_ip    *ip  = CNET_EXTRA_LAYER(struct cnet_ip,    pkt, &offset);
    struct cnet_tcp   *tcp = CNET_EXTRA_LAYER(struct cnet_tcp,   pkt, &offset);

    /* --- 3. Fill everything and compute checksums --- */
    if (CNET_PACKET_ETHER_FAST_SET(&cfg, eth) != CNET_SCFL) return 1;
    if (CNET_PACKET_IP_FAST_SET(&cfg, ip, CNET_IP_NOPTIONS) != CNET_SCFL) return 1;
    if (CNET_PACKET_TCP_FAST_SET(&cfg, tcp, ip, CNET_ETH_IP) != CNET_SCFL) return 1;

    CNET_SIZE_SET(&buf, 0, offset);

    /* --- 4. Send --- */
    struct sockaddr_ll addr;
    CNET_MEMSET(&addr, sizeof(addr));
    addr.sll_family  = AF_PACKET;
    addr.sll_ifindex = if_nametoindex(route.active);
    addr.sll_halen   = 6;
    memcpy(addr.sll_addr, eth->dst, 6);

    sendto(fd, pkt, offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(fd);
    return 0;
}
```

### The same idea for UDP

```c
cfg.ip.proto = CNET_IP_UDP;
cfg.udp.src  = 5353;
cfg.udp.dst  = CNET_PORT_DNS;

struct cnet_ip  *ip  = CNET_EXTRA_LAYER(struct cnet_ip,  pkt, &offset);
struct cnet_udp *udp = CNET_EXTRA_LAYER(struct cnet_udp, pkt, &offset);

CNET_PACKET_IP_FAST_SET(&cfg, ip, CNET_IP_NOPTIONS);
CNET_PACKET_UDP_FAST_SET(&cfg, udp, ip, CNET_ETH_IP);
```

Rules for the fast path:

- Fill `cfg` in **host order**; the `FAST_SET` functions convert.
- `CNET_PACKET_IP_FAST_SET` must run before the L4 filler, because the L4
  checksum reads the IP addresses and protocol.
- Do **not** call `CNET_BIG16()`, `CNET_L3_CSUM()` or `CNET_L4_CSUM()` on
  fields the fast path already filled.
- For IPv6, skip `CNET_PACKET_IP_FAST_SET` and fill `struct cnet_ipv6`
  manually, then call `CNET_PACKET_TCP_FAST_SET` / `_UDP_FAST_SET` with
  `CNET_ETH_IPV6`.
- `CNET_PACKET_IP_FAST_SET(..., CNET_IP_OPTIONS)` reserves a 60-byte IP
  header; place the option bytes after `struct cnet_ip` and advance `offset`.

---

## 4. Windows: send a frame through Npcap

```c
#include <cnet.h>

int main(void)
{
    struct cnet_route route;
    CNET_MEMSET(&route, sizeof(route));

    /* Pick an Npcap device and select it */
    strncpy(route.active, "\\Device\\NPF_{YOUR-GUID}", IFNAMSIZ - 1);

    pcap_t *adapter = CNET_SOCK(&route);
    if (adapter == NULL) return 1;

    if (CNET_CONFIGURE(&route, adapter) != CNET_SCFL) {
        pcap_close(adapter);
        return 1;
    }

    struct cnet_buffer buf;
    CNET_BUFFER_INIT(&buf);

    int offset = 0;
    uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);

    struct cnet_ether *eth = CNET_EXTRA_LAYER(struct cnet_ether, pkt, &offset);
    eth->next = CNET_BIG16(CNET_ETH_ARP);
    /* ... fill the ARP request ... */

    CNET_SIZE_SET(&buf, 0, offset);
    CNET_BUFFER_PACKETS(&buf, 1);
    CNET_BURST(&buf, adapter);

    pcap_close(adapter);
    return 0;
}
```

`CNET_BUFFER_INIT()` allocates the `pcap_send_queue`;
`CNET_BUFFER_PACKETS(&buf, n)` queues slots `0 .. n-1`;
`CNET_BURST()` transmits the queue.

---

## 5. ESP32: broadcast a beacon frame

```cpp
#include <Arduino.h>
extern "C" {
  #include <cnet_init.h>
  #include <cnet_frame/beacon.h>
  #include <cnet_frame/definition.h>
}
#include <cnet.hpp>

void setup() {
    CNET_ESP32_MAX_POWER(6);

    struct cnet_buffer buff;
    CNET_MEMSET(&buff, sizeof(buff));

    int offset = 0;
    uint8_t *pkt = CNET_PACKET_UPTR(&buff);

    struct cnet_frame_beacon *bcn =
        CNET_EXTRA_LAYER(struct cnet_frame_beacon, pkt, &offset);

    static const uint8_t bssid[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01};

    bcn->cnet_hdr.cnet_type.version = CNET_FRAME_VERSION;
    bcn->cnet_hdr.cnet_type.type    = CNET_FRAME_TYPE_MGT;
    bcn->cnet_hdr.cnet_type.stype   = CNET_FRAME_STYPE_BEACON;
    bcn->cnet_hdr.cnet_type.flags   = 0;
    bcn->cnet_hdr.id = 0;
    memcpy(bcn->cnet_hdr.addr1, (const void *)"\xFF\xFF\xFF\xFF\xFF\xFF", 6);
    memcpy(bcn->cnet_hdr.addr2, bssid, 6);
    memcpy(bcn->cnet_hdr.addr3, bssid, 6);
    bcn->cnet_hdr.seq = 0;

    bcn->tstamp = 0;
    bcn->bint   = CNET_BIG16(100);
    bcn->cinfo  = CNET_BIG16(0x0431);

    CNET_SIZE_SET(&buff, offset);
    CNET_BURST(&buff, 0 /* WIFI_IF_STA */, false);
}

void loop() { delay(1000); setup(); }
```

---

## 6. TCP statistics

```c
/* Linux */
struct cnet_tcp_info info;
if (CNET_GET_TCP_INFO(tcp_fd, &info) == CNET_SCFL) {
    printf("state=%u cwnd=%u rtt=%uus unack=%u lost=%u\n",
           info.state, info.cwnd, info.rtt, info.unack, info.lost);
}
```

```cpp
/* ESP32 */
struct cnet_tcp_info info;
if (CNET_GET_TCP_INFO(sock, &info) == CNET_SCFL) {
    printf("cwnd=%u rtt=%uus unack=%u ca=%u\n",
           info.cwnd, info.rtt, info.unack, info.cnet_state_ca);
}
```

---

## 7. Random addresses

```c
struct cnet_ether_bytes mac;
struct cnet_ip_bytes ip;

CNET_RAND_MAC(&mac);           /* locally administered, unicast */
CNET_RAND_IP(&ip);

struct cnet_ether eth = {0};
CNET_SET_SMAC(&eth, &mac);
```

`CNET_RAND(min, max)` is inclusive and thread-local.
