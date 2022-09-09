# DDScopeX

**DDScopeX-specific** files for a Direct Drive Telescope Plugin for OnStepX 
- A servo motor controlled F4.5, 13", ALT/AZ GoTo Newtonian Telescope

Author: Richard Benear 2021, 2022

This repository is a subset of [DDScopeX-Plugin-for-OnStepX](https://github.com/phylos51/DDScopeX-Plugin-for-OnStepX) and is intended to contain only the "DDScopeX specific" files and will not compile in this form since the OnStepX folders and files are not present. For a compilable and tested version go to the [DDScopeX-Plugin-for-OnStepX](https://github.com/phylos51/DDScopeX-Plugin-for-OnStepX) repository. This repository (**DDScopeX**) may trail the primary repository in versions.

The reason for isolating these files into [DDScopeX](https://github.com/phylos51/DDScopeX) is because there are multiple parts of this code that can act as examples for people searching for solutions to projects that don't necessarily involve a telescope controller. Hopefully, it may be helpful in reducing the amount of searching and development time for the examples listed below.

The README for [DDScopeX-Plugin-for-OnStepX](https://github.com/phylos51/DDScopeX-Plugin-for-OnStepX).
The README for [OnStepX](https://github.com/hjd1964/OnStepX/blob/main/README.md) .

## Examples that could be helpful

- [C++ Class and Methods](https://github.com/phylos51/DDScopeX/blob/main/DDScope/display/UIelements.cpp) for doing flicker free canvas printing and drawing buttons to a TFT screen.
- [Fonts](https://github.com/phylos51/DDScopeX/tree/main/DDScope/fonts) other than Default GFX Arial are used.
- [CAN bus driver and methods](https://github.com/phylos51/DDScopeX/tree/main/DDScope/ODriveTeensyCAN) for an ODrive controller
- [ODrive controller driver support functions](https://github.com/phylos51/DDScopeX/tree/main/DDScope/odriveExt)
- [ODrive Controller status and control Screen](https://github.com/phylos51/DDScopeX/blob/main/DDScope/screens/ODriveScreen.cpp)
- Nested Menu structure [here](https://github.com/phylos51/DDScopeX/blob/main/DDScope/screens/TouchScreen.cpp) and [here](https://github.com/phylos51/DDScopeX/blob/main/DDScope/display/Display.cpp) including Touchscreen
- Example of how to get a 3.5" rPi 16 bit TFT to work. [Here](https://github.com/phylos51/DDScopeX/tree/main/DDScope/Adafruit_ILI9486_Teensy) and [here](https://github.com/phylos51/DDScopeX/blob/main/DDScope/display/Display.cpp)
- [Screen](https://github.com/phylos51/DDScopeX/tree/main/DDScope/screens) examples of Planets, Stars, and Messier object catalogs.
- Example of drawing a [full screen bitmap](https://github.com/phylos51/DDScopeX/blob/main/DDScope/display/Display.cpp) by reading SD on a Teensy4.1. ```Look for Display::drawPic(xxx)```
- Code example for a [State Machine](https://github.com/phylos51/DDScopeX/blob/main/DDScope/screens/AlignScreen.cpp) . Also, examples of printing bitmapped icons and using them as a button.
- Examples of how to print [icons](https://github.com/phylos51/DDScopeX/tree/main/DDScope/display) and use them as buttons.

## License

DDScopeX is open source free software, licensed under the GPL.  
See [LICENSE.txt](./LICENSE.txt) file.

