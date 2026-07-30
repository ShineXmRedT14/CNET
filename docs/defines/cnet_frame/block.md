# Defines — `cnet_frame/block.h`

Field-access macros for the bitmap in `struct cnet_frame_block_ack`.

| Define | Expands to | Use |
|---|---|---|
| `CNET_FRAME_BLOCK_MACROS_BIT8` | `map.b8it` | Bitmap viewed as `uint8_t[128]` |
| `CNET_FRAME_BLOCK_MACROS_BIT16` | `map.b16it` | Bitmap viewed as `uint16_t[64]` |
| `CNET_FRAME_BLOCK_MACROS_BIT32` | `map.b32it` | Bitmap viewed as `uint32_t[32]` |
| `CNET_FRAME_BLOCK_MACROS_BIT64` | `map.b64it` | Bitmap viewed as `uint64_t[16]` |

## See also

- Struct reference: [`docs/structs/cnet_frame_block.md`](../../structs/cnet_frame/block.md)
