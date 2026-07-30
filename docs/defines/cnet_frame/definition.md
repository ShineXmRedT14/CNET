# Defines — `cnet_frame/definition.h`

Status codes, auth algorithms, deauth/disassoc reason codes, action categories, and the field-access macros shared by every `cnet_frame/*` header.

## Status codes (`CNET_FRAME_STATUS_*`)

Used in response frames (association/reassociation response, etc.) to report success or a specific failure reason.

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_STATUS_S` | `0x0000` | Success |
| `CNET_FRAME_STATUS_UF` | `0x0001` | Unspecified failure |
| `CNET_FRAME_STATUS_CU` | `0x000A` | Cannot support all requested capabilities |
| `CNET_FRAME_STATUS_REANA` | `0x000B` | Reassociation denied — unable to confirm association exists |
| `CNET_FRAME_STATUS_ADU` | `0x000C` | Association denied — reason outside 802.11 scope |
| `CNET_FRAME_STATUS_AAD` | `0x000D` | Authentication algorithm not supported |
| `CNET_FRAME_STATUS_ASI` | `0x000E` | Authentication sequence number out of expected sequence |
| `CNET_FRAME_STATUS_CF` | `0x000F` | Authentication rejected — challenge failure |
| `CNET_FRAME_STATUS_AT` | `0x0010` | Authentication rejected — timeout waiting for next frame |
| `CNET_FRAME_STATUS_AUHSA` | `0x0011` | Association denied — AP unable to handle additional associated stations |
| `CNET_FRAME_STATUS_ADR` | `0x0012` | Association denied — data rate not supported |
| `CNET_FRAME_STATUS_ADSP` | `0x0013` | Association denied — Short Preamble not supported |
| `CNET_FRAME_STATUS_ADP` | `0x0014` | Association denied — PBCC not supported |
| `CNET_FRAME_STATUS_ADCA` | `0x0015` | Association denied — Channel Agility not supported |
| `CNET_FRAME_STATUS_ADSM` | `0x0016` | Association denied — Spectrum Management required |
| `CNET_FRAME_STATUS_ADBPC` | `0x0017` | Association denied — unacceptable power capability |
| `CNET_FRAME_STATUS_ADBC` | `0x0018` | Association denied — unacceptable supported channels |
| `CNET_FRAME_STATUS_ADSST` | `0x0019` | Association denied — Short Slot Time not supported |
| `CNET_FRAME_STATUS_ADDO` | `0x001A` | Association denied — DSSS-OFDM not supported |
| `CNET_FRAME_STATUS_II` | `0x0028` | Invalid Information Element |
| `CNET_FRAME_STATUS_IGC` | `0x0029` | Invalid Group Cipher |
| `CNET_FRAME_STATUS_IPC` | `0x002A` | Invalid Pairwise Cipher |
| `CNET_FRAME_STATUS_IA` | `0x002B` | Invalid AKMP |
| `CNET_FRAME_STATUS_URV` | `0x002C` | Unsupported RSN version |
| `CNET_FRAME_STATUS_IRC` | `0x002D` | Invalid RSN IE capabilities |
| `CNET_FRAME_STATUS_CR` | `0x002E` | Cipher suite rejected per security policy |

## Authentication algorithms (`CNET_FRAME_AUTH_*`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_AUTH_OPEN` | `0x0000` | Open System |
| `CNET_FRAME_AUTH_WEP` | `0x0001` | Shared Key (WEP) |
| `CNET_FRAME_AUTH_FT` | `0x0002` | Fast BSS Transition |
| `CNET_FRAME_AUTH_WPA3` | `0x0003` | SAE (WPA3) |

## Reason codes (`CNET_FRAME_REASON_*`)

Carried in Deauthentication / Disassociation frames.

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_REASON_UNSPECIFIED` | `0x0001` | Unspecified reason |
| `CNET_FRAME_REASON_ANLV` | `0x0002` | Previous authentication no longer valid |
| `CNET_FRAME_REASON_DL` | `0x0003` | Deauthenticated — leaving/has left |
| `CNET_FRAME_REASON_DI` | `0x0004` | Disassociated due to inactivity |
| `CNET_FRAME_REASON_DAB` | `0x0005` | Disassociated — AP unable to handle all stations |
| `CNET_FRAME_REASON_C2FN` | `0x0006` | Class 2 frame from nonauthenticated station |
| `CNET_FRAME_REASON_C3FN` | `0x0007` | Class 3 frame from nonassociated station |
| `CNET_FRAME_REASON_DSL` | `0x0008` | Disassociated — station leaving/has left |
| `CNET_FRAME_REASON_SNA` | `0x0009` | Station requesting (re)association not authenticated |
| `CNET_FRAME_REASON_BPC` | `0x000A` | Disassociated — unacceptable power capability |
| `CNET_FRAME_REASON_BSC` | `0x000B` | Disassociated — unacceptable supported channels |
| `CNET_FRAME_REASON_II` | `0x000D` | Invalid Information Element |
| `CNET_FRAME_REASON_MF` | `0x000E` | Michael MIC failure |
| `CNET_FRAME_REASON_4WHT` | `0x000F` | 4-Way Handshake timeout |
| `CNET_FRAME_REASON_GKT` | `0x0010` | Group Key Handshake timeout |
| `CNET_FRAME_REASON_ID` | `0x0011` | 4-Way Handshake IE differs from (Re)Association Request/Probe Response |
| `CNET_FRAME_REASON_IGC` | `0x0012` | Invalid Group Cipher |
| `CNET_FRAME_REASON_IPC` | `0x0013` | Invalid Pairwise Cipher |
| `CNET_FRAME_REASON_IA` | `0x0014` | Invalid AKMP |
| `CNET_FRAME_REASON_URV` | `0x0015` | Unsupported RSN version |
| `CNET_FRAME_REASON_IRC` | `0x0016` | Invalid RSN IE capabilities |
| `CNET_FRAME_REASON_8021XAF` | `0x0017` | 802.1X authentication failed |
| `CNET_FRAME_REASON_CSR` | `0x0018` | Cipher suite rejected per security policy |

## Action categories (`CNET_FRAME_ACTION_*`, for `struct cnet_frame_action.category`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_FRAME_ACTION_CSM` | `0x00` | Spectrum Management |
| `CNET_FRAME_ACTION_CQ` | `0x01` | QoS |
| `CNET_FRAME_ACTION_CD` | `0x02` | DLS |
| `CNET_FRAME_ACTION_CBA` | `0x03` | Block Ack |
| `CNET_FRAME_ACTION_CP` | `0x04` | Public |
| `CNET_FRAME_ACTION_CRM` | `0x05` | Radio Measurement |
| `CNET_FRAME_ACTION_CFBT` | `0x06` | Fast BSS Transition |
| `CNET_FRAME_ACTION_CH` | `0x07` | HT |
| `CNET_FRAME_ACTION_CSQ` | `0x08` | SA Query |
| `CNET_FRAME_ACTION_CPD` | `0x09` | Protected Dual of Public Action |
| `CNET_FRAME_ACTION_CW` | `0x0A` | WNM |
| `CNET_FRAME_ACTION_CM` | `0x0D` | Mesh |
| `CNET_FRAME_ACTION_CMH` | `0x0E` | Multihop |
| `CNET_FRAME_ACTION_CSP` | `0x0F` | Self-protected |
| `CNET_FRAME_ACTION_CV` | `0x15` | VHT |
| `CNET_FRAME_ACTION_CVS` | `0x7F` | Vendor Specific |

## Field-access macros

Shortcuts into the nested `cnet_hdr` / `cnet_type` fields shared by every frame struct.

| Define | Expands to | Use |
|---|---|---|
| `CNET_FRAME_MACROS_ID` | `cnet_hdr.id` | Duration/ID field |
| `CNET_FRAME_MACROS_ADDR1` | `cnet_hdr.addr1` | Address 1 (receiver) |
| `CNET_FRAME_MACROS_ADDR2` | `cnet_hdr.addr2` | Address 2 (transmitter) |
| `CNET_FRAME_MACROS_ADDR3` | `cnet_hdr.addr3` | Address 3 (BSSID / source or destination, context-dependent) |
| `CNET_FRAME_MACROS_SEQ` | `cnet_hdr.seq` | Sequence control field |
| `CNET_FRAME_MACROS_VERSION` | `cnet_hdr.cnet_type.version` | Protocol version bits, via a struct that embeds `cnet_hdr` |
| `CNET_FRAME_MACROS_TYPE` | `cnet_hdr.cnet_type.type` | Frame type bits, via `cnet_hdr` |
| `CNET_FRAME_MACROS_STYPE` | `cnet_hdr.cnet_type.stype` | Frame subtype bits, via `cnet_hdr` |
| `CNET_FRAME_MACROS_FLAGS` | `cnet_hdr.cnet_type.flags` | Frame control flags, via `cnet_hdr` |
| `CNET_FRAME_MACROS_RVERSION` | `cnet_type.version` | Protocol version bits, for structs that embed `cnet_type` directly (no `cnet_hdr`, e.g. control frames) |
| `CNET_FRAME_MACROS_RTYPE` | `cnet_type.type` | Frame type bits, direct-`cnet_type` variant |
| `CNET_FRAME_MACROS_RSTYPE` | `cnet_type.stype` | Frame subtype bits, direct-`cnet_type` variant |
| `CNET_FRAME_MACROS_RFLAGS` | `cnet_type.flags` | Frame control flags, direct-`cnet_type` variant |

> The `R`-prefixed macros exist because several control frames (RTS, CTS, ACK, Block Ack, Trigger, PS-Poll, CF, Control Wrapper) embed `struct cnet_frame_type` directly instead of going through a full `struct cnet_frame_hdr` — see [`docs/structs/`](../../structs/) for which is which.

## See also

- [`cnet_frame/hdr.h` defines](hdr.md) — frame type/subtype/flags values these macros expose
