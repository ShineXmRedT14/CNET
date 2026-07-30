# `CNET_GET_MAC`

```c
cnet_errno_t CNET_GET_MAC(void *route);
```

![CNET_GET_MAC implementation](assets/get_mac.jpg)

## Description

Resolves the local machine's MAC address for the interface stored in `struct cnet_route` and writes it back into that same structure.

## Parameters

- `void *route` — pointer to a `struct cnet_route` (cast to `void *`) that will receive the local MAC address

## Returns

A `cnet_errno_t` status code indicating success or the reason the lookup failed.

## See also

- `struct cnet_route` — `docs/structs/cnet_route.md`
