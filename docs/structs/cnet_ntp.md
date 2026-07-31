# Structs — `cnet_ntp.h`

## `struct cnet_ntp_short`

Short NTP fixed-point value containing seconds and fractional parts.

```c
struct cnet_ntp_short
{
    uint16_t sec;
    uint16_t frc;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `sec` | `uint16_t` | Seconds part |
| `frc` | `uint16_t` | Fractional part |

## `struct cnet_ntp_timestamp`

NTP timestamp containing a 32-bit seconds field and a 32-bit fractional field.

```c
struct cnet_ntp_timestamp
{
    uint32_t sec;
    uint32_t frc;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `sec` | `uint32_t` | Seconds since the NTP epoch |
| `frc` | `uint32_t` | Fractional seconds |

## `struct cnet_ntp`

The NTP packet header.

```c
struct cnet_ntp
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t li:2;
    uint8_t vn:3;
    uint8_t md:3;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t md:3;
    uint8_t vn:3;
    uint8_t li:2;
#else
#error "Big/Little Endians bytes-error"
#endif
    uint8_t strat;
    uint8_t poll;
    uint8_t prec;

    struct cnet_ntp_short delay;
    struct cnet_ntp_short disper;

    uint32_t ref_id;

    struct cnet_ntp_timestamp ref_tm;
    struct cnet_ntp_timestamp org_tm;
    struct cnet_ntp_timestamp recv_tm;
    struct cnet_ntp_timestamp trsm_tm;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `li` | `uint8_t:2` | Leap Indicator — one of `CNET_NTP_LI_*` |
| `vn` | `uint8_t:3` | NTP version — `CNET_NTP_VERSION` |
| `md` | `uint8_t:3` | NTP mode — one of `CNET_NTP_MD_*` |
| `strat` | `uint8_t` | Stratum |
| `poll` | `uint8_t` | Poll interval |
| `prec` | `uint8_t` | Clock precision |
| `delay` | `struct cnet_ntp_short` | Root delay |
| `disper` | `struct cnet_ntp_short` | Root dispersion |
| `ref_id` | `uint32_t` | Reference clock identifier |
| `ref_tm` | `struct cnet_ntp_timestamp` | Reference timestamp |
| `org_tm` | `struct cnet_ntp_timestamp` | Originate timestamp |
| `recv_tm` | `struct cnet_ntp_timestamp` | Receive timestamp |
| `trsm_tm` | `struct cnet_ntp_timestamp` | Transmit timestamp |

## See also

- Defines: [`docs/defines/cnet_ntp.md`](../defines/cnet_ntp.md)