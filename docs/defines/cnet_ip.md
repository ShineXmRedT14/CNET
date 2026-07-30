# Defines — `cnet_ip.h`

## Protocol numbers (for `struct cnet_ip.proto`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_IP_ICMP` | `0x01` | ICMP |
| `CNET_IP_IGMP` | `0x02` | IGMP |
| `CNET_IP_TCP` | `0x06` | TCP |
| `CNET_IP_UDP` | `0x11` | UDP |
| `CNET_IP_RAW` | `0xFF` | Raw / reserved placeholder |

## Header defaults

| Define | Value | Meaning |
|---|---|---|
| `CNET_IP_VERSION` | `0x04` | IPv4 version nibble, for `struct cnet_ip.version` |
| `CNET_IP_TYPE` | `0x00` | Default Type of Service / DSCP byte |
| `CNET_IP_TTL` | `0x40` | Default Time To Live (64) |

## Timestamp option flags (for `struct cnet_ip_timestamp.flags`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_IP_TSP_ONLY` | `0x00` | Timestamps only |
| `CNET_IP_TSP_ADDR` | `0x01` | Each timestamp preceded by the recording address |
| `CNET_IP_TSP_DEFINED` | `0x03` | Addresses pre-specified by the sender; only those hosts record a timestamp |

## Field-access macros

Convenience aliases into the anonymous unions of `struct cnet_ip` / `struct cnet_ip_bytes`, so you can write `ip.CNET_IP_MACROS_SRC32` instead of `ip.src.s32rc`.

| Define | Expands to | Use |
|---|---|---|
| `CNET_IP_MACROS_SRC8` | `src.s8rc` | Source address as a `uint8_t[4]` |
| `CNET_IP_MACROS_SRC32` | `src.s32rc` | Source address as a single `uint32_t` |
| `CNET_IP_MACROS_DST8` | `dst.d8st` | Destination address as a `uint8_t[4]` |
| `CNET_IP_MACROS_DST32` | `dst.d32st` | Destination address as a single `uint32_t` |
| `CNET_IP_MACROS_BYTES8` | `bytes.b8its` | `struct cnet_ip_bytes` viewed as `uint8_t[4]` |
| `CNET_IP_MACROS_BYTES32` | `bytes.b32its` | `struct cnet_ip_bytes` viewed as a single `uint32_t` |

## See also

- Struct reference: [`docs/structs/cnet_ip.md`](../structs/cnet_ip.md)
