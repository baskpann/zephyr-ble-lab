# Wiring — STM32F4 Host ↔ nRF52840 Controller

The Host and Controller communicate over UART using the H4 HCI transport.
Four signals are required (data + hardware flow control); GND is shared.

## Pin mapping

| Signal | STM32F4 Disco | nRF52840-DK | Notes |
|---|---|---|---|
| TX (Host → Controller) | PB10 (USART3_TX) | P0.06 | Host TX → Controller RX |
| RX (Host ← Controller) | PB11 (USART3_RX) | P0.08 | Controller TX → Host RX |
| RTS (Host → Controller) | PB14 (USART_RTS) | P0.07 | Host CTS → Controller RTS |
| CTS (Host ← Controller) | PB13 (USART_CTS) | P0.05 | Controller CTS → Host RTS |
| GND | GND | GND | Common ground — required |

**Cross-over reminder:** TX<->RX and RTS<->CTS are swapped between the two
boards (a board's TX drives the other's RX; a board's RTS gates the
other's CTS) — this is the single most common wiring mistake and is worth
re-checking against the diagram below before powering on.

## Signal diagram

```
        STM32F4 (Host)                      nRF52840 (Controller)
        ┌──────────────┐                    ┌──────────────┐
        │              │                    │              │
        │   USART3_TX  ├───────────────────►│  UART0_RX    │
        │   USART3_RX  │◄───────────────────┤  UART0_TX    │
        │   USART3_RTS ├───────────────────►│  UART0_CTS   │
        │   USART3_CTS │◄───────────────────┤  UART0_RTS   │
        │   GND        ├────────────────────┤  GND         │
        │              │                    │              │
        └──────────────┘                    └──────────────┘
```

## UART configuration

Both sides must agree on baud rate and flow control — mismatches here are
the second most common bring-up failure after crossed TX/RX.

| Parameter | Value |
|---|---|
| Baud rate | 115200 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Flow control | Hardware (RTS/CTS) |

## Verifying the wiring before flashing

1. Power both boards independently via their USB debug ports (don't power
   one from the other's 3V3/5V rail unless you've confirmed voltage
   compatibility).
2. Confirm common GND continuity between boards.
3. Only then flash both images and proceed to `bt init` / `bt advertise on`
   per the main README.
