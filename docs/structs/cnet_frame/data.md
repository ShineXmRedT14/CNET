# Structs — `cnet_frame/data.h`

Data-frame variants, from the plain 24-byte-header case up to QoS + 4-address (WDS) + HT Control.

## `struct cnet_frame_data`

Plain data frame — header only, payload (MSDU) follows.

```c
struct cnet_frame_data
{
    struct cnet_frame_hdr cnet_hdr;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_DATA`, `stype = CNET_FRAME_STYPE_DATA` (or one of the CF-Ack/CF-Poll variants) |

## `struct cnet_frame_null`

Null Data frame — header only, no payload at all (used for power-save state signaling).

```c
struct cnet_frame_null
{
    struct cnet_frame_hdr cnet_hdr;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `stype = CNET_FRAME_STYPE_NF` |

## `struct cnet_frame_data_wds`

Four-address data frame, used in Wireless Distribution System (AP-to-AP) links.

```c
struct cnet_frame_data_wds
{
    struct cnet_frame_hdr cnet_hdr;
    uint8_t addr4[6];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `type = CNET_FRAME_TYPE_DATA`, both To-DS and From-DS flags set |
| `addr4` | `uint8_t[6]` | Address 4 — the original source (or final destination) in a WDS link |

## `struct cnet_frame_qos_data`

Data frame with a QoS Control field.

```c
struct cnet_frame_qos_data
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t qos;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `stype` one of `CNET_FRAME_STYPE_QOSD*` |
| `qos` | `uint16_t` | QoS Control (TID, EOSP, Ack Policy, ...) |

## `struct cnet_frame_qos_data_wds`

QoS data + four-address (WDS).

```c
struct cnet_frame_qos_data_wds
{
    struct cnet_frame_hdr cnet_hdr;
    uint8_t addr4[6];
    uint16_t qos;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | QoS data, To-DS + From-DS set |
| `addr4` | `uint8_t[6]` | Address 4 |
| `qos` | `uint16_t` | QoS Control |

## `struct cnet_frame_qos_data_ht`

QoS data with an HT Control field (802.11n+).

```c
struct cnet_frame_qos_data_ht
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t qos;
    uint32_t ht;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | QoS data, `+Order` flag set |
| `qos` | `uint16_t` | QoS Control |
| `ht` | `uint32_t` | HT Control field |

## `struct cnet_frame_qos_data_merge`

The full combination: QoS + four-address + HT Control.

```c
struct cnet_frame_qos_data_merge
{
    struct cnet_frame_hdr cnet_hdr;
    uint8_t addr4[6];
    uint16_t qos;
    uint32_t ht;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | QoS data, To-DS + From-DS + Order all set |
| `addr4` | `uint8_t[6]` | Address 4 |
| `qos` | `uint16_t` | QoS Control |
| `ht` | `uint32_t` | HT Control field |

## `struct cnet_frame_qos_null`

QoS Null — QoS Control field but no payload.

```c
struct cnet_frame_qos_null
{
    struct cnet_frame_hdr cnet_hdr;
    uint16_t qos;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `cnet_hdr` | `struct cnet_frame_hdr` | `stype` one of `CNET_FRAME_STYPE_QOS_NF`/`CFA`/`CFP`/`CFAP` |
| `qos` | `uint16_t` | QoS Control |

## See also

- [`docs/structs/cnet_frame/hdr.md`](hdr.md)
