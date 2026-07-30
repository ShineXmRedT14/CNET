# Defines — `cnet_icmp.h`

## Message types (for `struct cnet_icmp.type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_ICMP_EREPLY` | `0x00` | Echo Reply |
| `CNET_ICMP_UNREACH` | `0x03` | Destination Unreachable |
| `CNET_ICMP_QUENCH` | `0x04` | Source Quench |
| `CNET_ICMP_ECHO` | `0x08` | Echo Request |
| `CNET_ICMP_EXCEEDED` | `0x0B` | Time Exceeded |
| `CNET_ICMP_PROB` | `0x0C` | Parameter Problem |
| `CNET_ICMP_TIMESTAMP` | `0x0D` | Timestamp Request |
| `CNET_ICMP_TIMEREPLY` | `0x0E` | Timestamp Reply |
| `CNET_ICMP_IREQ` | `0x0F` | Information Request |
| `CNET_ICMP_IREP` | `0x10` | Information Reply |
| `CNET_ICMP_ADDR` | `0x11` | Address Mask Request |
| `CNET_ICMP_ADDRREPLY` | `0x12` | Address Mask Reply |

## Codes (for `struct cnet_icmp.code`, meaning depends on `type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_ICMP_CODE_NET_UNREACH` | `0x00` | Network unreachable |
| `CNET_ICMP_CODE_HOST_UNREACH` | `0x01` | Host unreachable |
| `CNET_ICMP_CODE_PROTOCOL_UNREACH` | `0x02` | Protocol unreachable |
| `CNET_ICMP_CODE_PORT_UNREACH` | `0x03` | Port unreachable |
| `CNET_ICMP_CODE_FRAG_NEEDED` | `0x04` | Fragmentation needed, DF set |
| `CNET_ICMP_CODE_SOURCE_ROUTE_FAILED` | `0x05` | Source route failed |
| `CNET_ICMP_CODE_NET_UNKNOWN` | `0x06` | Destination network unknown |
| `CNET_ICMP_CODE_HOST_UNKNOWN` | `0x07` | Destination host unknown |
| `CNET_ICMP_CODE_HOST_ISOLATED` | `0x08` | Source host isolated |
| `CNET_ICMP_CODE_NET_PROHIBITED` | `0x09` | Communication with network prohibited |
| `CNET_ICMP_CODE_HOST_PROHIBITED` | `0x0A` | Communication with host prohibited |
| `CNET_ICMP_CODE_NET_UNREACH_TOS` | `0x0B` | Network unreachable for this TOS |
| `CNET_ICMP_CODE_HOST_UNREACH_TOS` | `0x0C` | Host unreachable for this TOS |
| `CNET_ICMP_CODE_FILTER_PROHIBITED` | `0x0D` | Communication administratively prohibited |
| `CNET_ICMP_CODE_HOST_PRECEDENCE` | `0x0E` | Host precedence violation |
| `CNET_ICMP_CODE_PRECEDENCE_CUTOFF` | `0x0F` | Precedence cutoff in effect |
| `CNET_ICMP_CODE_REDIRECT_NET` | `0x00` | Redirect for network (used with a redirect-type message) |
| `CNET_ICMP_CODE_REDIRECT_HOST` | `0x01` | Redirect for host |
| `CNET_ICMP_CODE_REDIRECT_TOS_NET` | `0x02` | Redirect for TOS and network |
| `CNET_ICMP_CODE_REDIRECT_TOS_HOST` | `0x03` | Redirect for TOS and host |
| `CNET_ICMP_CODE_TTL_EXPIRED` | `0x00` | TTL expired in transit (used with `CNET_ICMP_EXCEEDED`) |
| `CNET_ICMP_CODE_FRAG_REASSEMBLY` | `0x01` | Fragment reassembly time exceeded |
| `CNET_ICMP_CODE_POINTER_ERROR` | `0x00` | Pointer indicates the error (used with `CNET_ICMP_PROB`) |
| `CNET_ICMP_CODE_MISSING_OPTION` | `0x01` | Missing a required option |
| `CNET_ICMP_CODE_BAD_LENGTH` | `0x02` | Bad length |

> Note: several code values (e.g. `0x00`–`0x03`) are reused across different `type`s (unreachable vs. redirect vs. time-exceeded vs. parameter-problem); always pick the constant matching the `type` you're building.

## Field-access macros

| Define | Expands to | Use |
|---|---|---|
| `CNET_ICMP_MACROS_RAW` | `icmp.raw` | Raw view of the 4-byte payload union (`raw32`/`raw16[2]`/`raw8[4]`) |
| `CNET_ICMP_MACROS_ECHO` | `icmp.echo` | View as `struct cnet_icmp_echo` (id/seq) |
| `CNET_ICMP_MACROS_GATE` | `icmp.gate` | View as `struct cnet_icmp_gate` (redirect gateway) |
| `CNET_ICMP_MACROS_MTU` | `icmp.mtu` | View as `struct cnet_icmp_mtu` (unused/mtu) |

## See also

- Struct reference: [`docs/structs/cnet_icmp.md`](../structs/cnet_icmp.md)
