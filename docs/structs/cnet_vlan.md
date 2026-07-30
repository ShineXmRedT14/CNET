# Structs — `cnet_vlan.h`

## `struct cnet_vlan`

The 4-byte 802.1Q VLAN tag, inserted between the Ethernet header and the next EtherType.

```c
struct cnet_vlan
{
    uint16_t id;
    uint16_t vdp;
    uint16_t ether;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `id` | `uint16_t` | Tag Protocol Identifier — `CNET_VLAN_ID_Q` (0x8100) or `CNET_VLAN_ID_AD` (0x88A8), big-endian |
| `vdp` | `uint16_t` | Packed Priority Code Point (3 bits) + Drop Eligible Indicator (1 bit) + VLAN ID (12 bits) — build with `CNET_VLAN_VDP()` and a `struct cnet_dvlan` |
| `ether` | `uint16_t` | The real EtherType of the frame that follows (e.g. `CNET_ETH_IP`), big-endian |

## See also

- Defines: [`docs/defines/cnet_vlan.md`](../defines/cnet_vlan.md)
- `CNET_VLAN_VDP` and `struct cnet_dvlan` (see [`docs/structs/cnet.md`](cnet.md))
