Cherry MX Keyboard
=======

[![Keyboard](https://cdn.sparkfun.com/assets/home_page_posts/2/4/1/2/Front.jpg)](https://cdn.sparkfun.com/assets/home_page_posts/2/4/1/2/Front.jpg)

The keyboard has a 4x4 matrix of cherry MX switches, which are connected to the [SparkFun Pro Micro](https://www.sparkfun.com/products/12587). Each key can send either a keyboard or mouse command to the host computer thanks to the improved [HID Arduino Library](https://github.com/NicoHood/HID). Rotary encoder support was provided by the [Teensy Encoder Library](https://www.pjrc.com/teensy/td_libs_Encoder.html). The software is currently configured as an Eagle shortcut keyboard, where each button corresponds to an Eagle command. The left rotary encoder controls the computer's volume, and the built in encoder switch for mute. The right encoder controls the grid spacing, and the encoder switch will toggle between imperial and metric units.

External Files Used
-------------------

 * The Cherry MX key caps were 3D printed on a [Lulzbot Mini](https://www.sparkfun.com/products/13256). The keycaps used were found on Thingiverse [here](https://www.thingiverse.com/thing:1320847).
 * The HID Arduino Library can be found [here](https://github.com/NicoHood/HID).
 * The Encoder library can be found [here](https://www.pjrc.com/teensy/td_libs_Encoder.html).

Repository Contents
-------------------

* **/Lasercut Frame** - Files used to cut out the 1/8" acrylic (.pdf, .svg)
    * The Cherry MX switches mounting tabs have a thickness of 1/16", the raster path was used to cut through approximately half the material thickness.
* **/Schematic** - Design files (.png, .sch)
* **/Firmware** - Code for the Pro Mini (.ino)

License Information
-------------------

This design is [OSHW](http://www.oshwa.org/definition/) and public domain but you buy me a beer if you use this and we meet someday ([Beerware license](http://en.wikipedia.org/wiki/Beerware)).