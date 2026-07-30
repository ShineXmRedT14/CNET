# Structs — `cnet_ether.h`

## `struct cnet_ether_bytes`

A bare 6-byte MAC address, used as the input type for [`CNET_SET_SMAC()`](../functions/CNET_SET_SMAC.md) / [`CNET_SET_DMAC()`](../functions/CNET_SET_DMAC.md) / `CNET_RAND_MAC()` / `CNET_MAC_BYTES()`.

```c
struct cnet_ether_bytes
{
    uint8_t bytes[6];
};
```

| Field | Type | Meaning |
|---|---|---|
| `bytes` | `uint8_t[6]` | The 6 MAC address octets |

## `struct cnet_ether`

The 14-byte Ethernet II header.

```c
struct cnet_ether
{
    uint8_t dst[6];
    uint8_t src[6];
    uint16_t next;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `dst` | `uint8_t[6]` | Destination MAC — set with `CNET_SET_DMAC()` |
| `src` | `uint8_t[6]` | Source MAC — set with `CNET_SET_SMAC()` |
| `next` | `uint16_t` | EtherType — `CNET_ETH_IP` / `ARP` / `IPV6` / `VLAN`, big-endian |

## See also

- Defines: [`docs/defines/cnet_ether.md`](../defines/cnet_ether.md)
- [`CNET_SET_SMAC`](../functions/CNET_SET_SMAC.md), [`CNET_SET_DMAC`](../functions/CNET_SET_DMAC.md), `CNET_RAND_MAC`, [`CNET_MAC_BYTES`](../functions/CNET_MAC_BYTES.md)
