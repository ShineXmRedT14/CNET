# Structs — `cnet_frame/block.h`

## `struct cnet_frame_block_ackr`

Block Ack Request (BAR).

```c
struct cnet_frame_block_ackr
{
    struct cnet_frame_type cnet_type;
    uint16_t id;
    uint8_t ra[6];
    uint8_t ta[6];
    uint16_t ctl;
    uint16_t info;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_BAR` |
| `id` | `uint16_t` | Duration/ID |
| `ra` | `uint8_t[6]` | Receiver Address |
| `ta` | `uint8_t[6]` | Transmitter Address |
| `ctl` | `uint16_t` | BAR Control |
| `info` | `uint16_t` | BAR Information (starting sequence control) |

## `struct cnet_frame_block_ack`

Block Ack (BA) — same leading fields as the request, plus the acknowledgment bitmap.

```c
struct cnet_frame_block_ack
{
    struct cnet_frame_type cnet_type;
    uint16_t id;
    uint8_t ra[6];
    uint8_t ta[6];
    uint16_t ctl;
    uint16_t info;
    union
    {
        uint8_t b8it[128];
        uint16_t b16it[64];
        uint32_t b32it[32];
        uint64_t b64it[16];
    } map;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_BA` |
| `id` | `uint16_t` | Duration/ID |
| `ra` | `uint8_t[6]` | Receiver Address |
| `ta` | `uint8_t[6]` | Transmitter Address |
| `ctl` | `uint16_t` | BA Control |
| `info` | `uint16_t` | BA Information (starting sequence control) |
| `map` | union | Block Ack bitmap — 128 bytes / 1024 bits, viewed as `uint8_t[128]`, `uint16_t[64]`, `uint32_t[32]`, or `uint64_t[16]` via `CNET_FRAME_BLOCK_MACROS_BIT8`/`16`/`32`/`64` |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
- Defines: [`docs/defines/cnet_frame/block.md`](../../defines/cnet_frame/block.md)
