# Defines — `cnet_ether.h`

## EtherType values (for `struct cnet_ether.next`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_ETH_IP` | `0x0800` | IPv4 payload |
| `CNET_ETH_ARP` | `0x0806` | ARP payload |
| `CNET_ETH_RARP` | `0x8035` | RARP payload |
| `CNET_ETH_IPV6` | `0x86DD` | IPv6 payload |
| `CNET_ETH_VLAN` | `0x8100` | 802.1Q tagged frame follows |

All values are in host order; run them through `CNET_BIG16()` before storing into `next`.

## See also

- `CNET_SET_SMAC()` / `CNET_SET_DMAC()` — [`docs/functions/CNET_SET_SMAC.md`](../functions/CNET_SET_SMAC.md), [`docs/functions/CNET_SET_DMAC.md`](../functions/CNET_SET_DMAC.md)
- Struct reference: [`docs/structs/cnet_ether.md`](../structs/cnet_ether.md)
