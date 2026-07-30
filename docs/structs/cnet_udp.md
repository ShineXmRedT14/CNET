# Structs — `cnet_udp.h`

## `struct cnet_udp`

The 8-byte UDP header.

```c
struct cnet_udp
{
    uint16_t src;
    uint16_t dst;
    uint16_t len;
    uint16_t sum;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `src` | `uint16_t` | Source port — big-endian, use `CNET_BIG16()` |
| `dst` | `uint16_t` | Destination port — big-endian |
| `len` | `uint16_t` | Length of the UDP header + payload, big-endian |
| `sum` | `uint16_t` | Checksum (optional in IPv4, mandatory in IPv6) — fill with [`CNET_L4_CSUM()`](../functions/CNET_L4_CSUM.md) |

## See also

- [`CNET_L4_CSUM`](../functions/CNET_L4_CSUM.md)
