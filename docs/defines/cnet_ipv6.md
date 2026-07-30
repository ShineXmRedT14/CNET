# Defines — `cnet_ipv6.h`

## Next-header / protocol numbers

| Define | Value | Meaning |
|---|---|---|
| `CNET_IPV6_ICMP` | `0x01` | ICMP (legacy, not normally used over IPv6) |
| `CNET_IPV6_TCP` | `0x06` | TCP |
| `CNET_IPV6_UDP` | `0x11` | UDP |
| `CNET_IPV6_ICMPV6` | `0x3A` | ICMPv6 (58) |

## Padding option types (for `struct cnet_ipv6_opt`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_IPV6_PAD1` | `0` | Pad1 — single padding byte, no length field |
| `CNET_IPV6_PADN` | `1` | PadN — multi-byte padding, uses the `len` field |

## See also

- Struct reference: [`docs/structs/cnet_ipv6.md`](../structs/cnet_ipv6.md)
