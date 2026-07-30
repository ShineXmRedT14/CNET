# Structs — `cnet_ipv6.h`

## Extension headers

All extension-header structs below only model the fixed leading bytes common to that header type — the variable-length option/data area that follows isn't part of the struct and should be read/written directly against the packet buffer using `len`.

### `struct cnet_ipv6_ext`

Generic/base extension header shape (used as a fallback when no more specific struct applies).

| Field | Type | Meaning |
|---|---|---|
| `proto` | `uint8_t` | Next Header — protocol number of the header that follows |
| `len` | `uint8_t` | Header Extension Length, in 8-byte units, not counting the first 8 bytes |

### `struct cnet_ipv6_hbh`

Hop-by-Hop Options header. Same layout as `cnet_ipv6_ext`.

| Field | Type | Meaning |
|---|---|---|
| `proto` | `uint8_t` | Next Header |
| `len` | `uint8_t` | Header Extension Length |

### `struct cnet_ipv6_dst`

Destination Options header. Same layout as `cnet_ipv6_ext`.

| Field | Type | Meaning |
|---|---|---|
| `proto` | `uint8_t` | Next Header |
| `len` | `uint8_t` | Header Extension Length |

### `struct cnet_ipv6_route`

Routing header (fixed portion, RFC 8200).

| Field | Type | Meaning |
|---|---|---|
| `proto` | `uint8_t` | Next Header |
| `len` | `uint8_t` | Header Extension Length |
| `type` | `uint8_t` | Routing type |
| `sleft` | `uint8_t` | Segments Left |

### `struct cnet_ipv6_route0`

Routing header Type 0 (source routing), with the strict source route address map.

| Field | Type | Meaning |
|---|---|---|
| `proto` | `uint8_t` | Next Header |
| `len` | `uint8_t` | Header Extension Length |
| `type` | `uint8_t` | Routing type (0) |
| `sleft` | `uint8_t` | Segments Left |
| `res` | `uint8_t` | Reserved |
| `smap` | `uint8_t[3]` | Start of the strict/loose source route address list (addresses continue past the struct) |

### `struct cnet_ipv6_frag`

Fragment header.

| Field | Type | Meaning |
|---|---|---|
| `proto` | `uint8_t` | Next Header |
| `res` | `uint8_t` | Reserved |
| `off` | `uint16_t` | Fragment Offset (13 bits) + Res (2 bits) + M flag (1 bit), big-endian on the wire |
| `id` | `uint32_t` | Identification |

## Options (used inside Hop-by-Hop / Destination Options headers)

### `struct cnet_ipv6_opt`

Generic TLV option header.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Option Type — `CNET_IPV6_PAD1` / `CNET_IPV6_PADN` / others |
| `len` | `uint8_t` | Option Data Length (not present at all for Pad1) |

### `struct cnet_ipv6_opt_jumbo`

Jumbo Payload option (RFC 2675).

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Option Type |
| `len` | `uint8_t` | Option Data Length (4) |
| `jumbo` | `uint8_t[4]` | Jumbo payload length, big-endian 32-bit value stored byte-wise |

### `struct cnet_ipv6_opt_nsap`

NSAP-style address option (source/destination lengths).

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Option Type |
| `len` | `uint8_t` | Option Data Length |
| `lsrc` | `uint8_t` | Source NSAP address length |
| `ldst` | `uint8_t` | Destination NSAP address length |

### `struct cnet_ipv6_opt_tunnel`

Tunnel Encapsulation Limit option (RFC 2473).

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Option Type |
| `len` | `uint8_t` | Option Data Length (1) |
| `limit` | `uint8_t` | Tunnel encapsulation limit |

### `struct cnet_ipv6_opt_router`

Router Alert option (RFC 2711).

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Option Type |
| `len` | `uint8_t` | Option Data Length (2) |
| `value` | `uint8_t[2]` | Alert value, big-endian |

## `struct cnet_ipv6`

The fixed 40-byte IPv6 header.

```c
struct cnet_ipv6
{
    uint32_t vtc;
    uint16_t plen;
    uint8_t proto;
    uint8_t ttl;
    uint8_t src[16];
    uint8_t dst[16];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `vtc` | `uint32_t` | Packed Version (4 bits) + Traffic Class (8 bits) + Flow Label (20 bits) — build with [`CNET_IPV6_VTC()`](../functions/CNET_IPV6_VTC.md) and a `struct cnet_dipv6` |
| `plen` | `uint16_t` | Payload Length (everything after this header), big-endian |
| `proto` | `uint8_t` | Next Header — `CNET_IPV6_TCP` / `CNET_IPV6_UDP` / `CNET_IPV6_ICMPV6` / an extension header type |
| `ttl` | `uint8_t` | Hop Limit |
| `src` | `uint8_t[16]` | Source address |
| `dst` | `uint8_t[16]` | Destination address |

## See also

- Defines: [`docs/defines/cnet_ipv6.md`](../defines/cnet_ipv6.md)
- Struct used to fill `vtc`: [`docs/structs/cnet.md`](cnet.md#struct-cnet_dipv6)
- [`CNET_IPV6_VTC`](../functions/CNET_IPV6_VTC.md)
