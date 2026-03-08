# USB Communication Protocol

## Overview

This document describes the binary packet protocol used for USB CDC communication between the STM32 microcontroller and the PC.

---

## Packet Structure

```
┌──────────┬───────────────┬───────────┬─────────────┬──────────────┐
│  Byte 0  │  Bytes [1-2]  │  Byte 3   │  Bytes [4…n]│ Bytes [n, n+1]│
├──────────┼───────────────┼───────────┼─────────────┼──────────────┤
│  Magic   │     Length    │   Type    │   Payload   │    CRC16     │
│0b11000011│   (uint16_t)  │ (uint8_t) │  (N bytes)  │  (uint16_t)  │
└──────────┴───────────────┴───────────┴─────────────┴──────────────┘
```

| Offset    | Size    | Field    | Description                                      |
|-----------|---------|----------|--------------------------------------------------|
| `0`       | 1 byte  | Magic    | Start byte — always `0xC3` (`0b11000011`)        |
| `[1-2]`   | 2 bytes | Length   | Payload length in bytes, little-endian `uint16_t`|
| `3`       | 1 byte  | Type     | Data type identifier                             |
| `[4…n]`   | N bytes | Payload  | Data content, length defined by the Length field |
| `[n, n+1]`| 2 bytes | CRC16    | CRC16 checksum over the payload bytes            |

---
