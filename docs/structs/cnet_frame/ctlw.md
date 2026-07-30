# Structs — `cnet_frame/ctlw.h`

## `struct cnet_frame_ctlw`

Control Wrapper — carries an encapsulated (HT/VHT-control-protected) control frame.

```c
struct cnet_frame_ctlw
{
    struct cnet_frame_type cnet_type;
    uint16_t id;
    uint8_t ra[6];
    uint16_t frame;
    uint8_t ht[4];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_CTLW` |
| `id` | `uint16_t` | Duration/ID |
| `ra` | `uint8_t[6]` | Receiver Address |
| `frame` | `uint16_t` | Carried Frame Control (the wrapped frame's own type/subtype/flags) |
| `ht` | `uint8_t[4]` | HT Control field |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
