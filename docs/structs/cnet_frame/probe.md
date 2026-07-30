# Structs — `cnet_frame/probe.h`

## `struct cnet_frame_probe_req`

```c
struct cnet_frame_probe_req
{
    struct cnet_frame_hdr cnet_hdr;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_PROBE_REQ` |

Followed by Information Elements (SSID, supported rates) with no other fixed fields.

## `struct cnet_frame_probe_resp`

```c
struct cnet_frame_probe_resp
{
    struct cnet_frame_hdr cnet_hdr;
    uint64_t tstamp;
    uint16_t interval;
    uint16_t info
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_PROBE_RESP` |
| `tstamp` | `uint64_t` | Timestamp — TSF timer value |
| `interval` | `uint16_t` | Beacon Interval |
| `info` | `uint16_t` | Capability Information bitmap |

> Known issue: the header's definition of this struct is missing the semicolon after `info`, which will fail to compile as-is (`uint16_t info` runs into the closing brace). Add `;` after `info` to fix.

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
