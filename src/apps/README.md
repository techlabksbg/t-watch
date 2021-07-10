# How to write your first app
The easiest way to write your app is in C++. You will need:

 - An idea what your app should do
 - A nice icon with transparency, 72 by 72 pixels.

 Let's go through it with a sample app, a shopping list.

 ## Create a folder for the app
 Under src/apps/demoapps
 
    mkdir shoppinglist

 ## Find an icon and create a .h file out of it
 For our shopping list app I'd like to have a shopping cart as icon. Luckily, there is this
 very nice site, https://openmoji.org which lets me use the icons if I give credit and share alike.
 That's fair for me!

 I found this nice icon, let's download it as png - from the shoppinglist folder:

    curl "https://openmoji.org/php/download_asset.php?type=emoji&emoji_hexcode=1F6D2&emoji_variant=color" -o shoppinglist.png

Resize to 72 by 72 pixels in your favorite image editor / command line tool. For me it's

    sips -Z 72 shoppinglist.png

But ImageMagick's convert works as well. Add to git already, just in case you want to go back to it.

To convert to a .h file, use https://lvgl.io/tools/imageconverter - there is a small command line utility on the top level of our repository:

    ./../../../imageconverter/lv_img_conv.bash shoppinglist.png

This should have created a shoppinglist.c in the same directory.

