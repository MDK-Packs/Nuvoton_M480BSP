Blinky project
==============

The **Blinky** project can be easily used to verify the basic tool setup.

It is compliant to the [Cortex Microcontroller Software Interface Standard (CMSIS)](https://arm-software.github.io/CMSIS_5/General/html/index.html)
and uses the [CMSIS-RTOS v2 API](https://arm-software.github.io/CMSIS_5/RTOS2/html/index.html) for RTOS functionality. The CMSIS-RTOS v2 API
is supported by various real-time operating systems, for example [Keil RTX5](https://arm-software.github.io/CMSIS_5/RTOS2/html/rtx5_impl.html) or [FreeRTOS](https://github.com/ARM-software/CMSIS-FreeRTOS).

Operation
---------

 - At start the `vioLED0` blinks in 1 sec interval.
 - The `vioBUTTON0` changes the blink frequency and start/stops `vioLED1`.

The board hardware mapping of `vioLED0`, `vioLED1`, and `vioBUTTON0` depends on the 
configuration of the [CMSIS-Driver VIO](https://arm-software.github.io/CMSIS_5/Driver/html/group__vio__interface__gr.html).

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
