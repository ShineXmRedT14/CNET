# `CNET_SOCK_ADDR_LL`

```c
struct sockaddr_ll CNET_SOCK_ADDR_LL(int sock);
```

## Description

Builds a `struct sockaddr_ll` bound to the local interface (as discovered by [`CNET_GET_IF()`](CNET_GET_IF.md)) for sending raw Layer 2 (`AF_PACKET`) frames on the given socket — pass the result as the `addr` parameter to [`CNET_BUFFER_INIT_ALL()`](CNET_BUFFER_INIT_ALL.md) / [`CNET_BUFFER_INIT_SET()`](CNET_BUFFER_INIT_SET.md).

This is the Layer 2 counterpart of [`CNET_SOCK_ADDR_IN()`](CNET_SOCK_ADDR_IN.md).

## Parameters

- `int sock` — an `AF_PACKET` socket, as returned by `CNET_SOCK(CNET_LAYER_ETHER, 0)`; used to resolve the interface index via `ioctl(SIOCGIFINDEX)`

## Returns

A `struct sockaddr_ll` with `sll_family = AF_PACKET` and `sll_ifindex` set to the local interface's index.

## Example

```c
int sockfd = CNET_SOCK(CNET_LAYER_ETHER, 0);
struct sockaddr_ll addr = CNET_SOCK_ADDR_LL(sockfd);
```

## See also

- [`CNET_SOCK_ADDR_IN`](CNET_SOCK_ADDR_IN.md) — Layer 3 equivalent
- [`CNET_GET_IF`](CNET_GET_IF.md), [`CNET_SOCK`](CNET_SOCK.md)
