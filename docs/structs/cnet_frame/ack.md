# Structs — `cnet_frame/ack.h`

## `struct cnet_frame_ack`

The shortest 802.11 frame — a bare Ack.

```c
struct cnet_frame_ack
{
    struct cnet_frame_type cnet_type;
    uint16_t id;
    uint8_t ra[6];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_ACK` |
| `id` | `uint16_t` | Duration/ID (usually 0 for an Ack) |
| `ra` | `uint8_t[6]` | Receiver Address |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
