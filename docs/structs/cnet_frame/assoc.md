# Structs — `cnet_frame/assoc.h`

## `struct cnet_frame_assoc_req`

```c
struct cnet_frame_assoc_req
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t info;
    uint16_t interval;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_ASSOC_REQ` |
| `info` | `uint16_t` | Capability Information bitmap |
| `interval` | `uint16_t` | Listen Interval |

## `struct cnet_frame_assoc_resp`

```c
struct cnet_frame_assoc_resp
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t info;
    uint16_t code;
    uint16_t id;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_ASSOC_RESP` |
| `info` | `uint16_t` | Capability Information bitmap |
| `code` | `uint16_t` | Status Code — one of `CNET_FRAME_STATUS_*` |
| `id` | `uint16_t` | Association ID |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
