# Useful methods for apps
The [App](app.h) class provides multiple methods to make your life easier.

## Mandatory methods
You have to implement the following methods:
- `create`
- `show`
- `hide`
- `destroy`

## App life cycle
### Instanciation at boot
At bootup, your app will be instanciated, but nothing more. If your app is a watch, you should implement a
constructor (also directly in you .h-file) as follows:

```c++
class YourCollWatch: public App {
    public:
    YourCoolWatch() {
        appType = TYPE_WATCH;
    }
    //  ---snip --- //
}
```
### Create at launch
When your app is first launched, the `create`-method is called. Typically you build the UI (with `myScr` as the parent object) and do smaller memory allocations here. The app is not shown yet, so don't start the loop task yet. 

The `create`-method has to return true. Otherwise the app will never be launched (it is treated like an unrecoverable error).

Do **never** open or close any app (including yours) inside this method.

### Show at (re)launch
Now it's time for bigger memory allocations and starting the loop task (if necessary).

The `show`-method has to return true. Otherwise the app will never be launched (it is treated like an unrecoverable error).

Do **never** open or close any app (including yours) inside this method.

### Hide
Your app will be hidden. Stop the loop taks (if any) and free all memory allocated in the `show`-method.

The `hide`-method has to return true. Otherwise the app will never be launched (it is treated like an unrecoverable error).

Do **never** open or close any app (including yours) inside this method.


### Destroy
Completely clean up all allocated memory. All allocated lv-objects depending on myScr will be cleaned up automatically after this method.
For now, this method will never be called, but it might be called in the future when memory is low.

The `destroy`-method has to return true. Otherwise the app will never be launched (it is treated like an unrecoverable error).

Do **never** open or close any app (including yours) inside this method.

## Optional methods
### Styles and common UI-Elements
There are several styles and commond UI-Elements defined in [styles.h](styles/styles.h).
- `styles.stdPage(myScr)` returns a vertically scrollable page with the standard background. Put all your GUI-elements on top of this page.
- `styles.stdBgImage(myScr)` returns an image with the standard background (for non-scrollable contents).
- `styles.stdLabel(lv_obj_t* parent, const char* text)` returns a standard label with the given text.
- `stdButton(lv_obj_t* parent, const char* text, lv_event_cb_t callback=nullptr, void* user_data=nullptr)` returns a standard Button with the given Text as label. You can optionally pass a callback for ui-events and set the userdata (typically `this` as a reference to the current app). This can be used as follows:
```c++
    playButton = styles.stdButton(bg, "Sound!", [](lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ((SpiffsAudio*)(button->user_data))->play_button();
    }, this);
```
See [spiffsaudio.cpp](demoapps/spiffsaudio/spiffsaudio.cpp) for Details.
- `styles.stdTitle(lv_obj_t* parent, const char* text)` returns a label with bigger text for titles.
- `styles.showSpinner(lv_obj_t* scr, const char* text = nullptr)` shows a spinner on top of typically `myScr`, with optional text. See [wifimanager.cpp](settingapps/wifimanager/wifimanager.cpp) for a possible usage.
- `styles.hideSpinner()` hides the spinner.
- `styles.messageBox.show(char* text, const char** buttons, std::function<void (uint16_t )> cb)` shows a message box
and calls the callback function with the result. Use it as follows (example from [wifimanager.h](settingapps/wifimanager/wifimanager.h))
```c++
    static const char* btns[] = {"Cancel", "Ok", ""};  // Last string must be empty!
    styles.messageBox.show(buf, btns, [delButton](uint16_t btnNr)->void {
        Serial.printf("messageBox callback called with btn=%d\n", btnNr);
        if (btnNr==1) { // Only when OK
            self->removeSSID(delButton);
        }
    });
```
#### Examples
Well, look at the code of the app which is most similar to what you need to do.

### Starting and stopping the loop task
In your `.h`-file, declare the following method:
```c++
    virtual void loop();
```
and implement it in your `.cpp`-file.


In your `.cpp`-file in your `show`-method call
```c++
    start_loop(1000);
```
to call your `loop`-method every 1000 milliseconds. In your `hide`-method call
```c++
    stop_loop();
```
to stop you `loop`-method from beeing called.

#### Examples
See the files [techlab.h](watches/techlabwatch/techlabwatch.h) and [techlab.cpp](watches/techlabwatch/techlabwatch.cpp).

The loop-feature is implemented in [app.h](app.h) and [app.cpp](app.cpp).

### Prevent double-tap from hiding your app
In your `.h`-file add the following:

```c++
virtual bool processDoubleTap(){ return true; } // Do not hide app on double tap
```

You may even implement other things to react to double tapping. But then preferably 
implement the method in your `.cpp`-file.

#### Examples
See the file [drawing.h](demoapps/drawing/drawing.h).

The callback is managed in [launcher.h](launcher/launcher.h), the interrupt handled in [loop.cpp](os/loop.cpp).

### Setting an alarm
Your app can set an alarm by calling
```c++
setAlarm(int hours, int minutes)
```

You have to implement the `processAlarm()`-method, called when the alarm fired. The `show`-method will be called just before, if necessary.


#### Examples
See the files [simplealarm.h](demoapps/simplealarm/simplealarm.h) and [simplealarm.h](demoapps/simplealarm/simplealarm.cpp).

The callback is managed in [launcher.h](launcher/launcher.h).


### Requesting WiFi connection
Outside the `create`- and `show`-methods (typically from a UI-event) you can call
```c++
wifimanager.connect(this);
```
This will open the wifimanager to establish the latest connection and if successful, will `show` your app again.
Otherwise, the user chooses manually a WiFi-Connection and has to restart your app manually.

You may also use the following async call inside the `show`-method to automatically start the wifimanager:
```c++
    if (!WiFi.isConnected()) {
        lv_async_call([](void *userdata) {
            wifiManager.connect((App*)userdata);
        }, this);
    } else  {
        // set up whatever you need wifi for
    }
```

#### Examples
See the files [simplealarm.h](demoapps/webaudio/webaudio.h) and [simplealarm.h](demoapps/webaudio/webaudio.cpp).

WiFi is managed in [wifimanager.h](settingapps/wifimanager/wifimanager.h) and [wifimanager.cpp](settingapps/wifimanager/wifimanager.cpp).

### Download a file to SPIFFS if it not already there
Your app might need some data files on the internal storage (SPIFFS). You can download these files
from an URL.

First include `#include "services/services.h"`, located in the root of the project.

Then in the `show`-method:
```c++
bool SpiffsAudio::show() {
    // This is the root certificate for the following website. Check the site you want to download from.
    #include "../../certs/ISRG_Root_X1.h"
    // The Filename needs a leading slash, for example "/test.mp3"
    // You might also use http without the certificate, if the website supports it.
    // The file will only be downloaded, if it does not exist yet.
    if (downloadToSPIFFS(this, "/upchime2.mp3", "https://tech-lab.ch/twatch/upchime2.mp3", root_ca)) {
        // Do whatever you need to do, once the file exists.
    }
    return true;
}
```
If the file does not exist, your app will be hidden, wifi started and the file downloaded. Then your app
will be shown again.

#### Examples
See [spiffsaudio.cpp](demoapps/spiffsaudio/spiffsaudio.cpp).

To see how it works under the hood see [downloadtospiffs.cpp](../services/downloadtospiffs.cpp).
