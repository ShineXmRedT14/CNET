# Structs — `cnet_icmp.h`

## Payload variants

### `struct cnet_icmp_echo`

Used by Echo Request/Reply (`CNET_ICMP_ECHO` / `CNET_ICMP_EREPLY`).

| Field | Type | Meaning |
|---|---|---|
| `id` | `uint16_t` | Identifier |
| `seq` | `uint16_t` | Sequence number |

### `struct cnet_icmp_mtu`

Used by Destination Unreachable when reporting a needed fragmentation size (`CNET_ICMP_CODE_FRAG_NEEDED`).

| Field | Type | Meaning |
|---|---|---|
| `unused` | `uint16_t` | Unused (must be 0) |
| `mtu` | `uint16_t` | Next-hop MTU |

### `struct cnet_icmp_gate`

Used by Redirect messages.

| Field | Type | Meaning |
|---|---|---|
| `gate` | `uint32_t` | Gateway (router) address to redirect to |

## `struct cnet_icmp`

The ICMP header — 4 fixed bytes plus a 4-byte payload union whose meaning depends on `type`.

```c
struct cnet_icmp
{
    uint8_t type;
    uint8_t code;
    uint16_t sum;
    union
    {
        union { uint32_t raw32; uint16_t raw16[2]; uint8_t raw8[4]; } raw;
        struct cnet_icmp_echo echo;
        struct cnet_icmp_gate gate;
        struct cnet_icmp_mtu mtu;
    } icmp;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Message type — one of `CNET_ICMP_*` |
| `code` | `uint8_t` | Message code — one of `CNET_ICMP_CODE_*`, meaning depends on `type` |
| `sum` | `uint16_t` | Checksum — fill with [`CNET_L3_CSUM()`](../functions/CNET_L3_CSUM.md) (ICMP uses the L3-style checksum, not L4) |
| `icmp` | union | Payload — use the matching `CNET_ICMP_MACROS_*` accessor: `RAW`, `ECHO`, `GATE`, or `MTU` |

## See also

- Defines: [`docs/defines/cnet_icmp.md`](../defines/cnet_icmp.md)
- [`CNET_L3_CSUM`](../functions/CNET_L3_CSUM.md)
