# Vgm demo player for esp32

[tocstart]: # (toc start)

  * [Introduction](#introduction)
  * [Key Features](#key-features)
  * [Supported platforms](#supported-platforms)
  * [Setting up](#setting-up)
  * [License](#license)

[tocend]: # (toc end)

## Introduction

This is proof of concept of VGM music player for ESP32. This project uses code from the following sources:

 * [vgmplay library for esp32](https://github.com/lexus2k/vgmplay_esp32) - ported to RTOS library. You
   can find original library for PC at https://github.com/vgmrips/vgmplay.
 * [audio player for esp32](https://github.com/lexus2k/audioplayer_esp32)
 * [Vampire Killer game vgm music files](https://vgmrips.net/packs/pack/vampire-killer-msx2)

## Key Features

 * Can support different formats (extendable).
 * For now supports: VGM, and native notes arrays.

## Supported platforms

| **Platforms** | **Comments** |
| :-------- |:---------|
| **ESP32 IDF** |     |

## Setting up

Connect low-power speaker or amplifier to GPIO25.

*ESP32-IDF*
  * How to get project
> git clone --recursive https://github.com/lexus2k/vgm_test
> git submodule update --init --recursive<br>

  * Recommended ESP32 IDF version
> git clone -b v3.2-beta1 --recursive https://github.com/espressif/esp-idf.git esp-idf<br>

## License

The program is free. If this project helps you, you can give me a cup of coffee.
[![Donate via Paypal](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.me/lexus2k)


Copyright (C) 2019  Alexey Dynda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

