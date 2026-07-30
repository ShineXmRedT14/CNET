# Structs — `cnet_igmp.h`

## `struct cnet_igmp`

The IGMP header (IGMPv2-style, 8 bytes).

```c
struct cnet_igmp
{
    uint8_t type;
    uint8_t code;
    uint16_t sum;
    uint32_t group;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Message type (Membership Query / Report / Leave — no `CNET_IGMP_*` constants are defined yet, use the raw IGMP type values) |
| `code` | `uint8_t` | Max Response Time (Query) or unused (0) for Report/Leave |
| `sum` | `uint16_t` | Checksum — fill with [`CNET_L3_CSUM()`](../functions/CNET_L3_CSUM.md) |
| `group` | `uint32_t` | Multicast group address |

> `cnet_igmp.h` currently has no `#define`d message-type constants — see [`docs/headers.md`](../headers.md) for the header index.

## See also

- [`CNET_L3_CSUM`](../functions/CNET_L3_CSUM.md)
