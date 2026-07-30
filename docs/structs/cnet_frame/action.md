# Structs — `cnet_frame/action.h`

## `struct cnet_frame_action` / `struct cnet_frame_action_na`

Identical layout; `action_na` is the "No Ack" variant (`stype = CNET_FRAME_STYPE_ACTION_NA`) that doesn't get an immediate Ack.

```c
struct cnet_frame_action
{
    struct cnet_frame_hdr cnet_hdr;
    uint8_t category;
    uint8_t action;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_MGT`, `stype = CNET_FRAME_STYPE_ACTION` (or `_ACTION_NA` for `cnet_frame_action_na`) |
| `category` | `uint8_t` | Action category — one of `CNET_FRAME_ACTION_*` |
| `action` | `uint8_t` | Category-specific action code |

Followed by category-specific fields (not modeled — varies per category).

> Known issue: this header's include guard is misspelled `#ifnder CNET_FRAME_ACTION_H` instead of `#ifndef`, so the guard never activates. Fix by changing `ifnder` to `ifndef`.

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
- Defines: [`docs/defines/cnet_frame/definition.md`](../../defines/cnet_frame/definition.md)
