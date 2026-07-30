# Defines — `cnet_dns.h`

## Header flag bits (for `struct cnet_dns.flags`, big-endian 16-bit field)

| Define | Value | Meaning |
|---|---|---|
| `CNET_DNS_FLAG_QR` | `0x8000` | Query (0) / Response (1) |
| `CNET_DNS_FLAG_OPCODE` | `0x7800` | 4-bit opcode mask |
| `CNET_DNS_FLAG_AA` | `0x0400` | Authoritative Answer |
| `CNET_DNS_FLAG_TC` | `0x0200` | Truncated |
| `CNET_DNS_FLAG_RD` | `0x0100` | Recursion Desired |
| `CNET_DNS_FLAG_RA` | `0x0080` | Recursion Available |
| `CNET_DNS_FLAG_RES` | `0x0070` | Reserved / Z bits |
| `CNET_DNS_FLAG_RCODE` | `0x000F` | 4-bit response code mask |

## Opcodes (masked from `CNET_DNS_FLAG_OPCODE`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_DNS_OPCODE_QUERY` | `0x0000` | Standard query |
| `CNET_DNS_OPCODE_IQUERY` | `0x0001` | Inverse query |
| `CNET_DNS_OPCODE_STATUS` | `0x0002` | Server status request |
| `CNET_DNS_OPCODE_NOTIFY` | `0x0004` | Zone change notification |
| `CNET_DNS_OPCODE_UPDATE` | `0x0005` | Dynamic update |

## Response codes (masked from `CNET_DNS_FLAG_RCODE`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_DNS_RCODE_NERR` | `0x0000` | No error |
| `CNET_DNS_RCODE_FMERR` | `0x0001` | Format error |
| `CNET_DNS_RCODE_SFAIL` | `0x0002` | Server failure |
| `CNET_DNS_RCODE_NXDOMAIN` | `0x0003` | Name does not exist |
| `CNET_DNS_RCODE_NTIMP` | `0x0004` | Not implemented |
| `CNET_DNS_RCODE_REFUSED` | `0x0005` | Refused |

## Record types (for `struct cnet_dns_question.type` / `struct cnet_dns_record.type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_DNS_TYPE_A` | `0x0001` | IPv4 address |
| `CNET_DNS_TYPE_NS` | `0x0002` | Name server |
| `CNET_DNS_TYPE_CNAME` | `0x0005` | Canonical name |
| `CNET_DNS_TYPE_SOA` | `0x0006` | Start of authority |
| `CNET_DNS_TYPE_PTR` | `0x000C` | Pointer (reverse DNS) |
| `CNET_DNS_TYPE_MX` | `0x000F` | Mail exchange |
| `CNET_DNS_TYPE_TXT` | `0x0010` | Text record |
| `CNET_DNS_TYPE_AAAA` | `0x001C` | IPv6 address |
| `CNET_DNS_TYPE_SRV` | `0x0021` | Service locator |
| `CNET_DNS_TYPE_OPT` | `0x0029` | EDNS0 pseudo-record |

## Class

| Define | Value | Meaning |
|---|---|---|
| `CNET_DNS_CLASS` | `0x0001` | Internet (IN) class |

## See also

- Struct reference: [`docs/structs/cnet_dns.md`](../structs/cnet_dns.md)
