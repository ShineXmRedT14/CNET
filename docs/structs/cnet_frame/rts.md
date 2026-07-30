# Structs — `cnet_frame/rts.h`

## `struct cnet_frame_rts`

Request To Send.

```c
struct cnet_frame_rts
{
    struct cnet_frame_types cnet_type;
    uint16_t id;
    uint8_t ra[6];
    uint8_t ta[6];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_RTS` |
| `id` | `uint16_t` | Duration/ID |
| `ra` | `uint8_t[6]` | Receiver Address |
| `ta` | `uint8_t[6]` | Transmitter Address |

> Known issue: the header declares the first member as `struct cnet_frame_types cnet_type` (extra trailing `s`) — no such type exists, only `struct cnet_frame_type` (defined in `hdr.h`). This is a compile error as written; fix by removing the `s`.

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
