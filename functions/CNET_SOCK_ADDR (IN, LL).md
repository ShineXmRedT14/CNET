# `CNET_SOCK_ADDR` (superseded)

> This page previously documented a single `CNET_SOCK_ADDR(int layer, struct cnet_ip *cip)` helper. The current `cnet.h` no longer has that function — it was split into two layer-specific functions instead. Update any code still calling `CNET_SOCK_ADDR(layer, cip)` to call one of the two below directly.

- **Layer 3 (`CNET_LAYER_IP`)** → [`CNET_SOCK_ADDR_IN(struct cnet_ip *cip)`](CNET_SOCK_ADDR_IN.md)
- **Layer 2 (`CNET_LAYER_ETHER`)** → [`CNET_SOCK_ADDR_LL(int sock)`](CNET_SOCK_ADDR_LL.md)

Note the parameter differs between the two: `CNET_SOCK_ADDR_IN` takes the destination `struct cnet_ip *`, while `CNET_SOCK_ADDR_LL` takes the socket file descriptor itself (it resolves the outgoing interface from the socket via `CNET_GET_IF()`).

## See also

- [`CNET_SOCK`](CNET_SOCK.md)
