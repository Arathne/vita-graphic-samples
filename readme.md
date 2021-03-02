# Vita Rendering Samples

## Prerequisites

In order to build samples, you need [vitasdk](https://vitasdk.org) installed in your system.

## Building

Every sample includes a CMakeLists.txt file. Go to the directory of the sample and run `cmake . && make` to build it.

## Running

Once the sample has been built, there should be a vpk file. Send the file to your vita using ftp or usb and install it.

## List

### VitaSDK

* `input`: detect user button presses. (warning: only detects one button at a time)
* `io`: read and write file
* `launch`: start a different application or game
* `time`: manage time using vitasdk

### SDL2

* `basic`: draws a rectangle to the screen.
* `crop`: draws part of an image to the screen.
* `image`: draws entire image to the screen.
* `loop`: moves a rectangle from left side of the screen to the right
* `text`: custom font

### vitaGL

* `color`: single array to draw triangle with colors
* `color2`: separate arrays to draw a triangle with colors
* `texture`: draw texture with glBegin()
* `triangle`: draws a triangle using glBegin()
* `vbo`: vertex buffer object to draw triangle
* `indexing`: 4 vertices to draw rectangle

## Notes

* SDL2 vpk size is much larger than vitaGL
* writing files is to be done in ux0:data folder
