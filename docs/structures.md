# Structures and enums

Every packet structure is declared `__attribute__((packed))`, so its in-memory
layout is the on-wire layout. Multi-byte fields are network order on the wire.

---

## Core helper types (`cnet_helper.h`)

### `cnet_errno_t`
Enum of return codes. See the table in [api.md](api.md#9-error-codes).

### `struct cnet_dipv6`
| Field | Type | Meaning |
|---|---|---|
| `version` | `int` | IPv6 version nibble (6) |
| `tc` | `int` | traffic class (8 bits) |
| `flow` | `uint32_t` | flow label (20 bits) |

Used by `CNET_IPV6_VTC()`.

### `struct cnet_dicmpv6`
| Field | Type | Meaning |
|---|---|---|
| `r` | `int` | router flag |
| `s` | `int` | solicited flag |
| `o` | `int` | override flag |

### `struct cnet_dvlan`
| Field | Type | Meaning |
|---|---|---|
| `v` | `int` | VLAN id (12 bits) |
| `d` | `int` | drop-eligible bit |
| `p` | `int` | priority code point (3 bits) |

### `struct cnet_ntp_dtime`
| Field | Type | Meaning |
|---|---|---|
| `sec` | `uint32_t` | seconds since 1900 |
| `frc` | `uint32_t` | fractional second, 2^-32 units |

### `struct cnet_l4_expand_info`
| Field | Type | Meaning |
|---|---|---|
| `buffer` | `uint8_t *` | destination |
| `buffer_size` | `size_t` | capacity of `buffer` |
| `main` | `void *` | fixed part |
| `main_count` | `size_t` | bytes in `main` |
| `options[64]` | `void *` | option blocks |
| `options_size[64]` | `size_t` | length of each option block |
| `count` | `size_t` | number of option blocks |

### `struct cnet_frame_fcs`
Union `fcs` with `fcs32`, `fcs16[2]`, `fcs8[4]`. A 32-bit 802.11 FCS holder.

Aliases: `CNET_FRAME_MACROS_FCS32`, `CNET_FRAME_MACROS_FCS16`,
`CNET_FRAME_MACROS_FCS8`.

### `union cnet_packet_fast_config`

| Member | Fields |
|---|---|
| `tcp` | `int doff; uint16_t flags; int src; int dst; uint32_t seq; uint32_t aseq; int win; int uptr;` |
| `udp` | `int src; int dst; int len;` |
| `ip` | `int type; int id; int frag; int ttl; int proto; char src[16]; char dst[16];` |
| `eth` | `char src[18]; char dst[18]; int next;` |

The integer fields are host order; the `FAST_SET` functions convert. `src` /
`dst` in `ip` and `eth` are text addresses filled with
`CNET_HELPER_SET_IP_SRC()` etc.

---

## Buffers (`cnet_buffer.h`)

### `struct cnet_ring` — Linux only
| Field | Type | Meaning |
|---|---|---|
| `sockfd` | `int` | ring socket |
| `ring_ptr` | `uint8_t *` | `mmap`ed ring |
| `ring_size` | `size_t` | total bytes |
| `frame_size` | `uint32_t` | size of one frame slot |
| `frame_count` | `uint32_t` | number of slots |
| `position` | `uint32_t` | current slot |
| `code` | `cnet_errno_t` | init result |

### `struct cnet_buffer`
| Platform | Fields |
|---|---|
| Linux | `uint8_t buff[64][2048]; size_t size[64]; struct iovec iov[64]; struct mmsghdr msg[64];` |
| Windows | `uint8_t buff[64][2048]; size_t size[64]; int pos; pcap_send_queue *buffer;` |
| ESP32 | `uint8_t buff[2048]; size_t size;` |

`APY == 64` slots, `APX == 2048` bytes per slot.

---

## Ethernet and VLAN

### `struct cnet_ether_bytes`
`uint8_t bytes[6]`.

### `struct cnet_ether`
| Field | Type | Meaning |
|---|---|---|
| `dst` | `uint8_t[6]` | destination MAC |
| `src` | `uint8_t[6]` | source MAC |
| `next` | `uint16_t` | EtherType (`CNET_ETH_*`) |

### `struct cnet_vlan`
| Field | Type | Meaning |
|---|---|---|
| `id` | `uint16_t` | TPID (0x8100 / 0x88A8) |
| `vdp` | `uint16_t` | PCP + DEI + VLAN id, built by `CNET_VLAN_VDP()` |
| `ether` | `uint16_t` | encapsulated EtherType |

---

## ARP (`cnet_arp.h`)

### `struct cnet_arp`
| Field | Type |
|---|---|
| `hwtype` | `uint16_t` |
| `ptype` | `uint16_t` |
| `hwlen` | `uint8_t` |
| `plen` | `uint8_t` |
| `code` | `uint16_t` |
| `hwsrc` | `uint8_t[6]` |
| `psrc` | `uint8_t[4]` |
| `hwdst` | `uint8_t[6]` |
| `pdst` | `uint8_t[4]` |

---

## IPv4 (`cnet_ip.h`)

### `struct cnet_ip_bytes`
Union `bytes`: `uint8_t b8its[4]` / `uint32_t b32its`.
Aliases: `CNET_IP_MACROS_BYTES8`, `CNET_IP_MACROS_BYTES32`.

### `struct cnet_ip_timestamp`
| Field | Type | Meaning |
|---|---|---|
| `len` | `uint8_t` | option length |
| `ptr` | `uint8_t` | next free slot |
| `flags` | 4-bit | timestamp flag |
| `flow` | 4-bit | overflow/use counter |
| `data[9]` | `uint32_t` | timestamp / address entries |

### `struct cnet_ip`
| Field | Type | Meaning |
|---|---|---|
| `ihl` | 4-bit | header length in 32-bit words |
| `version` | 4-bit | 4 |
| `type` | `uint8_t` | DSCP/ECN |
| `len` | `uint16_t` | total length |
| `id` | `uint16_t` | identification |
| `frag` | `uint16_t` | flags + fragment offset |
| `ttl` | `uint8_t` | time to live |
| `proto` | `uint8_t` | L4 protocol |
| `sum` | `uint16_t` | header checksum |
| `src` | union | `s8rc[4]` / `s32rc` |
| `dst` | union | `d8st[4]` / `d32st` |

Aliases: `CNET_IP_MACROS_SRC8`, `CNET_IP_MACROS_SRC32`,
`CNET_IP_MACROS_DST8`, `CNET_IP_MACROS_DST32`.

> The bitfield order of `ihl`/`version` is reversed on big-endian builds by
> the header.

---

## IPv6 (`cnet_ipv6.h`)

### `struct cnet_ipv6`
| Field | Type | Meaning |
|---|---|---|
| `vtc` | `uint32_t` | version + traffic class + flow label |
| `plen` | `uint16_t` | payload length |
| `proto` | `uint8_t` | next header |
| `ttl` | `uint8_t` | hop limit |
| `src[16]` | `uint8_t` | source |
| `dst[16]` | `uint8_t` | destination |

### Extension headers
| Struct | Fields |
|---|---|
| `cnet_ipv6_ext` | `proto`, `len` |
| `cnet_ipv6_hbh` | `proto`, `len` |
| `cnet_ipv6_dst` | `proto`, `len` |
| `cnet_ipv6_route` | `proto`, `len`, `type`, `sleft` |
| `cnet_ipv6_route0` | `proto`, `len`, `type`, `sleft`, `res`, `smap[3]` |
| `cnet_ipv6_frag` | `proto`, `res`, `off`, `id` |
| `cnet_ipv6_opt` | `type`, `len` |
| `cnet_ipv6_opt_jumbo` | `type`, `len`, `jumbo[4]` |
| `cnet_ipv6_opt_nsap` | `type`, `len`, `lsrc`, `ldst` |
| `cnet_ipv6_opt_tunnel` | `type`, `len`, `limit` |
| `cnet_ipv6_opt_router` | `type`, `len`, `value[2]` |

---

## ICMP (`cnet_icmp.h`)

### `struct cnet_icmp_echo`
`uint16_t id; uint16_t seq;`

### `struct cnet_icmp_mtu`
`uint16_t unused; uint16_t mtu;`

### `struct cnet_icmp_gate`
`uint32_t gate;`

### `struct cnet_icmp`
| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | message type |
| `code` | `uint8_t` | message code |
| `sum` | `uint16_t` | checksum |
| `icmp` | union | `raw` (`raw32`/`raw16[2]`/`raw8[4]`), `echo`, `gate`, `mtu` |

Aliases: `CNET_ICMP_MACROS_RAW`, `_ECHO`, `_GATE`, `_MTU`.

## ICMPv6 (`cnet_icmpv6.h`)

### `struct cnet_icmpv6`
Same shape as `cnet_icmp`: `type`, `code`, `sum`, union `icmp` with `raw`,
`echo`, `ra`, `na`, `mtu`.

### Payload structs
| Struct | Fields |
|---|---|
| `cnet_icmpv6_echo` | `id`, `seq` |
| `cnet_icmpv6_ra` | `ttl`, `flags`, `ltime` |
| `cnet_icmpv6_na` | `rso` (router/solicited/override bits) |
| `cnet_icmpv6_mtu` | `mtu` |

Aliases: `CNET_ICMPV6_MACROS_RAW`, `_ECHO`, `_RA`, `_NA`, `_MTU`.

## IGMP (`cnet_igmp.h`)

### `struct cnet_igmp`
`uint8_t type; uint8_t code; uint16_t sum; uint32_t group;`

---

## TCP (`cnet_tcp.h`)

### `enum cnet_state_ca`
| Value | Name |
|---:|---|
| 0 | `CNET_TCP_OPEN` |
| 1 | `CNET_TCP_DISC` |
| 2 | `CNET_TCP_CWR` |
| 3 | `CNET_TCP_RECOV` |
| 4 | `CNET_TCP_LOSS` |

### `struct cnet_tcp_info`
| Field | Type | Meaning |
|---|---|---|
| `state` | `uint8_t` | TCP state |
| `cnet_state_ca` | `uint8_t` | congestion state (`enum cnet_state_ca`) |
| `probes` | `uint8_t` | zero-window probes |
| `off` | `uint8_t` | backoff / retransmit counter |
| `options` | `uint8_t` | negotiated options bitmap |
| `sscale` | 4-bit | send window scale |
| `rscale` | 4-bit | receive window scale |
| `rto` | `uint32_t` | retransmission timeout (µs) |
| `ato` | `uint32_t` | delayed-ACK timeout (µs) |
| `smss` | `uint32_t` | send MSS |
| `rmss` | `uint32_t` | receive MSS |
| `unack` | `uint32_t` | unacknowledged segments |
| `sack` | `uint32_t` | SACK blocks |
| `lost` | `uint32_t` | lost segments |
| `ret` | `uint32_t` | retransmissions |
| `ackets` | `uint32_t` | ACKed segments |
| `lds` | `uint32_t` | lost during slow start |
| `las` | `uint32_t` | lost after slow start |
| `ldr` | `uint32_t` | lost during recovery |
| `lar` | `uint32_t` | lost after recovery |
| `pmtu` | `uint32_t` | path MTU |
| `rth` | `uint32_t` | RTT histogram bucket |
| `rtt` | `uint32_t` | smoothed RTT (µs) |
| `rttv` | `uint32_t` | RTT variance (µs) |
| `sth` | `uint32_t` | slow-start threshold |
| `cwnd` | `uint32_t` | congestion window |
| `advmss` | `uint32_t` | advertised MSS |
| `reorder` | `uint32_t` | reordering metric |
| `rrtt` | `uint32_t` | receiver RTT |
| `rspace` | `uint32_t` | receive space |
| `tret` | `uint32_t` | total retransmissions |

Not every field is filled on every platform; see
[`CNET_GET_TCP_INFO`](api.md#7-tcp-statistics).

### `struct cnet_tcp`
| Field | Type | Meaning |
|---|---|---|
| `src` | `uint16_t` | source port |
| `dst` | `uint16_t` | destination port |
| `seq` | `uint32_t` | sequence number |
| `aseq` | `uint32_t` | acknowledgment number |
| `res1` | 4-bit | reserved |
| `doff` | 4-bit | data offset (32-bit words) |
| `flags` | 6-bit | control bits |
| `res2` | 2-bit | reserved |
| `win` | `uint16_t` | window |
| `sum` | `uint16_t` | checksum |
| `uptr` | `uint16_t` | urgent pointer |

## UDP (`cnet_udp.h`)

### `struct cnet_udp`
`uint16_t src; uint16_t dst; uint16_t len; uint16_t sum;`

---

## SCTP (`cnet_sctp.h`)

### `struct cnet_sctp`
`uint16_t sport; uint16_t dport; uint32_t vtag; uint32_t sum;`

### Chunk structures
| Struct | Fields |
|---|---|
| `cnet_sctp_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_data_chunk` | `type`, `flags`, `len`, `tsn`, `sid`, `sseq`, `ppid` |
| `cnet_sctp_init_chunk` | `type`, `flags`, `len`, `itag`, `arwnd`, `nos`, `nis`, `itsn` |
| `cnet_sctp_init_ack_chunk` | same as INIT |
| `cnet_sctp_sack_chunk` | `type`, `flags`, `len`, `cta`, `arwnd`, `nga`, `ndt` |
| `cnet_sctp_param` | `type`, `len` |
| `cnet_sctp_gap_ack` | `start`, `end` |
| `cnet_sctp_hbeat_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_hbeat_ack_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_abort_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_shutdown_chunk` | `type`, `flags`, `len`, `cta` |
| `cnet_sctp_shutdown_ack_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_shutdown_complete_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_error_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_error_c_chunk` | `cc`, `cl` |
| `cnet_sctp_cookie_echo_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_cookie_ack_chunk` | `type`, `flags`, `len` |
| `cnet_sctp_tsn_stream` | `sid`, `sseq` |
| `cnet_sctp_tsn_chunk` | `type`, `flags`, `len`, `nct` |
| `cnet_sctp_ecne_chunk` | `type`, `flags`, `len`, `ltsn` |
| `cnet_sctp_cwr_chunk` | `type`, `flags`, `len`, `ltsn` |
| `cnet_sctp_auth_chunk` | `type`, `flags`, `len`, `skey`, `hmac` |
| `cnet_sctp_asconf_chunk` | `type`, `flags`, `len`, `serial` |
| `cnet_sctp_asconf_ack_chunk` | `type`, `flags`, `len`, `serial` |
| `cnet_sctp_asconf_param` | `type`, `len`, `id` |
| `cnet_sctp_asconf_res` | `type`, `len`, `id` |

Chunk type, flag, parameter, error-cause and ASCONF constants are listed in
[defines.md](defines.md#sctp).

---

## DHCP, DNS, NTP, TLS

### `struct cnet_dhcp`
| Field | Type |
|---|---|
| `op`, `htype`, `hlen`, `hops` | `uint8_t` |
| `xid` | `uint32_t` |
| `secs`, `flags` | `uint16_t` |
| `caddr`, `yaddr`, `naddr`, `gaddr` | unions of `uint8_t[4]` / `uint32_t` |
| `chaddr[16]`, `hostname[64]`, `file[128]` | `uint8_t` |
| `cookie` | `uint32_t` |

Aliases: `CNET_DHCP_MACROS_CADDR8/32`, `_YADDR8/32`, `_NADDR8/32`,
`_GADDR8/32`.

### `struct cnet_dhcp_option`
`uint8_t code; uint8_t len;`

### `struct cnet_dns`
`uint16_t id, flags, qdc, anc, nsc, arc;`

### `struct cnet_dns_question`
`uint16_t type; uint16_t class;`

### `struct cnet_dns_record`
`uint16_t type; uint16_t class; uint32_t ttl; uint16_t len;`

### `struct cnet_short`
`uint16_t sec; uint16_t frc;` — short NTP timestamp.

### `struct cnet_ntp_timestamp`
`uint32_t sec; uint32_t frc;`

### `struct cnet_ntp`
| Field | Type | Meaning |
|---|---|---|
| `li` | 2-bit | leap indicator |
| `vn` | 3-bit | version |
| `md` | 3-bit | mode |
| `strat` | `uint8_t` | stratum |
| `poll` | `uint8_t` | poll exponent |
| `prec` | `uint8_t` | precision |
| `delay` | `cnet_ntp_short` | root delay |
| `disper` | `cnet_ntp_short` | root dispersion |
| `ref_id` | `uint32_t` | reference id |
| `ref_tm` | `cnet_ntp_timestamp` | reference timestamp |
| `org_tm` | `cnet_ntp_timestamp` | originate timestamp |
| `recv_tm` | `cnet_ntp_timestamp` | receive timestamp |
| `trsm_tm` | `cnet_ntp_timestamp` | transmit timestamp |

> `cnet_ntp` refers to `struct cnet_ntp_short`, while the header defines
> `struct cnet_short`. See [caveats.md](caveats.md).

### `struct cnet_tls_record`
`uint8_t type; uint16_t version; uint16_t len;`

### `struct cnet_tls_handshake`
`uint8_t type; uint8_t len[3];`

---

## Routing (`cnet_route.h`)

### `struct cnet_route` — Linux
| Field | Type | Meaning |
|---|---|---|
| `iface[32][IFNAMSIZ]` | `char` | interface names |
| `active[IFNAMSIZ]` | `char` | selected interface |
| `count` | `int` | number of interfaces |
| `mac[6]` | `uint8_t` | selected interface MAC |
| `ip` | union | `i8p[4]` / `i32p` local IPv4 |

### `struct cnet_route` — Windows
Adds `local[16][16]`, `local_count`, `smac[6]`, `dmac[6]`; `ip` is the gateway
address. There is no `CNET_PLATFORM_ESP32` definition of `cnet_route`.

---

## 802.11 frames (`cnet_frame/`)

### `struct cnet_frame_type`
| Field | Type | Meaning |
|---|---|---|
| `version` | 2-bit | protocol version |
| `type` | 2-bit | management / control / data |
| `stype` | 4-bit | subtype |
| `flags` | 8-bit | ToDS/FromDS/… |

### `struct cnet_frame_hdr`
| Field | Type |
|---|---|
| `cnet_type` | `struct cnet_frame_type` |
| `id` | `uint16_t` |
| `addr1[6]`, `addr2[6]`, `addr3[6]` | `uint8_t` |
| `seq` | `uint16_t` |

### Management frames
| Struct | Extra fields |
|---|---|
| `cnet_frame_assoc_req` | `info`, `interval` |
| `cnet_frame_assoc_resp` | `info`, `code`, `id` |
| `cnet_frame_reassoc_req` | `info`, `interval`, `ap[6]` |
| `cnet_frame_reassoc_resp` | `info`, `code`, `id` |
| `cnet_frame_probe_req` | `cnet_probe_body`, `cnet_probe_rate` |
| `cnet_frame_probe_body` | `eid`, `len` |
| `cnet_frame_probe_rates` | `eid`, `len`, `rates[8]` |
| `cnet_frame_probe_resp` | `tstamp`, `interval`, `info` |
| `cnet_frame_beacon` | `tstamp`, `bint`, `cinfo` |
| `cnet_frame_atim` | (header only) |
| `cnet_frame_diassoc` | `code` |
| `cnet_frame_auth` | `auth`, `seq`, `code` |
| `cnet_frame_deauth` | `code` |
| `cnet_frame_action` | `category`, `action` |
| `cnet_frame_action_na` | `category`, `action` |
| `cnet_frame_adv` | `tstamp`, `info` |

### Control frames
| Struct | Extra fields |
|---|---|
| `cnet_frame_psp` | `aid`, `bssid[6]`, `ta[6]` |
| `cnet_frame_rts` | `id`, `ra[6]`, `ta[6]` |
| `cnet_frame_cts` | `id`, `ra[6]` |
| `cnet_frame_ack` | `id`, `ra[6]` |
| `cnet_frame_cf` | `id`, `ra[6]`, `bssid[6]` |
| `cnet_frame_cf_ack` | `id`, `ra[6]`, `bssid[6]` |
| `cnet_frame_ctlw` | `id`, `ra[6]`, `frame`, `ht[4]` |
| `cnet_frame_trigger` | `id`, `ra[6]`, `ta[6]`, `info` |
| `cnet_frame_block_ackr` | `id`, `ra[6]`, `ta[6]`, `ctl`, `info` |

### `struct cnet_frame_block_ack`
Fields `id`, `ra[6]`, `ta[6]`, `ctl`, `info`, plus union `map`
(`b8it[128]`, `b16it[64]`, `b32it[32]`, `b64it[16]`).

Aliases: `CNET_FRAME_BLOCK_MACROS_BIT8/16/32/64`.

### Data frames
| Struct | Extra fields |
|---|---|
| `cnet_frame_data` | (header only) |
| `cnet_frame_null` | (header only) |
| `cnet_frame_data_wds` | `addr4[6]` |
| `cnet_frame_qos_data` | `qos` |
| `cnet_frame_qos_data_wds` | `addr4[6]`, `qos` |
| `cnet_frame_qos_data_ht` | `qos`, `ht` |
| `cnet_frame_qos_data_merge` | `addr4[6]`, `qos`, `ht` |
| `cnet_frame_qos_null` | `qos` |

Every frame struct ends with `uint8_t fcs[4]`.

Header aliases: `CNET_FRAME_MACROS_ID`, `_ADDR1`, `_ADDR2`, `_ADDR3`,
`_SEQ`, `_VERSION`, `_TYPE`, `_STYPE`, `_FLAGS`; for frames whose first member
is `cnet_type` rather than `cnet_hdr`: `CNET_FRAME_MACROS_RVERSION`, `_RTYPE`,
`_RSTYPE`, `_RFLAGS`.
