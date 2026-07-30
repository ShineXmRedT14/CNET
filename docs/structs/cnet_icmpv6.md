# Structs — `cnet_icmpv6.h`

## Payload variants

### `struct cnet_icmpv6_echo`

Used by Echo Request/Reply (`CNET_ICMPV6_ECHO_REQ` / `CNET_ICMPV6_ECHO_REP`).

| Field | Type | Meaning |
|---|---|---|
| `id` | `uint16_t` | Identifier |
| `seq` | `uint16_t` | Sequence number |

### `struct cnet_icmpv6_ra`

Used by Router Advertisement (`CNET_ICMPV6_RA`).

| Field | Type | Meaning |
|---|---|---|
| `ttl` | `uint8_t` | Cur Hop Limit to advertise |
| `flags` | `uint8_t` | `CNET_ICMPV6_RA_MANAGED`/`OTHER`/`AGENT`/`PROXY`/`PRF_*` OR'd together |
| `ltime` | `uint16_t` | Router Lifetime, seconds |

### `struct cnet_icmpv6_na`

Used by Neighbor Advertisement (`CNET_ICMPV6_NA`). Build `rso` with `CNET_ICMPV6_RSO()` and a `struct cnet_dicmpv6`.

| Field | Type | Meaning |
|---|---|---|
| `rso` | `uint32_t` | Packed Router (R) / Solicited (S) / Override (O) flags in the top 3 bits, remainder reserved |

### `struct cnet_icmpv6_mtu`

Used by Packet Too Big (`CNET_ICMPV6_MTU`).

| Field | Type | Meaning |
|---|---|---|
| `mtu` | `uint32_t` | MTU of the next-hop link |

## `struct cnet_icmpv6`

The ICMPv6 header — 4 fixed bytes plus a 4-byte payload union whose meaning depends on `type`.

```c
struct cnet_icmpv6
{
    uint8_t type;
    uint8_t code;
    uint16_t sum;
    union
    {
        union { uint32_t raw32; uint16_t raw16[2]; uint8_t raw8[4]; } raw;
        struct cnet_icmpv6_echo echo;
        struct cnet_icmpv6_ra ra;
        struct cnet_icmpv6_na na;
        struct cnet_icmpv6_mtu mtu;
    } icmp;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Message type — one of `CNET_ICMPV6_*` |
| `code` | `uint8_t` | Message code, meaning depends on `type` |
| `sum` | `uint16_t` | Checksum — ICMPv6 checksums cover a pseudo-header like TCP/UDP, use [`CNET_L4_CSUM()`](../functions/CNET_L4_CSUM.md) |
| `icmp` | union | Payload — use `CNET_ICMPV6_MACROS_RAW` / `ECHO` / `RA` / `NA` / `MTU` |

## See also

- Defines: [`docs/defines/cnet_icmpv6.md`](../defines/cnet_icmpv6.md)
- `CNET_ICMPV6_RSO` — builds `struct cnet_icmpv6_na.rso` from a `struct cnet_dicmpv6`
