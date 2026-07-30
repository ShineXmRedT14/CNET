# Structs — `cnet_frame/deauth.h`

## `struct cnet_frame_deauth`

```c
struct cnet_frame_deauth
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t code;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_DEAUTH` |
| `code` | `uint16_t` | Reason Code — one of `CNET_FRAME_REASON_*` |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
- Defines: [`docs/defines/cnet_frame/definition.md`](../../defines/cnet_frame/definition.md)
