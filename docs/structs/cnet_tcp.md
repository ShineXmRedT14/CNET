# Structs — `cnet_tcp.h`

## `enum cnet_state_ca`

Congestion-avoidance state, used by `struct cnet_tcp_info.cnet_state_ca`.

| Value | Value | Meaning |
|---|---|---|
| `CNET_TCP_OPEN` | `0` | Normal operation |
| `CNET_TCP_DISC` | `1` | Disorder — out-of-order segments detected |
| `CNET_TCP_CWR` | `2` | Congestion Window Reduced |
| `CNET_TCP_RECOV` | `3` | Fast Recovery |
| `CNET_TCP_LOSS` | `4` | Loss recovery (RTO fired) |

## `struct cnet_tcp_info`

A connection-statistics snapshot, mirroring the shape of the kernel's `struct tcp_info` (`TCP_INFO` socket option) rather than something placed on the wire.

| Field | Type | Meaning |
|---|---|---|
| `state` | `uint8_t` | TCP connection state (`ESTABLISHED`, etc., kernel-defined values) |
| `cnet_state_ca` | `uint8_t` | Congestion-avoidance state — see `enum cnet_state_ca` above |
| `probes` | `uint8_t` | Number of unanswered zero-window probes |
| `off` | `uint8_t` | (Kernel `tcp_info` compatible) reordering/offset field |
| `options` | `uint8_t` | Negotiated option bitmask (SACK/timestamps/window scale, kernel-defined) |
| `sscale` | `4 bits` | Send-side window scale |
| `rscale` | `4 bits` | Receive-side window scale |
| `rto` | `uint32_t` | Retransmission timeout, µs |
| `ato` | `uint32_t` | Delayed-ACK timeout, µs |
| `smss` | `uint32_t` | Send MSS |
| `rmss` | `uint32_t` | Receive MSS |
| `unack` | `uint32_t` | Unacknowledged bytes in flight |
| `sack` | `uint32_t` | SACK'd segments |
| `lost` | `uint32_t` | Lost segments |
| `ret` | `uint32_t` | Retransmitted segments |
| `ackets` | `uint32_t` | Segments ACKed |
| `lds` | `uint32_t` | Time since last data sent, ms |
| `las` | `uint32_t` | Time since last ACK received, ms |
| `ldr` | `uint32_t` | Time since last data received, ms |
| `lar` | `uint32_t` | Last advertised receive window |
| `pmtu` | `uint32_t` | Path MTU |
| `rth` | `uint32_t` | Receiver-side RTT estimate helper |
| `rtt` | `uint32_t` | Smoothed round-trip time, µs |
| `rttv` | `uint32_t` | RTT variance, µs |
| `sth` | `uint32_t` | Slow-start threshold |
| `cwnd` | `uint32_t` | Congestion window |
| `advmss` | `uint32_t` | Advertised MSS |
| `reorder` | `uint32_t` | Reordering metric |
| `rrtt` | `uint32_t` | Receiver RTT |
| `rspace` | `uint32_t` | Receive-buffer space |
| `tret` | `uint32_t` | Total retransmits for the connection |

## `struct cnet_tcp`

The 20-byte TCP header (no options).

```c
struct cnet_tcp
{
    uint16_t src;
    uint16_t dst;
    uint32_t seq;
    uint32_t aseq;
    uint16_t res1:4;
    uint16_t doff:4;
    uint16_t flags:6;
    uint16_t res2:2;
    uint16_t win;
    uint16_t sum;
    uint16_t uptr;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `src` | `uint16_t` | Source port — big-endian, use `CNET_BIG16()` |
| `dst` | `uint16_t` | Destination port — big-endian |
| `seq` | `uint32_t` | Sequence number — big-endian, use `CNET_BIG32()` |
| `aseq` | `uint32_t` | Acknowledgment number — big-endian |
| `res1` | `4 bits` | Reserved — `CNET_TCP_RES1` (0) |
| `doff` | `4 bits` | Data Offset in 32-bit words — `CNET_TCP_DOFF` (5) with no options |
| `flags` | `6 bits` | `CNET_TCP_FIN` / `SYN` / `RST` / `PSH` / `ACK` / `URG`, OR'd together |
| `res2` | `2 bits` | Reserved — `CNET_TCP_RES2` (0) |
| `win` | `uint16_t` | Window size — big-endian, `CNET_TCP_WIN` for max |
| `sum` | `uint16_t` | Checksum — fill with [`CNET_L4_CSUM()`](../functions/CNET_L4_CSUM.md) |
| `uptr` | `uint16_t` | Urgent Pointer — valid only when `CNET_TCP_URG` is set |

Field order for `res1`/`doff` and `flags`/`res2` flips between little- and big-endian builds to preserve the on-wire layout.

## See also

- Defines: [`docs/defines/cnet_tcp.md`](../defines/cnet_tcp.md)
- [`CNET_L4_CSUM`](../functions/CNET_L4_CSUM.md)
