# Structs — `cnet.h` (core)

Small helper structs and the library-wide error enum used by the umbrella header itself (as opposed to a specific protocol).

## `struct cnet_dipv6`

Input to [`CNET_IPV6_VTC()`](../functions/CNET_IPV6_VTC.md) — the three logical fields that get packed into the 32-bit `vtc` word of `struct cnet_ipv6`.

| Field | Type | Meaning |
|---|---|---|
| `version` | `int` | IP version (4 bits used) |
| `tc` | `int` | Traffic Class (8 bits used) |
| `flow` | `uint32_t` | Flow Label (20 bits used) |

## `struct cnet_dicmpv6`

Input to `CNET_ICMPV6_RSO()` — the three flag bits packed into `struct cnet_icmpv6_na.rso`.

| Field | Type | Meaning |
|---|---|---|
| `r` | `int` | Router flag (1 bit used) |
| `s` | `int` | Solicited flag (1 bit used) |
| `o` | `int` | Override flag (1 bit used) |

## `struct cnet_dvlan`

Input to `CNET_VLAN_VDP()` — the three logical fields packed into the 16-bit `vdp` word of `struct cnet_vlan`.

| Field | Type | Meaning |
|---|---|---|
| `v` | `int` | VLAN ID (12 bits used) |
| `d` | `int` | Drop Eligible Indicator (1 bit used) |
| `p` | `int` | Priority Code Point, see `CNET_VLAN_PCP_*` (3 bits used) |

## `cnet_errno_t` (enum)

Return type for the `CNET_GET_IF()` / `CNET_GET_IP()` / `CNET_GET_MAC()` family — a small negative-on-error convention.

| Value | Value | Meaning |
|---|---|---|
| `CNET_SCFL` | `0` | Success |
| `CNET_ERR_IFADDRS` | `-1` | `getifaddrs()` failed |
| `CNET_ERR_SOCKET` | `-2` | `socket()` failed |
| `CNET_ERR_IOCTL` | `-3` | `ioctl()` failed |
| `CNET_ERR_NIFACE` | `-4` | No usable (non-loopback, up, `AF_INET`) interface found |

## See also

- Defines: [`docs/defines/cnet.md`](../defines/cnet.md)
- [`CNET_IPV6_VTC`](../functions/CNET_IPV6_VTC.md), `CNET_ICMPV6_RSO`, `CNET_VLAN_VDP`, [`CNET_GET_IF`](../functions/CNET_GET_IF.md)
