# Defines — `cnet_buffer.h`

| Define | Value | Meaning |
|---|---|---|
| `APY` | `64` | Number of packet slots in a `struct cnet_buffer` — the max packets `CNET_BURST()` can send in one call |
| `APX` | `2048` | Size in bytes of each packet slot (`buff[APY][APX]`) — the max size of a single buffered packet |

Both are compile-time constants; there is no runtime way to resize a `struct cnet_buffer` beyond these limits.

## See also

- Struct reference: [`docs/structs/cnet_buffer.md`](../structs/cnet_buffer.md)
- [`CNET_BUFFER_INIT_ALL`](../functions/CNET_BUFFER_INIT_ALL.md), [`CNET_BUFFER_INIT_SET`](../functions/CNET_BUFFER_INIT_SET.md), [`CNET_BURST`](../functions/CNET_BURST.md)
