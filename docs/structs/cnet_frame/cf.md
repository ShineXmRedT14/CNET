# Structs — `cnet_frame/cf.h`

## `struct cnet_frame_cf` / `struct cnet_frame_cf_ack`

Identical layout; `cf_ack` is the CF-End + CF-Ack variant.

```c
struct cnet_frame_cf
{
    struct cnet_frame_type cnet_type;
    uint16_t id;
    uint8_t ra[6];
    uint8_t bssid[6];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_CFE` for `cnet_frame_cf`, `CNET_FRAME_STYPE_CFEA` for `cnet_frame_cf_ack` |
| `id` | `uint16_t` | Duration/ID |
| `ra` | `uint8_t[6]` | Receiver Address |
| `bssid` | `uint8_t[6]` | BSSID |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
