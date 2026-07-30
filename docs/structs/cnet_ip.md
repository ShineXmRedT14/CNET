# Structs — `cnet_ip.h`

## `struct cnet_ip_bytes`

A 4-byte IPv4 address viewed two ways at once, used by `CNET_IP_BYTES()` and `CNET_RAND_IP()`.

```c
struct cnet_ip_bytes
{
    union
    {
        uint8_t b8its[4];
        uint32_t b32its;
    } bytes;
};
```

| Field | Type | Meaning |
|---|---|---|
| `bytes.b8its` | `uint8_t[4]` | Address as four individual octets (dotted-quad order) |
| `bytes.b32its` | `uint32_t` | Same 4 bytes as a single 32-bit value (network byte order once populated by `CNET_IP_BYTES()` / `CNET_RAND_IP()`) |

Access via the `CNET_IP_MACROS_BYTES8` / `CNET_IP_MACROS_BYTES32` macros — see [defines](../defines/cnet_ip.md).

## `struct cnet_ip_timestamp`

The IP Timestamp option (RFC 791 option type 68). Packed, bit-order aware.

| Field | Type | Meaning |
|---|---|---|
| `len` | `uint8_t` | Option length in bytes |
| `ptr` | `uint8_t` | Pointer to the next free timestamp slot |
| `flags` | `4 bits` | One of `CNET_IP_TSP_*` — see [defines](../defines/cnet_ip.md) |
| `flow` | `4 bits` | Overflow counter — number of hosts that couldn't register a timestamp |
| `data` | `uint32_t[9]` | Up to 9 recorded timestamps (and optionally addresses, depending on `flags`) |

`flags`/`flow` swap bit order between little- and big-endian builds so the on-wire layout stays correct either way — this is a recurring pattern throughout CNET's bitfield structs.

## `struct cnet_ip`

The IPv4 header. `packed`, 20 bytes with no options.

```c
struct cnet_ip
{
    uint8_t ihl:4;
    uint8_t version:4;
    uint8_t type;
    uint16_t len;
    uint16_t id;
    uint16_t frag;
    uint8_t ttl;
    uint8_t proto;
    uint16_t sum;
    union { uint8_t s8rc[4]; uint32_t s32rc; } src;
    union { uint8_t d8st[4]; uint32_t d32st; } dst;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `ihl` | `4 bits` | Internet Header Length, in 32-bit words — use `IHL(extra_bytes)` to compute it (see [defines](../defines/cnet.md)) |
| `version` | `4 bits` | IP version — `CNET_IP_VERSION` (4) |
| `type` | `uint8_t` | Type of Service / DSCP+ECN byte |
| `len` | `uint16_t` | Total packet length (header + payload), big-endian on the wire — use `CNET_BIG16()` |
| `id` | `uint16_t` | Identification, for fragment reassembly |
| `frag` | `uint16_t` | Flags (3 bits) + Fragment Offset (13 bits), big-endian on the wire |
| `ttl` | `uint8_t` | Time To Live — `CNET_IP_TTL` default (64) |
| `proto` | `uint8_t` | Next-layer protocol — `CNET_IP_TCP` / `CNET_IP_UDP` / `CNET_IP_ICMP` / etc. |
| `sum` | `uint16_t` | Header checksum — fill with `CNET_L3_CSUM()` |
| `src` | union | Source address; `src.s8rc` (bytes) or `src.s32rc` (32-bit), see `CNET_IP_MACROS_SRC*` |
| `dst` | union | Destination address; `dst.d8st` (bytes) or `dst.d32st` (32-bit), see `CNET_IP_MACROS_DST*` |

`ihl`/`version` and `src`/`dst` change field order between little- and big-endian builds; always use the endian-safe field-access macros (`CNET_IP_MACROS_*`) rather than the raw union member names if you want code that's portable.

## See also

- Defines: [`docs/defines/cnet_ip.md`](../defines/cnet_ip.md)
- [`CNET_L3_CSUM`](../functions/CNET_L3_CSUM.md), [`CNET_IP_BYTES`](../functions/CNET_IP_BYTES.md), [`CNET_RAND_IP`](../functions/CNET_RAND_IP.md)
