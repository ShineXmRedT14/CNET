# Structs — `cnet_frame/adv.h`

## `struct cnet_frame_adv`

Public Action / Advertisement Protocol frame.

```c
struct cnet_frame_adv
{
    struct cnet_frame_hdr cnet_hdr;
    uint64_t tstamp;
    uint16_t info;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_ADV` |
| `tstamp` | `uint64_t` | Timestamp |
| `info` | `uint16_t` | Capability/advertisement info bitmap |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
