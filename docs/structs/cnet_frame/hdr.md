# Structs — `cnet_frame/hdr.h`

Shared by (almost) every 802.11 frame struct in `cnet_frame/*`.

## `struct cnet_frame_type`

The 2-byte Frame Control field's type/subtype/flags — either embedded inside `struct cnet_frame_hdr` (management & most data frames), or embedded directly in short control frames that skip the full 24-byte header (RTS/CTS/ACK/Block Ack/Trigger/PS-Poll/CF/Control Wrapper).

```c
struct cnet_frame_type
{
    uint16_t version:2;
    uint16_t type:2;
    uint16_t stype:4;
    uint16_t flags:8;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `version` | `2 bits` | Protocol version — `CNET_FRAME_VERSION` (0) |
| `type` | `2 bits` | `CNET_FRAME_TYPE_MGT` / `CTL` / `DATA` |
| `stype` | `4 bits` | Subtype — meaning depends on `type`, see `CNET_FRAME_STYPE_*` |
| `flags` | `8 bits` | `CNET_FRAME_FLAG_*` OR'd together (To-DS, From-DS, Retry, Protected, ...) |

Field order flips between little- and big-endian builds to keep the on-wire bit layout correct.

## `struct cnet_frame_hdr`

The generic 24-byte 802.11 MAC header used by management frames and most data frames.

```c
struct cnet_frame_hdr
{
    struct cnet_frame_type cnet_type;
    uint16_t id;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_type` | `struct cnet_frame_type` | Frame Control field — access sub-fields via `CNET_FRAME_MACROS_VERSION`/`TYPE`/`STYPE`/`FLAGS` |
| `id` | `uint16_t` | Duration/ID |
| `addr1` | `uint8_t[6]` | Receiver Address |
| `addr2` | `uint8_t[6]` | Transmitter Address |
| `addr3` | `uint8_t[6]` | BSSID, source, or destination address (meaning depends on the To-DS/From-DS flags) |
| `seq` | `uint16_t` | Sequence Control (fragment number + sequence number) |

## See also

- Defines: [`docs/defines/cnet_frame/hdr.md`](../../defines/cnet_frame/hdr.md), [`docs/defines/cnet_frame/definition.md`](../../defines/cnet_frame/definition.md)
- Which struct embeds `cnet_frame_hdr` vs. `cnet_frame_type` directly: see each frame's own page below, and [`docs/headers.md`](../../headers.md)
