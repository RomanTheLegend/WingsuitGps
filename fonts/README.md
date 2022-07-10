TFT_eSPI library uses vlw font files that are converted to a byte array. It's a bit of a pain to convert OTF/TTF to that format, but it probably saves memory because you don't need any other libs.

On the other hand the bigger font, the more memory it takes, so this approach needs more analysis. Maybe switching to https://lvgl.io would be justified afterall.


Adding new fonts:

1) Download and install the font in your system
2) Clone this tool: https://gitlab.com/armogur/ttf-to-vlw
3) In `src/main/java/it/armogur/fontconverter/FontConverter.java` change font name at line 64 and font size at line 67
4) `gradlew --no-daemon installDist`  (probably you'll have to play around with dependencies a bit though)
5) `build/install/ttf-to-vlw/bin/ttf-to-vlw`
6) Copy `date-font.h` it produced to `fonts` folder of this project


Alternatively, if you already have vlw font you may convert it to .h file here:

https://tomeko.net/online_tools/file_to_hex.php?lang=en