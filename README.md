https://github.com/user-attachments/assets/3a3785d2-84e4-4510-96f3-b70b62eddb09

# FreshWatch

This is a watch that can monitor air quality, with open-source hardware and software, allowing for free programming.

![20241003_224117](doc/20241003_224117.jpg)

![20241004_153557](doc/20241004_153557.jpg)

![20241004_153757](doc/20241004_153757.jpg)

![20241004_153816](doc/20241004_153816.jpg)

![20241004_154010](doc/20241004_154010.jpg)

![20241004_154049](doc/20241004_154049.jpg)





# Specification

- NRF52840
- SGP40
- SHT30
- LSM6DS3TR-C
- Display: LPM009M360A (0.85inch 72*144 )
- Battery: 402030
- watch band: 20mm

![20241008163717](doc/20241008163717.jpg)



# 3D Printing

[3D files](https://makeronline.com/en/model/Fresh%20Watch/48677.html)



# Hardware

PCB thickness: 0.8mm



# Mechanical

![preview](doc/preview.png)

# Software

#### PlatformIO

Before burning, you need to use a programming tool like JLink to install the bootloader (see the file: freshwatch_nrf52840_bootloader-0.9.2_s140_6.1.1.hex). Then, you can compile this code and proceed with the download.

For future firmware updates, you can connect the device via USB, then enter UF2 mode through the menu, and drag the flash.uf2 file into the USB drive.

A flash.uf2 file will be automatically generated each time you compile, making it convenient for updates.

The pages directory contains content related to the various app functionalities. You can write these according to the rules in the template folder and sequentially fill in these functions.

![04-10-2024 13.47.22](doc/04-10-2024 13.47.22.png)





#### CircuitPython

First, you need to burn the bootloader, then drag the firmware_9.1.1_0826.uf2 file into the generated USB drive.

This firmware uses external flash as the code storage system, so if you want to use this Python framework, you must solder the flash chip.

For further development, you can refer to the CircuitPython official website.









# License

This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/).

[![320px-Cc-by-nc-sa_icon](https://github.com/user-attachments/assets/735aec1d-c601-4b99-9d29-5e30462bed77)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

