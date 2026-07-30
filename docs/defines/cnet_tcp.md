# Defines — `cnet_tcp.h`

## Flags (for `struct cnet_tcp.flags`, a 6-bit field)

| Define | Value | Meaning |
|---|---|---|
| `CNET_TCP_FIN` | `0x0001` | FIN — no more data from sender |
| `CNET_TCP_SYN` | `0x0002` | SYN — synchronize sequence numbers |
| `CNET_TCP_RST` | `0x0004` | RST — reset the connection |
| `CNET_TCP_PSH` | `0x0008` | PSH — push buffered data to the app |
| `CNET_TCP_ACK` | `0x0010` | ACK — acknowledgment field is significant |
| `CNET_TCP_URG` | `0x0020` | URG — urgent pointer field is significant |

Combine with bitwise OR for multi-flag segments, e.g. `CNET_TCP_SYN | CNET_TCP_ACK`.

## Header defaults

| Define | Value | Meaning |
|---|---|---|
| `CNET_TCP_DOFF` | `0x0005` | Default data offset (5 → 20-byte header, no options) |
| `CNET_TCP_RES1` | `0x0000` | Reserved bits before the flags (always 0) |
| `CNET_TCP_RES2` | `0x0000` | Reserved bits after the flags (always 0) |
| `CNET_TCP_WIN` | `0xFFFF` | Maximum window size, handy as a default advertised window |

## See also

- `enum cnet_state_ca` and struct reference: [`docs/structs/cnet_tcp.md`](../structs/cnet_tcp.md)
