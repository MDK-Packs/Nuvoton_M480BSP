Platform project
================

The Platform project configures the hardware of the evaluation board and is a CMSIS-RTOS2 based software template that can be further expanded.

RTOS: Keil RTX5 Real-Time Operating System
------------------------------------------

The real-time operating system [Keil RTX5](https://arm-software.github.io/CMSIS_5/RTOS2/html/rtx5_impl.html) implements the resource management. 

It is configured with the following settings:

- [Global Dynamic Memory size](https://arm-software.github.io/CMSIS_5/RTOS2/html/config_rtx5.html#systemConfig): 24000 bytes
- [Default Thread Stack size](https://arm-software.github.io/CMSIS_5/RTOS2/html/config_rtx5.html#threadConfig): 3072 bytes
- [Event Recorder Configuration](https://arm-software.github.io/CMSIS_5/RTOS2/html/config_rtx5.html#evtrecConfig)
  - [Global Initialization](https://arm-software.github.io/CMSIS_5/RTOS2/html/config_rtx5.html#evtrecConfigGlobIni): 1
    - Start Recording: 1

Refer to [Configure RTX v5](https://arm-software.github.io/CMSIS_5/RTOS2/html/config_rtx5.html) for a detailed description of all configuration options.

Board: Nuvoton NuMaker-M487KM
-----------------------------

The tables below list the device configuration for this board.

The heap/stack setup and the CMSIS-Driver assignment is in configuration files of related software components.

The example project can be re-configured to work on custom hardware. Refer to ["Configuring Example Projects with MCUXpresso Config Tools"](https://github.com/MDK-Packs/Documentation/tree/master/Using_MCUXpresso) for information.

### System Configuration

| System Component        | Setting
|:------------------------|:-------------------------------------------------------------
| Device                  | M487JIDAE
| Board                   | NuMaker-M487KM
| Heap                    | 256  B
| Stack (MSP)             |   2 kB

### Clock Configuration

| Clock                   | Setting
|:------------------------|:--------
| PLLCLK                  |  192 MHz

### GPIO Configuration and usage

| Port | Pin | Identifier | Pin Settings      | Usage
|:-----|:----|:-----------|:------------------|:------------------------
| PH   |  4  | LEDR       | output, pull-down | User LEDR (PH.4)
| PH   |  5  | LEDG       | output, pull-down | User LEDG (PH.5)
| PG   | 15  | SW2        | input             | User Button SW2 (PG.15)
| PF   | 11  | SW3        | input             | User Button SW3 (PF.11)

### CMSIS-Driver mapping

| CMSIS-Driver VIO  | Physical board hardware
|:------------------|:------------------------------
| vioBUTTON0        | User Button SW2
| vioBUTTON1        | User Button SW3
| vioLED0           | User LED RED
| vioLED1           | User LED GREEN
