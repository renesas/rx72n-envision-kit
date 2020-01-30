PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_sdc_sdmem_rx (SD Mode SD Memory Card Driver)

=======================

Document Number 
---------------
R01AN4233JJ0203
R01AN4233EJ0203

Version
-------
v2.03

Overview
--------
This driver is a block type device driver to control the SD mode SD memory card using SD Host Interface (SDHI).

Features
--------
* SD Specifications Part 1 Physical Layer Simplified Specifications version 6.00 (SD memory control commands are supported)
* Block type device driver which takes 1 block to be 512 bytes
* SD mode (4 bits) is supported.
* Default speed mode are supported.
* Standard capacity SD memory cards (SDSC) and high-capacity SD memory cards (SDHC, SDXC) are supported.
* Only user areas are supported.
* Only CD pin detection is supported.

Supported MCUs
--------------
* RX Family MCU

Boards Tested On
----------------
* RSK for RX Family MCU
 
Limitations
-----------
None

Peripherals Used Directly
-------------------------
* SD Host Interface (SDHI)

Required Packages
-----------------
* r_bsp    v3.60

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (3).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (2) or (4) for unsupported RX devices.

(1) Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2) Adding the FIT module to your project using "FIT Configurator" in e2 studio
By using the "FIT Configurator" in e2 studio,
the FIT module is automatically added to your project.
Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3) Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(4) Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

Toolchain(s) Used
-----------------
* Renesas RX v2.08

File Structure
--------------
r_sdc_sdmem_rx
|   r_sdc_sd_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an4233ej0203-rx.pdf
|   |
|   +---ja
|           r01an4233jj0203-rx.pdf
|
+---ref
|       r_sdc_sd_rx_config_reference.h
|
+---src
    |   r_sdc_sd_cd.c
    |   r_sdc_sd_cmd.c
    |   r_sdc_sd_config.c
    |   r_sdc_sd_initialize.c
    |   r_sdc_sd_int.c
    |   r_sdc_sd_open.c
    |   r_sdc_sd_rx_private.h
    |   r_sdc_sd_trans.c
    |   r_sdc_sd_util.c
    |   r_sdc_sd_wrapper.c
    |
    +---mem
            r_sdc_sdmem_wrapper.c
            r_sdc_sdmem_param.c
            r_sdc_sdmem_read.c
            r_sdc_sdmem_util.c
            r_sdc_sdmem_wrapper.c
            r_sdc_sdmem_write.c

r_config
    r_sdc_sd_rx_config.h

