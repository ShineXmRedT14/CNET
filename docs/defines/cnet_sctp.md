# Defines — `cnet_sctp.h`

## Chunk types (for `struct cnet_sctp_chunk.type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_SCTP_DATA` | `0x00` | DATA |
| `CNET_SCTP_INIT` | `0x01` | INIT |
| `CNET_SCTP_INIT_ACK` | `0x02` | INIT ACK |
| `CNET_SCTP_SACK` | `0x03` | Selective ACK |
| `CNET_SCTP_HBEAT` | `0x04` | Heartbeat request |
| `CNET_SCTP_HBEAT_ACK` | `0x05` | Heartbeat ACK |
| `CNET_SCTP_ABORT` | `0x06` | Abort association |
| `CNET_SCTP_SHUTDOWN` | `0x07` | Shutdown |
| `CNET_SCTP_SHUTDOWN_ACK` | `0x08` | Shutdown ACK |
| `CNET_SCTP_ERR` | `0x09` | Operation error |
| `CNET_SCTP_COOKIE_ECHO` | `0x0A` | State cookie echo |
| `CNET_SCTP_COOKIE_ACK` | `0x0B` | Cookie ACK |
| `CNET_SCTP_ECN_ECNE` | `0x0C` | ECN Echo |
| `CNET_SCTP_ECN_CWR` | `0x0D` | Congestion Window Reduced |
| `CNET_SCTP_SHUTDOWN_COMPLETE` | `0x0E` | Shutdown complete |
| `CNET_SCTP_AUTH` | `0x0F` | Authentication chunk |
| `CNET_SCTP_TSN` | `0xC0` | Forward TSN (RFC 3758) |
| `CNET_SCTP_ASCONF` | `0xC1` | Address configuration change |
| `CNET_SCTP_ASCONF_ACK` | `0x80` | Address configuration ACK |

## DATA chunk flags (for `struct cnet_sctp_data_chunk.flags`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_SCTP_DATA_E` | `0x01` | Ending fragment |
| `CNET_SCTP_DATA_B` | `0x02` | Beginning fragment |
| `CNET_SCTP_DATA_U` | `0x04` | Unordered delivery |
| `CNET_SCTP_DATA_I` | `0x08` | Immediate SACK requested |

## Parameter types (for `struct cnet_sctp_param.type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_SCTP_PARAM_HBEAT_INFO` | `0x0001` | Heartbeat info |
| `CNET_SCTP_PARAM_IPV4_ADDR` | `0x0005` | IPv4 address |
| `CNET_SCTP_PARAM_IPV6_ADDR` | `0x0006` | IPv6 address |
| `CNET_SCTP_PARAM_STCOOKIE` | `0x0007` | State cookie |
| `CNET_SCTP_PARAM_UNR_PARAM` | `0x0008` | Unrecognized parameter |
| `CNET_SCTP_PARAM_COOKIE_PRESERV` | `0x0009` | Cookie preservative |
| `CNET_SCTP_PARAM_HOST_ADDR` | `0x000B` | Host name address |
| `CNET_SCTP_PARAM_SUPPORTED_ADDR` | `0x000C` | Supported address types |
| `CNET_SCTP_PARAM_ECN_CAP` | `0x8000` | ECN capable |
| `CNET_SCTP_PARAM_RANDOM` | `0x8002` | Random (auth) |
| `CNET_SCTP_PARAM_LCHUNK` | `0x8003` | Chunk list (auth) |
| `CNET_SCTP_PARAM_HMAC` | `0x8004` | HMAC algorithm (auth) |
| `CNET_SCTP_PARAM_SUPPORTED_EXT` | `0x8008` | Supported extensions |
| `CNET_SCTP_PARAM_SUPPORTED_TSN` | `0xC000` | Forward-TSN supported |
| `CNET_SCTP_PARAM_ADD_IP` | `0xC001` | Add IP address (ASCONF) |
| `CNET_SCTP_PARAM_DEL_IP` | `0xC002` | Delete IP address (ASCONF) |
| `CNET_SCTP_PARAM_ERR_CAUSE` | `0xC003` | Error cause indication |
| `CNET_SCTP_PARAM_SET_PRIMARY` | `0xC004` | Set primary address |
| `CNET_SCTP_PARAM_SUCCESS` | `0xC005` | Success indication |
| `CNET_SCTP_PARAM_ADAPT_LAYER` | `0xC006` | Adaptation layer indication |

## Chunk-specific flag/type fields

| Define | Value | Meaning |
|---|---|---|
| `CNET_SCTP_ABORT_T` | `0x01` | T-bit for ABORT (`struct cnet_sctp_abort_chunk.flags`) |
| `CNET_SCTP_SHUTDOWN_T` | `0x01` | T-bit for SHUTDOWN COMPLETE (`struct cnet_sctp_shutdown_complete_chunk.flags`) |

## Error codes (for `struct cnet_sctp_error_chunk` cause codes)

| Define | Value | Meaning |
|---|---|---|
| `CNET_SCTP_ERROR_INVALID_STREAM` | `0x0001` | Invalid stream identifier |
| `CNET_SCTP_ERROR_MISSING_PARAM` | `0x0002` | Missing mandatory parameter |
| `CNET_SCTP_ERROR_STALE_COOKIE` | `0x0003` | Stale cookie error |
| `CNET_SCTP_ERROR_OOR` | `0x0004` | Out of resource |
| `CNET_SCTP_ERROR_UNR_ADDR` | `0x0005` | Unresolvable address |
| `CNET_SCTP_ERROR_UNR_CHUNK` | `0x0006` | Unrecognized chunk type |
| `CNET_SCTP_ERROR_INVALID_PARAM` | `0x0007` | Invalid mandatory parameter |
| `CNET_SCTP_ERROR_UNR_PARAM` | `0x0008` | Unrecognized parameters |
| `CNET_SCTP_ERROR_NUSER_DATA` | `0x0009` | No user data |
| `CNET_SCTP_ERROR_COOKIE_SHUTDOWN` | `0x000A` | Cookie received while shutting down |
| `CNET_SCTP_ERROR_RASSOC` | `0x000B` | Restart of an association with new addresses |
| `CNET_SCTP_ERROR_UIA` | `0x000C` | User-initiated abort |
| `CNET_SCTP_ERROR_PV` | `0x000D` | Protocol violation |

## See also

- `CNET_SCTP_CSUM()` — [`docs/functions/CNET_SCTP_CSUM.md`](../functions/CNET_SCTP_CSUM.md)
- Struct reference: [`docs/structs/cnet_sctp.md`](../structs/cnet_sctp.md)
