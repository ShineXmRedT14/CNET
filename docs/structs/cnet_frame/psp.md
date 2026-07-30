# Structs — `cnet_frame/psp.h`

## `struct cnet_frame_psp`

PS-Poll (Power-Save Poll) — a station tells its AP it's awake and ready to receive one buffered frame.

```c
struct cnet_frame_psp
{
    struct cnet_frame_type cnet_type;
    uint16_t aid;
    uint8_t bssid[6];
    uint8_t ta[6];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_PSP` |
| `aid` | `uint16_t` | Association ID of the polling station |
| `bssid` | `uint8_t[6]` | BSSID (receiver — the AP) |
| `ta` | `uint8_t[6]` | Transmitter Address (the polling station) |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
