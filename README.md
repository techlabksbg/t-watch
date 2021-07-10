# t-watch


## Configurations
The platformio file has two configurations, ttgo-t-watch and ttgo-t-watch-macos. You can
choose which configuration is used by selecting it in the bottom bar of VS Code (towards env:...).

This is due to the different naming of the device that the watch gets recognized as ( /dev/ttyUSB0 for
Linux, /dev/tty.usbserial-... on Mac). So to use it on the Mac, set the environment variable PLATFORMIO_UPLOAD_PORT to your device port, for example as follows:

    launchctl setenv PLATFORMIO_UPLOAD_PORT /dev/tty.usbserial-0233D4FC

To make Visual Studio know about this when running MacOS, create a file environment.plist in
~/Library/LaunchAgents/ [Documentation on how these environment variables work](https://developer.apple.com/forums/thread/74371)

