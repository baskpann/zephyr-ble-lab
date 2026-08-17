# zephyr-ble-lab

Personal projects exploring Bluetooth Low Energy on Zephyr RTOS, using a
split Host/Controller architecture:

- **Controller** — nRF52840, running Zephyr's BLE Controller stack, exposing
  HCI over UART.
- **Host** — STM32F4, running Zephyr's BLE Host stack, talking to the
  Controller over the same HCI UART link.

This mirrors how BLE SoCs are split in real products (radio silicon runs the
Controller, application MCU runs the Host) and is a deliberate way to learn
both halves of the stack plus the HCI transport between them.

## Repo structure

```
zephyr-ble-lab/
├── west.yml                 # west manifest — pulls Zephyr + required HAL modules
├── apps/
│   ├── controller/          # nRF52840: Controller-only build, H4 UART
│   │   ├── src/
|   |   ├── inc/
|   |   └── boards/
│   ├── host                 # STM32F4: Host-only build
│   │   ├── 00-hci-bringup
|   │   |   ├── src/
|   |   |   ├── inc/
|   |   |   └── boards/
|   |   └── 01-...
├── docs/                    # Architecture notes, wiring diagrams
└── .github/workflows/       # CI: builds app on push
```

Each numbered folder under `host/` is a self-contained milestone project.
They're kept live and buildable on `main` rather than split into branches,
so the whole progression is visible at once. Tagged releases
(`v0.0-hci-bringup`, `v0.1-`, ...) mark the repo state at the end of each
project.

## Setup

This repo is a **west manifest repo** (T2 topology) — it doesn't contain
Zephyr itself. `west` fetches Zephyr and the required HAL modules into
sibling folders inside a workspace, based on `west.yml`.

```bash
# 1. Python environment for west + Zephyr tooling
python3 -m venv ~/zephyr-ble-workspace/.venv
# do this in every new shell
source ~/zephyr-ble-workspace/.venv/bin/activate
pip install west

# 2. Create the workspace and pull this repo into it as the manifest project
west init -m https://github.com/baskpann/zephyr-ble-lab --mr main zephyr-ble-workspace
cd zephyr-ble-workspace

# 3. Fetch Zephyr + modules per west.yml
west update
pip install -r zephyr/scripts/requirements.txt

# 4. Register Zephyr as a CMake package (one-time per checkout, not per shell)
west zephyr-export

# 5. Set up the Zephyr build environment (do this in every new shell)
source zephyr/zephyr-env.sh
```

Resulting layout:

```
zephyr-ble-workspace/            # not committed — machine-local workspace
├── .venv/
├── .west/
├── zephyr/               # pulled per west.yml revision pin
├── modules/               # hal_nordic, hal_stm32, etc.
└── zephyr-ble-lab/        # this repo
```

## Building an app

```bash
source ~/zephyr-ble-workspace/.venv/bin/activate
source ~/zephyr-ble-workspace/zephyr/zephyr-env.sh

# Controller (nRF52840)
west build -b nrf52840dk_nrf52840 -d build/controller \
  zephyr-ble-lab/apps/controller/00-hci-bringup

# Host (STM32F4)
west build -b stm32f4_disco -d build/host \
  zephyr-ble-lab/apps/host/00-hci-bringup/
```

Flash each with `west flash -d build/<controller|host>` after wiring the
boards per `docs/wiring.md`.

## Projects/Apps

| # | Project/Apps | Status |
|---|---------|--------|
| 00 | HCI bring-up (Host + Controller split over UART) | In progress |

Planning to add more apps in the future

## Hardware

- Controller: nRF52840-DK (nrf52840dk/nrf52840)
- Host: STM32F4 Discovery (stm32f4_disco)
- Wiring: UART TX/RX/RTS/CTS cross-connected + common GND — see
  `docs/wiring.md` for pin mapping.

## Tested against

- Zephyr: `v3.7.0`
- West: `v1.5.0`
