# Defines — `cnet_frame/hdr.h`

Shared 802.11 frame-header constants: the type/subtype/flags found in every management, control, and data frame (`struct cnet_frame_type`, embedded in `struct cnet_frame_hdr`).

## Frame type (for `cnet_type.type`, 2 bits)

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_TYPE_MGT` | `0x0` | Management frame |
| `CNET_FRAME_TYPE_CTL` | `0x1` | Control frame |
| `CNET_FRAME_TYPE_DATA` | `0x2` | Data frame |

## Version

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_VERSION` | `0x0` | Protocol version — always 0 for current 802.11 |

## Management subtypes (for `cnet_type.stype` when `type == CNET_FRAME_TYPE_MGT`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_STYPE_ASSOC_REQ` | `0x0` | Association Request |
| `CNET_FRAME_STYPE_ASSOC_RESP` | `0x1` | Association Response |
| `CNET_FRAME_STYPE_REASSOC_REQ` | `0x2` | Reassociation Request |
| `CNET_FRAME_STYPE_REASSOC_RESP` | `0x3` | Reassociation Response |
| `CNET_FRAME_STYPE_PROBE_REQ` | `0x4` | Probe Request |
| `CNET_FRAME_STYPE_PROBE_RESP` | `0x5` | Probe Response |
| `CNET_FRAME_STYPE_ADV` | `0x6` | Advertisement (public action) |
| `CNET_FRAME_STYPE_RES7` | `0x7` | Reserved |
| `CNET_FRAME_STYPE_BEACON` | `0x8` | Beacon |
| `CNET_FRAME_STYPE_ATIM` | `0x9` | ATIM |
| `CNET_FRAME_STYPE_DIASSOC` | `0xA` | Disassociation |
| `CNET_FRAME_STYPE_AUTH` | `0xB` | Authentication |
| `CNET_FRAME_STYPE_DEAUTH` | `0xC` | Deauthentication |
| `CNET_FRAME_STYPE_ACTION` | `0xD` | Action |
| `CNET_FRAME_STYPE_ACTION_NA` | `0xE` | Action, No Ack |
| `CNET_FRAME_STYPE_RES15` | `0xF` | Reserved |

## Control subtypes (for `cnet_type.stype` when `type == CNET_FRAME_TYPE_CTL`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_STYPE_TRIGGER` | `0x2` | Trigger |
| `CNET_FRAME_STYPE_BFRPT` | `0x4` | Beamforming Report Poll |
| `CNET_FRAME_STYPE_VNA` | `0x5` | VHT/HE NDP Announcement |
| `CNET_FRAME_STYPE_CTLE` | `0x6` | Control Frame Extension |
| `CNET_FRAME_STYPE_CTLW` | `0x7` | Control Wrapper |
| `CNET_FRAME_STYPE_BAR` | `0x8` | Block Ack Request |
| `CNET_FRAME_STYPE_BA` | `0x9` | Block Ack |
| `CNET_FRAME_STYPE_PSP` | `0xA` | PS-Poll |
| `CNET_FRAME_STYPE_RTS` | `0xB` | Request To Send |
| `CNET_FRAME_STYPE_CTS` | `0xC` | Clear To Send |
| `CNET_FRAME_STYPE_ACK` | `0xD` | Acknowledgement |
| `CNET_FRAME_STYPE_CFE` | `0xE` | CF-End |
| `CNET_FRAME_STYPE_CFEA` | `0xF` | CF-End + CF-Ack |

## Data subtypes (for `cnet_type.stype` when `type == CNET_FRAME_TYPE_DATA`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_STYPE_DATA` | `0x0` | Data |
| `CNET_FRAME_STYPE_DATA_CFA` | `0x1` | Data + CF-Ack |
| `CNET_FRAME_STYPE_DATA_CFP` | `0x2` | Data + CF-Poll |
| `CNET_FRAME_STYPE_DATA_CFAP` | `0x3` | Data + CF-Ack + CF-Poll |
| `CNET_FRAME_STYPE_NF` | `0x4` | Null (no data) |
| `CNET_FRAME_STYPE_CF` / `CFA` | `0x5` | CF-Ack (no data) |
| `CNET_FRAME_STYPE_CFP` | `0x6` | CF-Poll (no data) |
| `CNET_FRAME_STYPE_CFAP` | `0x7` | CF-Ack + CF-Poll (no data) |
| `CNET_FRAME_STYPE_QOSD` | `0x8` | QoS Data |
| `CNET_FRAME_STYPE_QOSD_CFA` | `0x9` | QoS Data + CF-Ack |
| `CNET_FRAME_STYPE_QOSD_CFP` | `0xA` | QoS Data + CF-Poll |
| `CNET_FRAME_STYPE_QOSD_CFAP` | `0xB` | QoS Data + CF-Ack + CF-Poll |
| `CNET_FRAME_STYPE_QOS_NF` | `0xC` | QoS Null (no data) |
| `CNET_FRAME_STYPE_QOS_CFA` | `0xD` | QoS CF-Ack (no data) |
| `CNET_FRAME_STYPE_QOS_CFP` | `0xE` | QoS CF-Poll (no data) |
| `CNET_FRAME_STYPE_QOS_CFAP` | `0xF` | QoS CF-Ack + CF-Poll (no data) |

> Note: `CNET_FRAME_STYPE_CF` and `CNET_FRAME_STYPE_CFA` share the value `0x5` in the header — pick whichever name reads better at the call site, they're identical.

## Frame control flags (for `cnet_type.flags`, 8 bits)

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_FLAG_TO_DS` | `0x01` | To Distribution System |
| `CNET_FRAME_FLAG_FROM_DS` | `0x02` | From Distribution System |
| `CNET_FRAME_FLAG_MORE_FRAG` | `0x04` | More Fragments follow |
| `CNET_FRAME_FLAG_RETRY` | `0x08` | Retransmission |
| `CNET_FRAME_FLAG_PWR` | `0x10` | Power Management |
| `CNET_FRAME_FLAG_MORE_DATA` | `0x20` | More Data buffered at the AP |
| `CNET_FRAME_FLAG_PROTECT` | `0x40` | Protected Frame (encrypted) |
| `CNET_FRAME_FLAG_ORDER` | `0x80` | Strictly Ordered |

## See also

- Struct reference: [`docs/structs/cnet_frame_hdr.md`](../../structs/cnet_frame/hdr.md)
- [`cnet_frame/definition.h` defines](definition.md) — status/reason/action codes and field-access macros built on top of this header
