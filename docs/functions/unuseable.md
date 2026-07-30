> **Internal helper — not part of the public API.**
> This function exists purely to support `CNET_RAND()`. It is not meant
> to be called directly from application `.c` files.

# `cnet_xorrand()`

```c
static inline void cnet_xorrand(uint32_t state, uint32_t *xorstate);
```

## Description

Seeds/advances the internal xorshift state used by `CNET_RAND()` to
produce pseudo-random numbers without relying on libc's non-reentrant
`rand()`. It is only ever called from inside `CNET_RAND()` and has no
direct use on its own.

## Parameters

- `uint32_t state` — seed value (typically derived from `CLOCK_MONOTONIC`)
- `uint32_t *xorstate` — pointer to the xorshift state to initialize/update

## See also

- `CNET_RAND()` — `docs/functions/CNET_RAND.md`
