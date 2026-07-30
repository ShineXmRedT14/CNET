# `CNET_VLAN_VDP`

```c
static inline void CNET_VLAN_VDP(void *vlan, struct cnet_dvlan *dvlan);
```

## Description

Packs an 802.1Q VLAN tag's combined Priority Code Point / Drop Eligible Indicator / VLAN ID field (`struct cnet_vlan.vdp`, a `uint16_t`) from the individual integer values held in a `struct cnet_dvlan`, and writes the result directly into the target VLAN tag.

Same pattern as [`CNET_IPV6_VTC()`](CNET_IPV6_VTC.md) and [`CNET_ICMPV6_RSO()`](CNET_ICMPV6_RSO.md).

## Parameters

- `void *vlan` — pointer to the `struct cnet_vlan` whose `vdp` field will be set
- `struct cnet_dvlan *dvlan` — pointer to the struct holding the decomposed `v` (VLAN ID), `d` (Drop Eligible Indicator), and `p` (Priority Code Point, see `CNET_VLAN_PCP_*`) values

## Example

```c
struct cnet_vlan vlan = {0};
vlan.id = CNET_BIG16(CNET_VLAN_ID_Q);
struct cnet_dvlan vdp = { .v = 100, .d = 0, .p = CNET_VLAN_PCP_VO };
CNET_VLAN_VDP(&vlan, &vdp);
vlan.ether = CNET_BIG16(CNET_ETH_IP);
```

## See also

- `struct cnet_dvlan` — [`docs/structs/cnet.md`](../structs/cnet.md)
- `struct cnet_vlan` — [`docs/structs/cnet_vlan.md`](../structs/cnet_vlan.md)
- Defines: [`docs/defines/cnet_vlan.md`](../defines/cnet_vlan.md)
- [`CNET_IPV6_VTC`](CNET_IPV6_VTC.md), [`CNET_ICMPV6_RSO`](CNET_ICMPV6_RSO.md)
