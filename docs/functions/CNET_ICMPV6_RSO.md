# `CNET_ICMPV6_RSO`

```c
static inline void CNET_ICMPV6_RSO(void *icmpv6, struct cnet_dicmpv6 *dicmpv6);
```

## Description

Packs a Neighbor Advertisement's Router / Solicited / Override flags (`struct cnet_icmpv6_na.rso`, a `uint32_t`) from the individual flag values held in a `struct cnet_dicmpv6`, and writes the result directly into the target ICMPv6 header's `na` payload.

Mirrors [`CNET_IPV6_VTC()`](CNET_IPV6_VTC.md) and `CNET_VLAN_VDP()` — same "decomposed struct in, packed bitfield out" pattern used throughout CNET for fields that mix several small bit-packed values.

## Parameters

- `void *icmpv6` — pointer to the `struct cnet_icmpv6` whose `icmp.na.rso` field will be set
- `struct cnet_dicmpv6 *dicmpv6` — pointer to the struct holding the decomposed `r` (Router), `s` (Solicited), and `o` (Override) flag values

## Example

```c
struct cnet_icmpv6 icmpv6 = {0};
icmpv6.type = CNET_ICMPV6_NA;
struct cnet_dicmpv6 rso = { .r = 1, .s = 1, .o = 0 };
CNET_ICMPV6_RSO(&icmpv6, &rso);
```

## See also

- `struct cnet_dicmpv6` — [`docs/structs/cnet.md`](../structs/cnet.md)
- `struct cnet_icmpv6_na` — [`docs/structs/cnet_icmpv6.md`](../structs/cnet_icmpv6.md)
- [`CNET_IPV6_VTC`](CNET_IPV6_VTC.md), [`CNET_VLAN_VDP`](CNET_VLAN_VDP.md)
