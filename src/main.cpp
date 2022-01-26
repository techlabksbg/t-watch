/**
 * @author Ivo Blöchliger
 */

//#include "BluetoothSerial.h"

// For some obscure reason, the initialization must be early. It fails in the app...
//BluetoothSerial bt;

#include "os/os.h"

void setup() {
//    bt.begin("btclient", true);
    //bt.end();  makes connection fail
    os_setup();
}

void loop() {
    os_loop();
}
