# Defines — `cnet_icmpv6.h`

## Router Advertisement flags (for `struct cnet_icmpv6_ra.flags`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_ICMPV6_RA_MANAGED` | `0x80` | Managed address configuration (M) flag |
| `CNET_ICMPV6_RA_OTHER` | `0x40` | Other configuration (O) flag |
| `CNET_ICMPV6_RA_AGENT` | `0x20` | Mobile IPv6 home agent (H) flag |
| `CNET_ICMPV6_RA_PRF` | `0x18` | Mask for the 2-bit default router preference field |
| `CNET_ICMPV6_RA_PROXY` | `0x04` | Neighbor discovery proxy (P) flag |
| `CNET_ICMPV6_RA_PRF_HIGH` | `(0x1 << 3)` | Router preference: High |
| `CNET_ICMPV6_RA_PRF_MEDIUM` | `(0x0 << 3)` | Router preference: Medium (default) |
| `CNET_ICMPV6_RA_PRF_LOW` | `(0x3 << 3)` | Router preference: Low |
| `CNET_ICMPV6_RA_PRF_RESERVE` | `(0x2 << 3)` | Router preference: Reserved |

## Message types (for `struct cnet_icmpv6.type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_ICMPV6_ECHO_REQ` | `0x80` | Echo Request (128) |
| `CNET_ICMPV6_ECHO_REP` | `0x81` | Echo Reply (129) |
| `CNET_ICMPV6_RA` | `0x86` | Router Advertisement (134) |
| `CNET_ICMPV6_NA` | `0x88` | Neighbor Advertisement (136) |
| `CNET_ICMPV6_MTU` | `0x02` | Packet Too Big (2) — also used as the payload selector for `struct cnet_icmpv6_mtu` |

## Field-access macros

| Define | Expands to | Use |
|---|---|---|
| `CNET_ICMPV6_MACROS_RAW` | `icmp.raw` | Raw view of the 4-byte payload union |
| `CNET_ICMPV6_MACROS_ECHO` | `icmp.echo` | View as `struct cnet_icmpv6_echo` |
| `CNET_ICMPV6_MACROS_RA` | `icmp.ra` | View as `struct cnet_icmpv6_ra` |
| `CNET_ICMPV6_MACROS_NA` | `icmp.na` | View as `struct cnet_icmpv6_na` — also the field written by `CNET_ICMPV6_RSO()` |
| `CNET_ICMPV6_MACROS_MTU` | `icmp.mtu` | View as `struct cnet_icmpv6_mtu` |

## See also

- `CNET_ICMPV6_RSO()` — [`docs/functions/CNET_ICMPV6_RSO.md`](../functions/CNET_ICMPV6_RSO.md)
- Struct reference: [`docs/structs/cnet_icmpv6.md`](../structs/cnet_icmpv6.md)
