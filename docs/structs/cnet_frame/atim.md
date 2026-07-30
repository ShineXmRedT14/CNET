# Structs — `cnet_frame/atim.h`

## `struct cnet_frame_atim`

```c
struct cnet_frame_atim
{
    struct cnet_frame_hdr cnet_hdr;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_ATIM` |

The ATIM (Announcement Traffic Indication Message) frame carries no body beyond the standard header.

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
