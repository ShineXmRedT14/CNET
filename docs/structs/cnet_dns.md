# Structs — `cnet_dns.h`

## `struct cnet_dns`

The 12-byte DNS message header. Question/answer/authority/additional sections follow immediately after, in the wire format (labels, not fixed structs).

```c
struct cnet_dns
{
    uint16_t id;
    uint16_t flags;
    uint16_t qdc;
    uint16_t anc;
    uint16_t nsc;
    uint16_t arc;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `id` | `uint16_t` | Query/response identifier |
| `flags` | `uint16_t` | QR/Opcode/AA/TC/RD/RA/Z/RCODE, see [defines](../defines/cnet_dns.md) |
| `qdc` | `uint16_t` | Question count |
| `anc` | `uint16_t` | Answer count |
| `nsc` | `uint16_t` | Authority (Name Server) count |
| `arc` | `uint16_t` | Additional record count |

All fields big-endian on the wire.

## `struct cnet_dns_question`

Trailer fields of a question entry — the name itself is a variable-length label sequence that precedes this struct in the packet.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint16_t` | Query type — `CNET_DNS_TYPE_*` |
| `class` | `uint16_t` | Query class — `CNET_DNS_CLASS` (IN) |

## `struct cnet_dns_record`

Trailer fields of an answer/authority/additional resource record — again preceded by a variable-length name.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint16_t` | Record type — `CNET_DNS_TYPE_*` |
| `class` | `uint16_t` | Record class — `CNET_DNS_CLASS` (IN) |
| `ttl` | `uint32_t` | Time to live, seconds |
| `len` | `uint16_t` | Length of the RDATA that follows |

## See also

- Defines: [`docs/defines/cnet_dns.md`](../defines/cnet_dns.md)
