# Structs — `cnet_sctp.h`

## `struct cnet_sctp`

The 12-byte SCTP common header, present at the start of every SCTP packet, followed by one or more chunks.

```c
struct cnet_sctp
{
    uint16_t sport;
    uint16_t dport;
    uint32_t vtag;
    uint32_t sum;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `sport` | `uint16_t` | Source port, big-endian |
| `dport` | `uint16_t` | Destination port, big-endian |
| `vtag` | `uint32_t` | Verification Tag for the association |
| `sum` | `uint32_t` | CRC32c checksum — fill with [`CNET_SCTP_CSUM()`](../functions/CNET_SCTP_CSUM.md) |

## Generic chunk header

### `struct cnet_sctp_chunk`

Generic 4-byte chunk header shape shared by every chunk type below (each specific chunk struct repeats these first three fields, then adds its own).

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Chunk type — one of `CNET_SCTP_*` (DATA, INIT, SACK, ...), see [defines](../defines/cnet_sctp.md) |
| `flags` | `uint8_t` | Chunk-specific flags |
| `len` | `uint16_t` | Chunk length (header + value), big-endian |

## Chunk types

### `struct cnet_sctp_data_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_DATA` |
| `flags` | `uint8_t` | `CNET_SCTP_DATA_E/B/U/I` OR'd together |
| `len` | `uint16_t` | Chunk length |
| `tsn` | `uint32_t` | Transmission Sequence Number |
| `sid` | `uint16_t` | Stream Identifier |
| `sseq` | `uint16_t` | Stream Sequence Number |
| `ppid` | `uint32_t` | Payload Protocol Identifier |

### `struct cnet_sctp_init_chunk` / `struct cnet_sctp_init_ack_chunk`

Identical layout; INIT ACK is the response to INIT.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_INIT` or `CNET_SCTP_INIT_ACK` |
| `flags` | `uint8_t` | Reserved (0) |
| `len` | `uint16_t` | Chunk length |
| `itag` | `uint32_t` | Initiate Tag |
| `arwnd` | `uint32_t` | Advertised Receiver Window Credit |
| `nos` | `uint16_t` | Number of Outbound Streams |
| `nis` | `uint16_t` | Number of Inbound Streams |
| `itsn` | `uint32_t` | Initial TSN |

### `struct cnet_sctp_sack_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_SACK` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |
| `cta` | `uint32_t` | Cumulative TSN Ack |
| `arwnd` | `uint32_t` | Advertised Receiver Window Credit |
| `nga` | `uint16_t` | Number of Gap Ack Blocks (followed by that many `struct cnet_sctp_gap_ack`) |
| `ndt` | `uint16_t` | Number of Duplicate TSNs |

### `struct cnet_sctp_gap_ack`

One entry in a SACK's Gap Ack Block list.

| Field | Type | Meaning |
|---|---|---|
| `start` | `uint16_t` | Gap Ack Block start offset |
| `end` | `uint16_t` | Gap Ack Block end offset |

### `struct cnet_sctp_hbeat_chunk` / `struct cnet_sctp_hbeat_ack_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_HBEAT` or `CNET_SCTP_HBEAT_ACK` |
| `flags` | `uint8_t` | Reserved (0) |
| `len` | `uint16_t` | Chunk length |

Followed by a Heartbeat Info parameter (`CNET_SCTP_PARAM_HBEAT_INFO`) using `struct cnet_sctp_param`.

### `struct cnet_sctp_abort_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_ABORT` |
| `flags` | `uint8_t` | `CNET_SCTP_ABORT_T` (T-bit) or 0 |
| `len` | `uint16_t` | Chunk length |

### `struct cnet_sctp_shutdown_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_SHUTDOWN` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |
| `cta` | `uint32_t` | Cumulative TSN Ack |

### `struct cnet_sctp_shutdown_ack_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_SHUTDOWN_ACK` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |

### `struct cnet_sctp_shutdown_complete_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_SHUTDOWN_COMPLETE` |
| `flags` | `uint8_t` | `CNET_SCTP_SHUTDOWN_T` (T-bit) or 0 |
| `len` | `uint16_t` | Chunk length |

### `struct cnet_sctp_error_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_ERR` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length (followed by one or more error causes) |

### `struct cnet_sctp_error_c_chunk`

One error cause entry inside an ERROR / ABORT chunk.

| Field | Type | Meaning |
|---|---|---|
| `cc` | `uint16_t` | Cause Code — one of `CNET_SCTP_ERROR_*` |
| `cl` | `uint16_t` | Cause Length |

### `struct cnet_sctp_cookie_echo_chunk` / `struct cnet_sctp_cookie_ack_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_COOKIE_ECHO` or `CNET_SCTP_COOKIE_ACK` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |

### `struct cnet_sctp_tsn_chunk`

Forward-TSN chunk (RFC 3758).

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_TSN` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |
| `nct` | `uint32_t` | New Cumulative TSN |

### `struct cnet_sctp_tsn_stream`

One stream entry following a Forward-TSN chunk.

| Field | Type | Meaning |
|---|---|---|
| `sid` | `uint16_t` | Stream Identifier |
| `sseq` | `uint16_t` | Stream Sequence |

### `struct cnet_sctp_ecne_chunk` / `struct cnet_sctp_cwr_chunk`

ECN Echo / Congestion Window Reduced chunks.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_ECN_ECNE` or `CNET_SCTP_ECN_CWR` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |
| `ltsn` | `uint32_t` | Lowest TSN observed with CE / echoed |

### `struct cnet_sctp_auth_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_AUTH` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |
| `skey` | `uint16_t` | Shared Key Identifier |
| `hmac` | `uint16_t` | HMAC Identifier |

### `struct cnet_sctp_asconf_chunk` / `struct cnet_sctp_asconf_ack_chunk`

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | `CNET_SCTP_ASCONF` or `CNET_SCTP_ASCONF_ACK` |
| `flags` | `uint8_t` | Reserved |
| `len` | `uint16_t` | Chunk length |
| `serial` | `uint32_t` | Serial Number |

### `struct cnet_sctp_asconf_param`

One address-configuration change request.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint16_t` | Parameter type — `CNET_SCTP_PARAM_ADD_IP` / `DEL_IP` / `SET_PRIMARY` |
| `len` | `uint16_t` | Parameter length |
| `id` | `uint32_t` | Address Parameter Correlation ID |

### `struct cnet_sctp_asconf_res`

Result entry in an ASCONF-ACK.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint16_t` | Parameter type — `CNET_SCTP_PARAM_SUCCESS` / `ERR_CAUSE` |
| `len` | `uint16_t` | Parameter length |
| `id` | `uint32_t` | Correlation ID this result answers |

## `struct cnet_sctp_param`

Generic TLV parameter header, used inside INIT/INIT ACK/heartbeat/etc. chunks.

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint16_t` | Parameter type — one of `CNET_SCTP_PARAM_*` |
| `len` | `uint16_t` | Parameter length (header + value), big-endian |

## See also

- Defines: [`docs/defines/cnet_sctp.md`](../defines/cnet_sctp.md)
- [`CNET_SCTP_CSUM`](../functions/CNET_SCTP_CSUM.md)
