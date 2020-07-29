Cherry MX Wireless Keyboard
=======

[![Wireless Keyboard](https://cdn.sparkfun.com/r/500-500/assets/home_page_posts/3/3/4/5/BLEkeyboard.jpg)](https://cdn.sparkfun.com/assets/home_page_posts/3/3/4/5/BLEkeyboard.jpg)

The keyboard has a 4x4 matrix of cherry MX switches, which are connected to the [SparkFun ESP32 Thing Plus](https://www.sparkfun.com/products/15663). Each key can send either a keyboard or mouse command to the host computer thanks to the [BLE HID Arduino Library](https://github.com/T-vK/ESP32-BLE-Keyboard). Rotary encoder support was provided by the [Teensy Encoder Library](https://www.pjrc.com/teensy/td_libs_Encoder.html). The software is currently configured as an Eagle shortcut keyboard, where each button corresponds to an Eagle command. The left rotary encoder controls the computer's volume, and the built in encoder switch for mute. The right encoder controls the grid spacing, and the encoder switch will toggle between imperial and metric units.

External Files Used
-------------------

 * The Cherry MX key caps can be found [here](https://www.sparkfun.com/products/15307).
 * The HID Arduino Library can be found [here](https://github.com/NicoHood/HID).
 * The Encoder library can be found [here](https://www.pjrc.com/teensy/td_libs_Encoder.html).

Repository Contents
-------------------

* **/Case** - Files used to cut out the 1/8" acrylic (.pdf, .svg) and images used for the key caps
    * The Cherry MX switches mounting tabs have a thickness of 1/16", the raster path was used to cut through approximately half the material thickness.
* **/Schematic** - Design files (.png, .sch)
* **/Firmware** - Code for the ESP32 (.ino)

License Information
-------------------

This design is [OSHW](http://www.oshwa.org/definition/) and public domain but you buy me a beer if you use this and we meet someday ([Beerware license](http://en.wikipedia.org/wiki/Beerware)).