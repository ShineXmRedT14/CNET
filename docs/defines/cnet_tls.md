# Defines — `cnet_tls.h`

## Record content types (for `struct cnet_tls_record.type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_TLS_TYPE_CCS` | `0x14` | Change Cipher Spec |
| `CNET_TLS_TYPE_ALERT` | `0x15` | Alert |
| `CNET_TLS_TYPE_HANDSHAKE` | `0x16` | Handshake |
| `CNET_TLS_TYPE_DATA` | `0x17` | Application Data |

## Protocol versions (for `struct cnet_tls_record.version`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_TLS_VERSION_1_0` | `0x0301` | TLS 1.0 |
| `CNET_TLS_VERSION_1_1` | `0x0302` | TLS 1.1 |
| `CNET_TLS_VERSION_1_2` | `0x0303` | TLS 1.2 |
| `CNET_TLS_VERSION_1_3` | `0x0304` | TLS 1.3 |

## Handshake message types (for `struct cnet_tls_handshake.type`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_TLS_HANDSHAKE_CLIENT` | `0x01` | ClientHello |
| `CNET_TLS_HANDSHAKE_SERVER` | `0x02` | ServerHello |
| `CNET_TLS_HANDSHAKE_CERT` | `0x0B` | Certificate |
| `CNET_TLS_HANDSHAKE_SERVER_KE` | `0x0C` | ServerKeyExchange |
| `CNET_TLS_HANDSHAKE_SERVER_DONE` | `0x0E` | ServerHelloDone |
| `CNET_TLS_HANDSHAKE_CLIENT_KE` | `0x10` | ClientKeyExchange |
| `CNET_TLS_HANDSHAKE_FINISHED` | `0x14` | Finished |

## See also

- Struct reference: [`docs/structs/cnet_tls.md`](../structs/cnet_tls.md)
