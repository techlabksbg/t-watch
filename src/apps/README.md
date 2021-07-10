# How to write your first app
The easiest way to write your app is in C++. You will need:

 - An idea what your app should do
 - A nice icon with transparency, 72 by 72 pixels.

 Let's go through it with a sample app, a shopping list.

 ## Create a folder for the app
 Under src/apps/demoapps
 
    mkdir shoppinglist

 ## Find an icon and create a icon.c file out of it
 For our shopping list app I'd like to have a shopping cart as icon. Luckily, there is this
 very nice site, https://openmoji.org which lets me use the icons if I give credit and share alike.
 That's fair for me!

 I found this nice icon, let's download it as png - from the shoppinglist folder:

    curl "https://openmoji.org/php/download_asset.php?type=emoji&emoji_hexcode=1F6D2&emoji_variant=color" -o shoppinglisticon.png

Resize to 72 by 72 pixels in your favorite image editor / command line tool. For me it's

    sips -Z 72 shoppinglisticon.png

But ImageMagick's convert works as well. Add to git already, just in case you want to go back to it.

To convert to a .h file, use https://lvgl.io/tools/imageconverter - there is a small command line utility on the top level of our repository:

    ./../../../imageconverter/lv_img_conv.bash shoppinglisticon.png

This should have created a shoppinglisticon.c in the same directory. Add that file, and add an entry
in about_icons.txt mentioning the author of the nice icon you're using.

## Your main application code
Create two files, shoppinglist.h and shoppinglist.cpp

Your shoppinglist.h file contains the methods your app *should* implement, even if it doesn't yet:

    #pragma once

    #include "../../app.h"

    LV_IMG_DECLARE(shoppinglisticon);


    class ShoppingList : public App {

        virtual bool create();
        virtual bool show();
        virtual bool hide() { return true;};
        virtual bool destroy() { return true;}
        
        void* getIcon() { return (void*) &shoppinglisticon; }
        const char * getName() { return "ShoppingList"; }
    };

Note the following elements:
  - we are declaring the icon with the LV_IMG_DECLARE macro
  - we are returning the name of the app

In your shoppinglist.cpp file, you're putting the actual behavior. Initially, it just contains
a trace command for app creation and showing the app:

    #include "shoppinglist.h"

    bool ShoppingList::create() {
        register_for_swipe_up(myScr);
        lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);

        Serial.println("Creating shopping list");

        return true;
    }

    bool ShoppingList::show() {
        Serial.println("Showing shopping list");

        return true;
    }
