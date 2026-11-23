# API reference

All declarations live in `headers/cnet.h` unless a header is named explicitly.
Platform column: **L** = Linux, **W** = Windows, **E** = ESP32,
**all** = every platform.

---

## 1. Byte order

### `uint16_t CNET_BIG16(uint16_t v)`
### `uint32_t CNET_BIG32(uint32_t v)`
Convert a host-order value to network (big-endian) order. On a big-endian
host they return the value unchanged. Usually wrapped around a literal:

```c
tcp->dst = CNET_BIG16(443);
ip->len  = CNET_BIG16(sizeof(*ip) + sizeof(*tcp));
```

### `uint16_t CNET_LITTLE16(uint16_t v)`
### `uint32_t CNET_LITTLE32(uint32_t v)`
Convert a host-order value to little-endian order. No-op on little-endian
hosts.

### `void CNET_DIRECT_SWAP16(uint8_t *ptr, size_t id)`
Swap the two bytes at `ptr[id]` and `ptr[id+1]` in place. Useful when the
value is already stored in a raw packet buffer.

### `void CNET_DIRECT_SWAP32(uint8_t *ptr, size_t id)`
Swap the four bytes starting at `ptr[id]` in place.

> These operate on raw bytes. `CNET_BIG*` / `CNET_LITTLE*` are the usual
> choice when you have a typed value.

---

## 2. Memory and packet layout

### `void CNET_MEMSET(void *ptr, size_t len)`
Zero `len` bytes at `ptr`. This is CNET's own loop, not `memset()`, and it is
used throughout the library. Note that the second argument is a **byte
count**, not a fill value:

```c
CNET_MEMSET(&hdr, sizeof(hdr));   /* correct */
```

### `void *CNET_CREATE_LAYER(uint8_t *pkt, int *offset, int size)`
Return `pkt + *offset` and advance `*offset` by `size`. Low-level primitive
behind `CNET_EXTRA_LAYER`.

### `CNET_EXTRA_LAYER(structure, pkt, offset)`
GNU statement-expression macro that casts `pkt + *offset` to `structure *` and
advances `offset` by `sizeof(structure)`:

```c
int offset = 0;
uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);

struct cnet_ip  *ip  = CNET_EXTRA_LAYER(struct cnet_ip,  pkt, &offset);
struct cnet_tcp *tcp = CNET_EXTRA_LAYER(struct cnet_tcp, pkt, &offset);
struct cnet_udp *udp = CNET_EXTRA_LAYER(struct cnet_udp, pkt, &offset);
```

If `pkt` or `offset` is `NULL`, it prints an error and returns `NULL`.
Because it is a statement expression it needs GCC/Clang (`-std=gnu*`), which
is what all supported toolchains use.

### `int IP_IHL_CODE(int data)` / `int IP_IHL_DECODE(int data)`
Convert between an IPv4 header length in bytes and the 4-bit IHL field:
`IP_IHL_CODE(5) == 20` (field value to bytes), `IP_IHL_DECODE(20) == 5`
(bytes to field value).

### `void CNET_SET_SMAC(void *eth, void *sbytes)`
### `void CNET_SET_DMAC(void *eth, void *dbytes)`
Copy six bytes from a `struct cnet_ether_bytes` into `cnet_ether.src` /
`cnet_ether.dst`.

### `void CNET_IPV6_VTC(void *ipv6, struct cnet_dipv6 *d)`
Pack `version`, `tc` and `flow` into the leading 32-bit IPv6 word
(`struct cnet_ipv6.vtc`).

### `void CNET_ICMPV6_RSO(void *icmpv6, struct cnet_dicmpv6 *d)`
Pack the Router / Solicited / Override bits into
`struct cnet_icmpv6_na.rso`.

### `void CNET_VLAN_VDP(void *vlan, struct cnet_dvlan *d)`
Pack priority (`p`), drop-eligible (`d`) and VLAN id (`v`) into
`struct cnet_vlan.vdp`.

### `int CNET_RAND(int min, int max)`
Return a pseudo-random integer in `[min, max]`, inclusive. Uses a
thread-local xorshift state seeded from `CLOCK_MONOTONIC`.

### `void CNET_RAND_IP(void *nip)`
Fill a `struct cnet_ip_bytes` with four random bytes.

### `void CNET_RAND_MAC(void *nmac)`
Fill a `struct cnet_ether_bytes` with six random bytes. The first byte is
forced to be a locally administered, unicast address.

---

## 3. Address parsing

### `void CNET_IP_BYTES(void *out, char ip[])`
Parse `"a.b.c.d"` into a `struct cnet_ip_bytes` (`sscanf`-based). The result
is in network order.

### `void CNET_MAC_BYTES(void *out, char mac[])`
Parse `"aa:bb:cc:dd:ee:ff"` into a `struct cnet_ether_bytes`.

```c
struct cnet_ip_bytes ipb;
CNET_IP_BYTES(&ipb, "192.168.0.10");
ip->src.CNET_IP_MACROS_SRC32 = ipb.bytes.b32its;

struct cnet_ether_bytes macb;
CNET_MAC_BYTES(&macb, "02:00:00:00:00:01");
CNET_SET_DMAC(eth, &macb);
```

---

## 4. Checksums and time

### `uint16_t CNET_L3_CSUM(void *data, size_t len)`
Internet checksum over an L3 header (typically the 20-byte IPv4 header).
Returns the value to store in `ip->sum`.

### `uint16_t CNET_L4_CSUM(uint8_t type, void *l3, void *l4, size_t len)`
Pseudo-header checksum for TCP/UDP/ICMP. `type` selects the L3 EtherType:

| `type` | Meaning |
|---|---|
| `CNET_ETH_IP` (0x0800) | IPv4 pseudo-header |
| `CNET_ETH_IPV6` (0x86DD) | IPv6 pseudo-header |
| anything else | returns `0x0000` |

`l3` points to a `struct cnet_ip` or `struct cnet_ipv6`, `l4` to the L4 header.
`len` is the L4 length (header + payload). The pseudo-header protocol field is
read from `l3`.

### `void CNET_SCTP_CSUM(void *sctp, size_t len)`
Compute the SCTP CRC32c and store it in `struct cnet_sctp.sum`.

### `uint32_t CNET_GET_IP_TIMESTAMP(void)`
Milliseconds since midnight UTC (`0 .. 86399999`), used by IPv4 timestamp
options.

### `struct cnet_ntp_dtime CNET_NTP_TIMESTAMP(void)`
Current time as an NTP 64-bit timestamp: `sec` is seconds since 1900 and
`frc` is the fractional second scaled to 2^32.

---

## 5. Whole-header fillers

These fill a complete header from a `union cnet_packet_fast_config`, perform
range checks and endianness conversion, and compute the checksum where
applicable. They return `CNET_SCFL` on success or `CNET_OVERFLOW` when a
field does not fit.

### `cnet_errno_t CNET_PACKET_ETHER_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_ether *eth)`

Reads `pfc->eth.src`, `pfc->eth.dst` (strings) and `pfc->eth.next`.

### `cnet_errno_t CNET_PACKET_IP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_ip *ip, uint8_t options)`

Reads `pfc->ip.{type,id,frag,ttl,proto,src,dst}`. `options` is
`CNET_IP_OPTIONS` or `CNET_IP_NOPTIONS`; it selects a 60-byte or 20-byte IP
header (`ihl` 15 or 5). Sets `len` to that header length plus the L4 header
size, based on `proto` (TCP, UDP, ICMP, IGMP). Returns
`CNET_UNSUPPORT_IP_PROTOCOL` for any other protocol and computes the IP
checksum. When `CNET_IP_OPTIONS` is used, the caller must place the option
bytes after `struct cnet_ip` and account for them in `offset`; the checksum
is still computed over the fixed 20-byte header, so recompute it over the full
60 bytes afterwards.

### `cnet_errno_t CNET_PACKET_TCP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_tcp *tcp, void *ip, uint8_t type)`

Reads `pfc->tcp.{doff,flags,src,dst,seq,aseq,win,uptr}`, sets `res1/res2`
from the `CNET_TCP_RES*` defaults, and computes the checksum using `ip`
(`struct cnet_ip *`) and `type` (`CNET_ETH_IP` or `CNET_ETH_IPV6`).

### `cnet_errno_t CNET_PACKET_UDP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_udp *udp, void *ip, uint8_t type)`

Reads `pfc->udp.{src,dst}`, sets `len` to `sizeof(struct cnet_udp)` and
computes the checksum.

### `void CNET_PACKET_IP_FAST_SET_TIMESTAMP(struct cnet_ip_timestamp *ts, uint8_t custom, const char *ip)`

Append one timestamp entry to an IPv4 timestamp option. `custom` is
`CNET_IP_NCUSTOM` to initialise a fresh option, otherwise pass
`CNET_IP_CUSTOM` to append. `ip` is used when the flag mode stores addresses
(`CNET_IP_TSP_ADDR` / `CNET_IP_TSP_DEFINED`).

### `void CNET_PACKET_TCP_FAST_SET_INFO(struct cnet_tcp_info *info, uint8_t flag)`

Convert all 32-bit counters in a `cnet_tcp_info` to big-endian. Pass
`CNET_TCP_INFO_LOCAL` to skip conversion when the statistics stay local.

### `void CNET_EXPAND(struct cnet_expand_info *info)`

Concatenate `info->main` (length `main_count`) followed by each entry of
`info->options[]` (lengths `options_size[]`) into `info->buffer`. Stops with a
printed error if the buffer would overflow or a required pointer is `NULL`.
Useful for assembling IPv4/TCP option blocks after the fixed header.

---

## 6. Packet buffers and transmission

The buffer type (`struct cnet_buffer`) differs per platform:

| Platform | Layout |
|---|---|
| Linux | `buff[64][2048]`, `size[64]`, an `iovec` and `mmsghdr` per slot |
| Windows | `buff[64][2048]`, `size[64]`, `pos`, `pcap_send_queue *buffer` |
| ESP32 | one `buff[2048]` and one `size` |

### `uint8_t *CNET_PACKET_UPTR(...)`
- **L / W:** `CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index)` — zero and
  return slot `index`.
- **E:** `CNET_PACKET_UPTR(struct cnet_buffer *cbuf)` — zero and return the
  single packet buffer.

### `void CNET_SIZE_SET(...)`
- **L:** `CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, size_t offset)`
- **W:** `CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, int offset)`
- **E:** `CNET_SIZE_SET(struct cnet_buffer *cbuf, size_t len)`

Record the packet length in the slot that was filled.

### `void CNET_BUFFER_INIT_SET(struct cnet_buffer *cbuf, int ind, void *addr, int layer)` — Linux only
Prepare slot `ind` for `sendmmsg`/`sendmsg`: set its `iovec` base/length and
point its `msg_name` at a `sockaddr_ll` when `layer == CNET_LAYER_ETHER`,
otherwise at a `sockaddr_in`.

### `void CNET_BUFFER_INIT_ALL(struct cnet_buffer *cbuf, int ind, void *addr, int layer)` — Linux only
Same as `CNET_BUFFER_INIT_SET`, for slots `0 .. ind-1`.

### `void CNET_BUFFER_CLONE(struct cnet_buffer *cbuf, int dst, int src)` — L / W
Copy the payload and length of slot `src` into slot `dst`.

### `void CNET_BUFFER_INIT(struct cnet_buffer *cbuf)` — Windows only
Zero the buffer and allocate the `pcap_send_queue` used by `CNET_BURST`.

### `void CNET_BUFFER_PACKETS(struct cnet_buffer *cbuf, int ind)` — Windows only
Queue slots `cbuf->pos .. ind-1` into the pcap send queue. At most 64 packets
per call; `pcap_sendqueue_queue` failures are skipped.

### `void CNET_BURST(...)`
- **L:** `CNET_BURST(struct cnet_buffer *cbuf, int sockfd, int hlen)` — one
  `sendmmsg()` with `hlen` messages. Prints an error on failure.
- **W:** `CNET_BURST(struct cnet_buffer *cbuf, pcap_t *socket)` — one
  `pcap_sendqueue_transmit()` of the queued packets.
- **E:** `CNET_BURST(struct cnet_buffer *buff, uint8_t mode, bool switcher)` —
  declared in `cnet.hpp`, calls `esp_wifi_80211_tx(mode, buff->buff, buff->size, switcher)`
  and returns `esp_err_t`.

### `int CNET_SOCK(struct cnet_route *route, int layer, uint8_t flag)` — Linux only

| Layer | Underlying socket |
|---|---|
| `CNET_LAYER_ETHER` | `socket(AF_PACKET, SOCK_RAW, 0)` bound to `route->active`, `PACKET_QDISC_BYPASS`, 4 MiB send buffer |
| `CNET_LAYER_IP` | `socket(AF_INET, SOCK_RAW, flag)` with `IP_HDRINCL`, where `flag` is the protocol (`CNET_IP_TCP`, `CNET_IP_UDP`, …) |

Returns the descriptor, or `-1` on failure (messages are printed to `stderr`).

### `pcap_t *CNET_SOCK(struct cnet_route *route)` — Windows only
`pcap_open_live(route->active, 65535, 1, 1, errbuf)` with reduced copy
latency. Returns `NULL` on failure.

### `cnet_errno_t CNET_CONFIGURE(struct cnet_route *route)` — Linux only
Enumerate interfaces (`getifaddrs`), choose the first non-loopback,
non-tunnel, up interface, then read its IPv4 address (`SIOCGIFADDR`) and MAC
(`SIOCGIFHWADDR`) into `route`. Returns `CNET_SCFL` or one of
`CNET_ERR_IFADDRS`, `CNET_ERR_NIFACE`, `CNET_ERR_IOCTL`, `CNET_ERR_SOCKET`.

### `cnet_errno_t CNET_CONFIGURE(struct cnet_route *route, pcap_t *socket)` — Windows only
Resolve the default gateway (`GetBestRoute`), enumerate Npcap devices, collect
IPv4 addresses and interface names, read the adapter MAC through
`pcap_oid_get_request(OID_802_3_CURRENT_ADDRESS)`, and look up the gateway MAC
in the ARP table (`GetIpNetTable`). Fills `route->iface`, `route->local`,
`route->ip`, `route->smac`, `route->dmac`.

### `struct cnet_ring CNET_RING_INIT(int layer, struct cnet_route *route)` — Linux only
Create an `AF_PACKET` TX ring (`PACKET_TX_RING`) on `route->active` and map
it. `layer` is currently used for the protocol of the ring socket. On failure
`ring.sockfd` is `-1` and `ring.code` holds the error:

`CNET_ERR_SETSOCKOPT_TX`, `CNET_ERR_RING_MAP_FAILED`, `CNET_ERR_IFINDEX`,
`CNET_ERR_BIND`, `CNET_ERR_SETSOCKOPT_QDISC`, `CNET_ERR_SETSOCKOPT_LOSS`.

### `void CNET_RING_BURST(struct cnet_ring *ring, struct cnet_buffer *buff, int len)` — Linux only
Copy `len` packets into the mapped ring, waiting until each frame slot is
`TP_STATUS_AVAILABLE`, then mark it `TP_STATUS_SEND_REQUEST`. Advances
`ring->position` circularly.

### `struct sockaddr_in CNET_SOCK_ADDR_IN(struct cnet_ip *ip)` — Linux only
Build a `sockaddr_in` whose destination is `ip->dst`. Used for Layer 3
`sendto()`.

### `struct sockaddr_ll CNET_SOCK_ADDR_LL(struct cnet_route *route, int sock)` — Linux only
Build a `sockaddr_ll` for the first usable interface. Used for Layer 2
sending.

---

## 7. TCP statistics

### `cnet_errno_t CNET_GET_TCP_INFO(int socket, struct cnet_tcp_info *info)`

Fills a `cnet_tcp_info` from a live TCP socket. Declared in `cnet.h` for
Linux and in `cnet.hpp` for ESP32; there is no Windows implementation.

**Linux** — `getsockopt(socket, IPPROTO_TCP, TCP_INFO, ...)`, mapping
`tcpi_state`, `tcpi_snd_cwnd`, `tcpi_rtt`, `tcpi_rttvar`, `tcpi_snd_mss`,
`tcpi_unacked`, `tcpi_lost`, `tcpi_probes`, `tcpi_backoff`, `tcpi_options`,
`tcpi_snd_wscale`, `tcpi_rcv_wscale`, `tcpi_rto`, `tcpi_ato`.

**ESP32** — reads the lwIP PCB behind the socket with
`lwip_socket_dbg_get_socket()`. `rtt`, `rttv` and `rto` are converted from
lwIP ticks using `TCP_SLOW_INTERVAL`; `unack` is the length of the `pcb->unacked`
segment list; `cnet_state_ca` is derived from `TF_INFR` / `nrtx`.

Returns `CNET_SCFL`, `CNET_ERR_SOCKET` (bad descriptor/pointer/not TCP) or
`CNET_TCP_INFO_BAD_CONNECTION` (ESP32 PCB missing).

---

## 8. Platform helpers

### `void CNET_ESP32_IWIP_INIT(void)` — ESP32

Initialise NVS (erasing on version mismatch), `esp_netif`, the default event
loop and a default STA interface, then start Wi-Fi in `WIFI_MODE_STA` with RAM
storage. Restarts the board if `esp_wifi_start()` fails.

### `void CNET_ESP32_MAX_POWER(uint8_t channel)` — ESP32

Tune the radio for raw transmission: disable brown-out reset, release the
Bluetooth controller memory, call `CNET_ESP32_IWIP_INIT()`, set max TX power,
240 MHz CPU, no power save, 11b/g/n, delete the task watchdog, silence logs,
HT20 bandwidth, promiscuous mode, and the given channel.

### `void cnet_xorrand(uint32_t state, uint32_t *out)`
Seed helper for the xorshift generator behind `CNET_RAND`. `state == 0` is
replaced by 1.

---

## 9. Error codes

`cnet_errno_t` is defined in `cnet_helper.h`:

| Value | Name | Meaning |
|---:|---|---|
| 0 | `CNET_SCFL` | success |
| -1 | `CNET_ERR_IFADDRS` | interface enumeration failed |
| -2 | `CNET_ERR_SOCKET` | socket creation/use failed |
| -3 | `CNET_ERR_IOCTL` | ioctl failed |
| -4 | `CNET_ERR_NIFACE` | no usable interface |
| -5 | `CNET_OVERFLOW` | value does not fit the field |
| -6 | `CNET_UNSUPPORT_IP_PROTOCOL` | protocol not handled by the fast filler |
| -7 | `CNET_TCP_INFO_BAD_CONNECTION` | TCP PCB missing (ESP32) |
| -8 | `CNET_ERR_SETSOCKOPT_TX` | `PACKET_TX_RING` failed |
| -9 | `CNET_ERR_RING_MAP_FAILED` | `mmap()` of the TX ring failed |
| -10 | `CNET_ERR_IFINDEX` | `SIOCGIFINDEX` failed |
| -11 | `CNET_ERR_BIND` | ring bind failed |
| -12 | `CNET_ERR_SETSOCKOPT_QDISC` | `PACKET_QDISC_BYPASS` failed |
| -13 | `CNET_ERR_SETSOCKOPT_LOSS` | `PACKET_LOSS` failed |
