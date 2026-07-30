# Structs — `cnet_frame/reassoc.h`

## `struct cnet_frame_reassoc_req`

```c
struct cnet_frame_reassoc_req
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t info;
    uint16_t interval;
    uint8_t ap[6];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_REASSOC_REQ` |
| `info` | `uint16_t` | Capability Information bitmap |
| `interval` | `uint16_t` | Listen Interval |
| `ap` | `uint8_t[6]` | Current AP Address (the AP the station is roaming from) |

## `struct cnet_frame_reassoc_resp`

```c
struct cnet_frame_reassoc_resp
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t info;
    uint16_t code;
    uint16_t id;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_REASSOC_RESP` |
| `info` | `uint16_t` | Capability Information bitmap |
| `code` | `uint16_t` | Status Code — one of `CNET_FRAME_STATUS_*` |
| `id` | `uint16_t` | Association ID |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
