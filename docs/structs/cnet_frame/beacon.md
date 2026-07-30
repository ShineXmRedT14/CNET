# Structs — `cnet_frame/beacon.h`

## `struct cnet_frame_beacon`

```c
struct cnet_frame_beacon
{
    struct cnet_frame_hdr cnet_hdr;
    uint64_t tstamp;
    uint16_t bint;
    uint16_t cinfo;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | Standard 802.11 header — `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_BEACON` |
| `tstamp` | `uint64_t` | Timestamp — AP's TSF timer value |
| `bint` | `uint16_t` | Beacon Interval, in Time Units (1.024 ms) |
| `cinfo` | `uint16_t` | Capability Information bitmap |

Followed by a variable-length list of Information Elements (SSID, supported rates, etc.) — not modeled as a struct since their layout is TLV and count varies.

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
