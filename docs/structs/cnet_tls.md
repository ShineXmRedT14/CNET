# Structs — `cnet_tls.h`

## `struct cnet_tls_record`

The 5-byte TLS record header (the encrypted/plaintext record fragment follows, `len` bytes long).

```c
struct cnet_tls_record
{
    uint8_t type;
    uint16_t version;
    uint16_t len;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Content type — one of `CNET_TLS_TYPE_*` (CCS/Alert/Handshake/Data) |
| `version` | `uint16_t` | Protocol version — one of `CNET_TLS_VERSION_*`, big-endian |
| `len` | `uint16_t` | Length of the record fragment that follows, big-endian |

## `struct cnet_tls_handshake`

The 4-byte handshake message header (nested inside a `CNET_TLS_TYPE_HANDSHAKE` record).

```c
struct cnet_tls_handshake
{
    uint8_t type;
    uint8_t len[3];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `type` | `uint8_t` | Handshake message type — one of `CNET_TLS_HANDSHAKE_*` |
| `len` | `uint8_t[3]` | 24-bit big-endian length of the handshake body that follows |

## See also

- Defines: [`docs/defines/cnet_tls.md`](../defines/cnet_tls.md)
