# Protocol reference

This page explains how each protocol header is meant to be used. Field tables
are in [structures.md](structures.md); constants are in
[defines.md](defines.md). Everything here applies to all platforms unless
noted.

## How a packet is assembled

CNET packets are built as a contiguous byte buffer. Layers are appended with
`CNET_EXTRA_LAYER`, which simply returns a typed pointer and advances an
offset:

```c
int offset = 0;
uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);

struct cnet_ether *eth = CNET_EXTRA_LAYER(struct cnet_ether, pkt, &offset);
struct cnet_ip    *ip  = CNET_EXTRA_LAYER(struct cnet_ip,    pkt, &offset);
struct cnet_tcp   *tcp = CNET_EXTRA_LAYER(struct cnet_tcp,   pkt, &offset);
uint8_t           *pay = CNET_EXTRA_LAYER(uint8_t,           pkt, &offset);
```

Rules that apply to every layer:

1. Zero or fill the header before computing checksums.
2. Write multi-byte integers in network order: `CNET_BIG16` / `CNET_BIG32`.
3. Set the **upper** layer's length field before its checksum.
4. `offset` after the last layer is the packet length.

---

## Ethernet II (`cnet_ether.h`)

```
 0               6               12      14
 +---------------+--------------- +------+----------------...
 | dst MAC (6)   | src MAC (6)   | type | payload
 +---------------+----------------+------+----------------...
```

```c
struct cnet_ether *eth = CNET_EXTRA_LAYER(struct cnet_ether, pkt, &offset);
struct cnet_ether_bytes dst;
CNET_MAC_BYTES(&dst, "ff:ff:ff:ff:ff:ff");
CNET_SET_DMAC(eth, &dst);
CNET_SET_SMAC(eth, &route_mac_bytes);
eth->next = CNET_BIG16(CNET_ETH_IP);
```

Use `CNET_ETH_IP`, `CNET_ETH_ARP`, `CNET_ETH_RARP`, `CNET_ETH_IPV6`,
`CNET_ETH_VLAN`.

## VLAN tag (`cnet_vlan.h`)

An 802.1Q tag sits between the Ethernet type and the encapsulated type
(`struct cnet_ether.next == CNET_ETH_VLAN`):

```c
struct cnet_vlan *vlan = CNET_EXTRA_LAYER(struct cnet_vlan, pkt, &offset);
struct cnet_dvlan d = { .v = 100, .d = 0, .p = CNET_VLAN_PCP_BE };
vlan->id = CNET_BIG16(CNET_VLAN_ID_Q);
CNET_VLAN_VDP(vlan, &d);
vlan->ether = CNET_BIG16(CNET_ETH_IP);
```

`vlan->id` is the TPID (`CNET_VLAN_ID_Q` = 0x8100, `CNET_VLAN_ID_AD` =
0x88A8); `vdp` holds priority + DEI + VLAN id.

## ARP (`cnet_arp.h`)

```c
struct cnet_arp *arp = CNET_EXTRA_LAYER(struct cnet_arp, pkt, &offset);
arp->hwtype = CNET_BIG16(1);        /* Ethernet */
arp->ptype  = CNET_BIG16(CNET_ETH_IP);
arp->hwlen  = 6;
arp->plen   = 4;
arp->code   = CNET_BIG16(CNET_ARP_REQ);
memcpy(arp->hwsrc, local_mac, 6);
memcpy(arp->psrc,  local_ip, 4);
memset(arp->hwdst, 0, 6);
memcpy(arp->pdst,  target_ip, 4);
```

Send an ARP request as an Ethernet broadcast (`ff:ff:ff:ff:ff:ff`,
`next = CNET_ETH_ARP`).

---

## IPv4 (`cnet_ip.h`)

```c
struct cnet_ip *ip = CNET_EXTRA_LAYER(struct cnet_ip, pkt, &offset);
ip->version = CNET_IP_VERSION;           /* 4 */
ip->ihl     = IP_IHL_DECODE(sizeof(*ip));
ip->type    = CNET_IP_TYPE;              /* 0 */
ip->len     = CNET_BIG16(sizeof(*ip) + l4_len);
ip->id      = CNET_BIG16(id);
ip->frag    = CNET_BIG16(0);
ip->ttl     = CNET_IP_TTL;
ip->proto   = CNET_IP_TCP;               /* or UDP, ICMP, IGMP */
ip->src.CNET_IP_MACROS_SRC32 = src_bytes.bytes.b32its;
ip->dst.CNET_IP_MACROS_DST32 = dst_bytes.bytes.b32its;
ip->sum     = CNET_L3_CSUM(ip, sizeof(*ip));
```

`type` here is the DS field (DSCP + ECN), despite the name.

### Options
`struct cnet_ip_timestamp` describes the IPv4 timestamp option. Create it
after the fixed header, then append entries with
`CNET_PACKET_IP_FAST_SET_TIMESTAMP()`:

```c
uint8_t custom = CNET_IP_NCUSTOM;
struct cnet_ip_timestamp *ts = CNET_EXTRA_LAYER(struct cnet_ip_timestamp, pkt, &offset);
CNET_PACKET_IP_FAST_SET_TIMESTAMP(ts, custom, "10.0.0.2");
CNET_PACKET_IP_FAST_SET_TIMESTAMP(ts, CNET_IP_CUSTOM, "10.0.0.2");
```

Timestamp flag modes: `CNET_IP_TSP_ONLY`, `CNET_IP_TSP_ADDR`,
`CNET_IP_TSP_DEFINED`.

---

## IPv6 (`cnet_ipv6.h`)

```c
struct cnet_ipv6 *ip6 = CNET_EXTRA_LAYER(struct cnet_ipv6, pkt, &offset);
struct cnet_dipv6 d = { .version = 6, .tc = 0, .flow = 0 };
CNET_IPV6_VTC(ip6, &d);
ip6->plen  = CNET_BIG16(payload_len);
ip6->proto = CNET_IPV6_TCP;
ip6->ttl   = 64;
memcpy(ip6->src, src16, 16);
memcpy(ip6->dst, dst16, 16);
```

`plen` does **not** include the 40-byte fixed header. L4 checksums use
`CNET_L4_CSUM(CNET_ETH_IPV6, ip6, l4, l4_len)`.

Extension headers are described by `cnet_ipv6_ext`, `cnet_ipv6_hbh`,
`cnet_ipv6_dst`, `cnet_ipv6_route`, `cnet_ipv6_route0`, `cnet_ipv6_frag`,
and the option structs (`cnet_ipv6_opt*`).

---

## ICMP (`cnet_icmp.h`)

```c
struct cnet_icmp *icmp = CNET_EXTRA_LAYER(struct cnet_icmp, pkt, &offset);
icmp->type = CNET_ICMP_ECHO;
icmp->code = 0;
icmp->sum  = 0;
icmp->CNET_ICMP_MACROS_ECHO.id  = CNET_BIG16(0x1234);
icmp->CNET_ICMP_MACROS_ECHO.seq = CNET_BIG16(1);
icmp->sum = CNET_L4_CSUM(CNET_ETH_IP, ip, icmp, sizeof(*icmp));
```

The union `icmp` provides `raw`, `echo`, `gate` (redirect gateway) and `mtu`
(fragmentation needed). For ICMP the L4 checksum is computed over the ICMP
message only (the pseudo-header protocol field is 1).

## ICMPv6 (`cnet_icmpv6.h`)

Same pattern, with `CNET_ETH_IPV6` passed to `CNET_L4_CSUM`. Neighbor
advertisements use:

```c
struct cnet_dicmpv6 f = { .r = 0, .s = 1, .o = 1 };
CNET_ICMPV6_RSO(icmp6, &f);
```

Types: `CNET_ICMPV6_ECHO_REQ`, `_ECHO_REP`, `_RA`, `_NA`; MTU option type
`CNET_ICMPV6_MTU`.

## IGMP (`cnet_igmp.h`)

```c
struct cnet_igmp *igmp = CNET_EXTRA_LAYER(struct cnet_igmp, pkt, &offset);
igmp->type  = 0x16;              /* membership report v2 */
igmp->code  = 0;
igmp->group = group_ip_network_order;
igmp->sum   = CNET_L3_CSUM(igmp, sizeof(*igmp));
```

Use `ip->proto = CNET_IP_IGMP`, TTL 1, destination equal to the group address.

---

## TCP (`cnet_tcp.h`)

```c
struct cnet_tcp *tcp = CNET_EXTRA_LAYER(struct cnet_tcp, pkt, &offset);
tcp->src   = CNET_BIG16(40000);
tcp->dst   = CNET_BIG16(CNET_PORT_HTTP);
tcp->seq   = CNET_BIG32(seq);
tcp->aseq  = CNET_BIG32(ack);
tcp->doff  = CNET_TCP_DOFF;         /* 5 words = 20 bytes */
tcp->flags = CNET_TCP_SYN | CNET_TCP_ACK;
tcp->win   = CNET_BIG16(CNET_TCP_WIN);
tcp->uptr  = 0;
tcp->sum   = CNET_L4_CSUM(CNET_ETH_IP, ip, tcp, sizeof(*tcp));
```

`flags` is a 6-bit field in the struct; the `CNET_TCP_*` constants are
bit values and can be OR-ed.

TCP options are appended after the 20-byte header; increase `doff`
accordingly and include the option bytes in the checksum length. Use
`CNET_L4_EXPAND()` to concatenate a fixed part plus option blocks into a
scratch buffer.

## UDP (`cnet_udp.h`)

```c
struct cnet_udp *udp = CNET_EXTRA_LAYER(struct cnet_udp, pkt, &offset);
udp->src = CNET_BIG16(5353);
udp->dst = CNET_BIG16(CNET_PORT_DNS);
udp->len = CNET_BIG16(sizeof(*udp) + payload_len);
udp->sum = 0;
udp->sum = CNET_L4_CSUM(CNET_ETH_IP, ip, udp, sizeof(*udp) + payload_len);
```

The checksum covers the UDP header plus payload. A checksum of `0x0000` means
"no checksum" in IPv4; in IPv6 it is mandatory.

## SCTP (`cnet_sctp.h`)

SCTP uses a common header (`struct cnet_sctp`) plus one or more chunks
(`struct cnet_sctp_chunk` and friends). The checksum is CRC32c over the whole
SCTP packet with the checksum field zeroed:

```c
CNET_SCTP_CSUM(sctp, sctp_len);
```

Chunk types, DATA flags, parameter ids and error causes are enumerated in
[defines.md](defines.md#sctp).

---

## DHCP (`cnet_dhcp.h`)

`struct cnet_dhcp` is the fixed BOOTP/DHCP header. Options follow it as
`code, len, value` triples (`struct cnet_dhcp_option`), terminated by
`CNET_DHCP_OP_END`. The magic cookie must be set:

```c
dhcp->cookie = CNET_BIG32(CNET_DHCP_MAGIC_COOKIE);
```

Common option codes: `_MTYPE` (message type), `_REQIP`, `_MASK`, `_ROUTER`,
`_DNS_SERV`, `_LTIME`, `_SERV_ID`, `_END`. Message types: `_DISCOVER`,
`_OFFER`, `_REQUEST`, `_DECLINE`, `_ACK`, `_NAK`, `_RELEASE`, `_INFORM`.

The address unions expose `CNET_DHCP_MACROS_CADDR8/32` etc.

## DNS (`cnet_dns.h`)

Header `struct cnet_dns` (id, flags, counts) + question(s)
(`struct cnet_dns_question`) + the encoded name + record(s)
(`struct cnet_dns_record`). Names use DNS label encoding, not a NUL string.
`flags` is assembled from the `CNET_DNS_FLAG_*` masks; record types and
response codes are listed in [defines.md](defines.md#dns).

## NTP (`cnet_ntp.h`)

`struct cnet_ntp` is the 48-byte NTP packet. Fill `li`, `vn`, `md`, then the
timestamps:

```c
struct cnet_ntp *ntp = CNET_EXTRA_LAYER(struct cnet_ntp, pkt, &offset);
ntp->li = CNET_NTP_LI_NONE;
ntp->vn = CNET_NTP_VERSION;
ntp->md = CNET_NTP_MD_CLIENT;

struct cnet_ntp_dtime now = CNET_NTP_TIMESTAMP();
ntp->trsm_tm.sec = CNET_BIG32(now.sec);
ntp->trsm_tm.frc = CNET_BIG32(now.frc);
```

Send to `CNET_PORT_NTP` over UDP. `CNET_NTP_TIMESTAMP()` returns the current
time in NTP epoch units.

> The fixed header declares `delay`/`disper` as `struct cnet_ntp_short` but
> the header defines `struct cnet_short`; see [caveats.md](caveats.md).

## TLS (`cnet_tls.h`)

Only the record and handshake prefixes are modelled:

```c
struct cnet_tls_record *rec = CNET_EXTRA_LAYER(struct cnet_tls_record, pkt, &offset);
rec->type    = CNET_TLS_TYPE_HANDSHAKE;
rec->version = CNET_BIG16(CNET_TLS_VERSION_1_2);
rec->len     = CNET_BIG16(handshake_len);
```

TLS bodies (cipher suites, extensions, keys) are not modelled; build them
manually after the record header.

---

## 802.11 frames (`cnet_frame/`)

ESP32 transmits raw 802.11 frames through
`CNET_BURST(struct cnet_buffer *, wifi_interface_t, bool)` (see `cnet.hpp`),
which calls `esp_wifi_80211_tx()`. Each frame struct bundles the MAC header,
the frame-specific fields and a 4-byte `fcs[4]`.

### Management frame header

```c
struct cnet_frame_hdr *hdr = &frame->cnet_hdr;
hdr->cnet_type.version = CNET_FRAME_VERSION;
hdr->cnet_type.type    = CNET_FRAME_TYPE_MGT;
hdr->cnet_type.stype   = CNET_FRAME_STYPE_BEACON;
hdr->cnet_type.flags   = 0;
hdr->id = 0;
memcpy(hdr->addr1, dst, 6);
memcpy(hdr->addr2, src, 6);
memcpy(hdr->addr3, bssid, 6);
hdr->seq = 0;    /* sequence/fragment control */
```

### Beacon

```c
struct cnet_frame_beacon *bcn =
    CNET_EXTRA_LAYER(struct cnet_frame_beacon, pkt, &offset);
/* fill cnet_hdr as above with stype BEACON */
bcn->tstamp = 0;
bcn->bint   = CNET_BIG16(100);      /* beacon interval, TU */
bcn->cinfo  = CNET_BIG16(0x0431);   /* capability info */
```

### Probe request

```c
struct cnet_frame_probe_req *pr =
    CNET_EXTRA_LAYER(struct cnet_frame_probe_req, pkt, &offset);
pr->cnet_probe_body.eid = CNET_FRAME_PROBE_BODY_EID;
pr->cnet_probe_body.len = CNET_FRAME_PROBE_BODY_LEN;
pr->cnet_probe_rate.eid = CNET_FRAME_PROBE_RATES_EID;
pr->cnet_probe_rate.len = CNET_FRAME_PROBE_RATES_LEN;
pr->cnet_probe_rate.rates[0] = CNET_FRAME_PROBE_RATES_MB1;
/* ... */
```

### FCS

On real hardware the FCS is normally appended by the radio. If you need to
compute it yourself for a captured/built frame:

```c
struct cnet_frame_fcs *fcs = (struct cnet_frame_fcs *)frame->fcs;
CNET_FRAME_FCS(fcs, frame, sizeof(*frame) - 4);
```

`CNET_FRAME_FCS()` is the CRC32 used by 802.11 and writes
`fcs->CNET_FRAME_MACROS_FCS32`.

### Frame type map

| File | Structs |
|---|---|
| `assoc.h` | `cnet_frame_assoc_req/resp` |
| `reassoc.h` | `cnet_frame_reassoc_req/resp` |
| `probe.h` | `cnet_frame_probe_req/resp`, body, rates |
| `beacon.h` | `cnet_frame_beacon` |
| `atim.h` | `cnet_frame_atim` |
| `diassoc.h` | `cnet_frame_diassoc` |
| `auth.h` | `cnet_frame_auth` |
| `deauth.h` | `cnet_frame_deauth` |
| `action.h` | `cnet_frame_action`, `_na` |
| `adv.h` | `cnet_frame_adv` |
| `psp.h` | `cnet_frame_psp` |
| `rts.h` | `cnet_frame_rts` |
| `cts.h` | `cnet_frame_cts` |
| `ack.h` | `cnet_frame_ack` |
| `cf.h` | `cnet_frame_cf`, `_ack` |
| `ctlw.h` | `cnet_frame_ctlw` |
| `trigger.h` | `cnet_frame_trigger` |
| `block.h` | `cnet_frame_block_ackr`, `_ack` |
| `data.h` | `cnet_frame_data`, `_null`, `_wds`, `_qos_*` |
| `hdr.h` | `cnet_frame_type`, `cnet_frame_hdr` |
| `definition.h` | status/auth/reason/action constants |
