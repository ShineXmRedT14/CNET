# Defines — `cnet_vlan.h`

## Tag protocol identifiers (for `struct cnet_vlan.id`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_VLAN_ID_Q` | `0x8100` | 802.1Q single-tag TPID |
| `CNET_VLAN_ID_AD` | `0x88A8` | 802.1ad (Q-in-Q) TPID |

## Priority Code Point values (for the `p` field consumed by `CNET_VLAN_VDP()`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_VLAN_PCP_BK` | `0x0001` | Background |
| `CNET_VLAN_PCP_BE` | `0x0000` | Best Effort (default) |
| `CNET_VLAN_PCP_EE` | `0x0002` | Excellent Effort |
| `CNET_VLAN_PCP_CA` | `0x0003` | Critical Applications |
| `CNET_VLAN_PCP_VI` | `0x0004` | Video |
| `CNET_VLAN_PCP_VO` | `0x0005` | Voice |
| `CNET_VLAN_PCP_IC` | `0x0006` | Internetwork Control |
| `CNET_VLAN_PCP_NC` | `0x0007` | Network Control |

## See also

- `CNET_VLAN_VDP()` — [`docs/functions/CNET_VLAN_VDP.md`](../functions/CNET_VLAN_VDP.md)
- Struct reference: [`docs/structs/cnet_vlan.md`](../structs/cnet_vlan.md)
