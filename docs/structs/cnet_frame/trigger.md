# Structs — `cnet_frame/trigger.h`

## `struct cnet_frame_trigger`

A control frame — embeds `struct cnet_frame_type` directly rather than the full `struct cnet_frame_hdr` (there's no Address 3 / Sequence Control in most control frames).

```c
struct cnet_frame_trigger
{
    struct cnet_frame_type cnet_type;
    uint16_t id;
    uint8_t ra[6];
    uint8_t ta[6];
    uint64_t info;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | `type = CNET_FRAME_TYPE_CTL`, `stype = CNET_FRAME_STYPE_TRIGGER` — access via `CNET_FRAME_MACROS_RTYPE`/`RSTYPE`/etc. |
| `id` | `uint16_t` | Duration/ID |
| `ra` | `uint8_t[6]` | Receiver Address (AP) |
| `ta` | `uint8_t[6]` | Transmitter Address |
| `info` | `uint64_t` | Common Info field (HE/EHT trigger parameters) |

Followed by one or more per-user Info fields — not modeled, length depends on the number of solicited stations.

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md) — for the `R`-prefixed field macros used with direct `cnet_type` structs
