# `CNET_SOCK_ADDR_IN`

```c
static inline struct sockaddr_in CNET_SOCK_ADDR_IN(struct cnet_ip *cip);
```

## Description

Builds a ready-to-use `struct sockaddr_in` targeting the destination address in `cip`, for sending Layer 3 (`AF_INET`) raw packets — pass the result as the `addr` parameter to [`CNET_BUFFER_INIT_ALL()`](CNET_BUFFER_INIT_ALL.md) / [`CNET_BUFFER_INIT_SET()`](CNET_BUFFER_INIT_SET.md), or use it directly with `sendto()`.

This is the Layer 3 counterpart of [`CNET_SOCK_ADDR_LL()`](CNET_SOCK_ADDR_LL.md), which builds the Layer 2 equivalent.

## Parameters

- `struct cnet_ip *cip` — pointer to the destination `struct cnet_ip`; its `dst` field supplies the address

## Returns

A `struct sockaddr_in` with `sin_family = AF_INET` and `sin_addr` set from `cip->dst`.

## Example

```c
struct cnet_ip ip = {0};
// ... fill in ip.dst, etc. ...
struct sockaddr_in addr = CNET_SOCK_ADDR_IN(&ip);
sendto(sockfd, &tcp, sizeof(tcp), 0, (struct sockaddr *)&addr, sizeof(addr));
```

## See also

- `struct cnet_ip` — [`docs/structs/cnet_ip.md`](../structs/cnet_ip.md)
- [`CNET_SOCK_ADDR_LL`](CNET_SOCK_ADDR_LL.md) — Layer 2 equivalent
- [`CNET_SOCK`](CNET_SOCK.md)
