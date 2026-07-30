# Structs — `cnet_route.h`

## `struct cnet_route`

Local network identity — the interface name, MAC, and IP address discovered by [`CNET_GET_IF()`](../functions/CNET_GET_IF.md) / [`CNET_GET_IP()`](../functions/CNET_GET_IP.md) / [`CNET_GET_MAC()`](../functions/CNET_GET_MAC.md).

```c
struct cnet_route
{
    char iface[16];
    uint8_t mac[6];
    union { uint8_t i8p[4]; uint32_t i32p; } ip;
};
```

| Field | Type | Meaning |
|---|---|---|
| `iface` | `char[16]` | Interface name (e.g. `"eth0"`), filled by `CNET_GET_IF()` |
| `mac` | `uint8_t[6]` | Interface MAC address, filled by `CNET_GET_MAC()` |
| `ip` | union | Interface IPv4 address — `ip.i8p` (bytes) or `ip.i32p` (32-bit), filled by `CNET_GET_IP()` |

`CNET_GET_IP()` and `CNET_GET_MAC()` both call `CNET_GET_IF()` internally, so you don't need to call it yourself unless you only need the interface name.

## See also

- [`CNET_GET_IF`](../functions/CNET_GET_IF.md), [`CNET_GET_IP`](../functions/CNET_GET_IP.md), [`CNET_GET_MAC`](../functions/CNET_GET_MAC.md)
