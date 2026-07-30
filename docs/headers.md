# Headers

Overview of every public header in the CNET library, grouped by what
they're for. Include only what you need — each header is independent
unless noted otherwise.

## Core

| Header | Purpose |
|---|---|
| `cnet.h` | Umbrella header — core types, error codes, and the `CNET_*` helper functions documented under [`docs/functions`](functions/). |

## Layer 3 / Layer 4 protocols

| Header | Purpose |
|---|---|
| `cnet_ip.h` | IPv4 header definitions |
| `cnet_ipv6.h` | IPv6 header definitions |
| `cnet_tcp.h` | TCP header definitions |
| `cnet_udp.h` | UDP header definitions |
| `cnet_sctp.h` | SCTP header definitions |
| `cnet_icmp.h` | ICMP (IPv4) header definitions |
| `cnet_icmpv6.h` | ICMPv6 header definitions |

## Buffers & routing

| Header | Purpose |
|---|---|
| `cnet_buffer.h` | `struct cnet_buffer` — the multi-packet buffer used by `CNET_BURST()` and friends |
| `cnet_route.h` | `struct cnet_route` — local interface / IP / MAC info used by `CNET_GET_IF()`, `CNET_GET_IP()`, `CNET_GET_MAC()` |

## Layer 2 & other protocols

| Header | Purpose |
|---|---|
| `cnet_arp.h` | ARP header definitions |
| `cnet_ether.h` | Ethernet header definitions |
| `cnet_dns.h` | DNS message definitions |
| `cnet_dhcp.h` | DHCP message definitions |
| `cnet_igmp.h` | IGMP header definitions |
| `cnet_tls.h` | TLS record/handshake definitions |
| `cnet_vlan.h` | 802.1Q VLAN tag definitions |

## 802.11 (Wi-Fi) frames — `cnet_frame/`

Management, control, and data frame formats for raw 802.11 handling.

| Header | Frame type |
|---|---|
| `cnet_frame/beacon.h` | Beacon |
| `cnet_frame/probe.h` | Probe request/response |
| `cnet_frame/auth.h` | Authentication |
| `cnet_frame/deauth.h` | Deauthentication |
| `cnet_frame/assoc.h` | Association request/response |
| `cnet_frame/reassoc.h` | Reassociation request/response |
| `cnet_frame/diassoc.h` | Disassociation |
| `cnet_frame/action.h` | Action frames |
| `cnet_frame/atim.h` | Announcement Traffic Indication Message |
| `cnet_frame/adv.h` | Advertisement (public action) frames |
| `cnet_frame/trigger.h` | Trigger frames |
| `cnet_frame/block.h` | Block Ack |
| `cnet_frame/ack.h` | Acknowledgement |
| `cnet_frame/cf.h` | Contention-Free frames |
| `cnet_frame/rts.h` | Request To Send |
| `cnet_frame/cts.h` | Clear To Send |
| `cnet_frame/ctlw.h` | Control Wrapper |
| `cnet_frame/psp.h` | PS-Poll (Power-Save Poll) |
| `cnet_frame/data.h` | Data frames |
| `cnet_frame/hdr.h` | Generic/shared 802.11 frame header |
| `cnet_frame/definition.h` | Shared constants and field definitions used across `cnet_frame/*` |

## Known issues

A few small bugs were found while writing this documentation. None are fixed in the source yet — flagging them here so they don't surprise anyone building against these headers.

| Header | Issue | Fix |
|---|---|---|
| `cnet_frame/action.h` | Include guard is misspelled `#ifnder CNET_FRAME_ACTION_H` instead of `#ifndef`, so the guard never activates. | Change `ifnder` → `ifndef`. |
| `cnet_frame/probe.h` | `struct cnet_frame_probe_resp` is missing the `;` after its last member (`uint16_t info`), which fails to compile as written. | Add the missing `;`. |
| `cnet_frame/rts.h` | `struct cnet_frame_rts` declares its first member as `struct cnet_frame_types cnet_type` (extra `s`) — no such type exists, only `struct cnet_frame_type` from `hdr.h`. | Remove the extra `s`. |

See each struct's page under [`docs/structs/cnet_frame/`](structs/cnet_frame/) for the same notes in context.
