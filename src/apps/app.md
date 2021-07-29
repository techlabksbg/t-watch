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

### Prevent double-tap from hiding your app
In your `.h`-file add the following:

```c++
virtual bool processDoubleTap(){ return true; } // Do not hide app on double tap
```

You may even implement other things to react to double tapping. But then preferably 
implement the method in your `.cpp`-file.

#### Examples
See the file [drawing.h](demoapps/drawing/drawing.h).


