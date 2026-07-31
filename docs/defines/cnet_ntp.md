# Defines — `cnet_ntp.h`

## NTP Version (for `struct cnet_ntp.vn`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_NTP_VERSION` | `0x4` | NTP version 4 |

## Stratum (for `struct cnet_ntp.strat`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_NTP_STRAT_KOD` | `0x00` | Kiss-o'-Death / invalid stratum |
| `CNET_NTP_STRAT_PRIMARY` | `0x01` | Primary reference clock |

## Leap Indicator (for `struct cnet_ntp.li`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_NTP_LI_NONE` | `0x0` | No warning |
| `CNET_NTP_LI_LM61` | `0x1` | Last minute has 61 seconds |
| `CNET_NTP_LI_LM59` | `0x2` | Last minute has 59 seconds |
| `CNET_NTP_LI_ALARM` | `0x3` | Alarm condition / clock unsynchronized |

## Mode (for `struct cnet_ntp.md`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_NTP_MD_RES` | `0x0` | Reserved |
| `CNET_NTP_MD_SACT` | `0x1` | Symmetric active |
| `CNET_NTP_MD_SPAS` | `0x2` | Symmetric passive |
| `CNET_NTP_MD_CLIENT` | `0x3` | Client |
| `CNET_NTP_MD_SERVER` | `0x4` | Server |
| `CNET_NTP_MD_BCAST` | `0x5` | Broadcast |
| `CNET_NTP_MD_CNTRL` | `0x6` | NTP control message |
| `CNET_NTP_MD_PRIVATE` | `0x7` | Reserved for private use |

## See also

- Struct reference: [`docs/structs/cnet_ntp.md`](../structs/cnet_ntp.md)