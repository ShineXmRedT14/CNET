# Structs — `cnet_buffer.h`

## `struct cnet_buffer`

The multi-packet send buffer that backs [`CNET_BURST()`](../functions/CNET_BURST.md) and the `CNET_BUFFER_*` / `CNET_SIZE_SET` / `CNET_PACKET_UPTR` family. Holds up to `APY` (64) packets of up to `APX` (2048) bytes each, plus the `iovec`/`mmsghdr` plumbing `sendmmsg()` needs to send them all in one syscall.

```c
struct cnet_buffer
{
    uint8_t buff[APY][APX];
    size_t size[APY];
    struct iovec iov[APY];
    struct mmsghdr msg[APY];
};
```

| Field | Type | Meaning |
|---|---|---|
| `buff` | `uint8_t[APY][APX]` | Raw packet bytes, one slot (row) per packet |
| `size` | `size_t[APY]` | Actual length used in each `buff[i]` slot — set via [`CNET_SIZE_SET()`](../functions/CNET_SIZE_SET.md) |
| `iov` | `struct iovec[APY]` | Scatter/gather descriptor per slot, pointing at `buff[i]`/`size[i]` |
| `msg` | `struct mmsghdr[APY]` | Per-slot message header consumed directly by `sendmmsg()` in `CNET_BURST()` |

You normally never touch `iov`/`msg` directly — [`CNET_BUFFER_INIT_ALL()`](../functions/CNET_BUFFER_INIT_ALL.md) / [`CNET_BUFFER_INIT_SET()`](../functions/CNET_BUFFER_INIT_SET.md) wire them up for you from `buff`/`size` and a destination address.

## See also

- Defines: [`docs/defines/cnet_buffer.md`](../defines/cnet_buffer.md)
- [`CNET_BUFFER_INIT_ALL`](../functions/CNET_BUFFER_INIT_ALL.md), [`CNET_BUFFER_INIT_SET`](../functions/CNET_BUFFER_INIT_SET.md), [`CNET_SIZE_SET`](../functions/CNET_SIZE_SET.md), [`CNET_PACKET_UPTR`](../functions/CNET_PACKET_UPTR.md), [`CNET_BUFFER_CLONE`](../functions/CNET_BUFFER_CLONE.md), [`CNET_BURST`](../functions/CNET_BURST.md)
