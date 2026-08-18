# `CNET_FRAME_FCS`

```c
void CNET_FRAME_FCS(struct cnet_frame_fcs *ffcs, void *data, size_t len);
```

## Description

Need to get FCS for frame packet

## Parameters

- `struct cnet_frame_fcs *ffcs` — structure where will getting fcs
- `void *data` — bytes of all frame packet
- `size_t len` — len of all bytes

## Returns

void
