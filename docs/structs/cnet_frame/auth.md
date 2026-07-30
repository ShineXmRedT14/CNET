# Structs — `cnet_frame/auth.h`

## `struct cnet_frame_auth`

```c
struct cnet_frame_auth
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t auth;
    uint16_t seq;
    uint16_t code;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_AUTH` |
| `auth` | `uint16_t` | Authentication Algorithm Number — one of `CNET_FRAME_AUTH_*` |
| `seq` | `uint16_t` | Authentication Transaction Sequence Number |
| `code` | `uint16_t` | Status Code — one of `CNET_FRAME_STATUS_*` |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
- Defines: [`docs/defines/cnet_frame/definition.md`](../../defines/cnet_frame/definition.md)
