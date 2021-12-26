// Inspired by https://www.electronicshub.org/esp32-ble-tutorial/

#include "BLEDevice.h"


class BLE_Link_Callbacks;

class BLE_Link {
public:
    static BLE_Link* getInstance();
    void begin(std::string name, const char* serviceUUID, const char* characteristicUUID, BLE_Link_Callbacks* callbacks);
    enum linkState {NOT_INITIALIZED, SCANNING_FOR_SERVER, CONNECTING_AS_CLIENT, CONNECTED_AS_CLIENT, CONNECTION_TO_SERVER_FAILED, CONNECTED_AS_SERVER, DISCONNECTED} state = NOT_INITIALIZED;
    bool write(std::string message);
    std::string read();

private:
    BLE_Link() {}

    BLE_Link_Callbacks* bleLinkCallbacks;
    BLERemoteCharacteristic* pRemoteCharacteristic = nullptr;
    BLEAdvertisedDevice* myDevice = nullptr;
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pCharacteristic;
    BLEUUID* serviceUUID;
    BLEUUID* charUUID;
    bool connected = false;
    std::string name;

    static BLE_Link* singletonInstance;

    bool connectToServer();
    void startServer();

    static void scanCompleteCB(BLEScanResults bleScanResults);
    static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

    class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
        public:
        MyAdvertisedDeviceCallbacks(BLE_Link* bleLinkInstance): BLEAdvertisedDeviceCallbacks(), bleLink{bleLinkInstance} {}
        /* Called for each advertising BLE server. */
        void onResult(BLEAdvertisedDevice advertisedDevice);

        private:
        BLE_Link* bleLink;
    };

    class MyClientCallback : public BLEClientCallbacks {
        public:
        MyClientCallback(BLE_Link* bleLinkInstance): BLEClientCallbacks(), bleLink{bleLinkInstance} {}
        void onConnect(BLEClient* pclient);
        void onDisconnect(BLEClient* pclient);

        private:
        BLE_Link* bleLink;
    };

    // See https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/src/BLECharacteristic.h
    class MyCharacteristicCallback : public BLECharacteristicCallbacks {
        public:
        MyCharacteristicCallback(BLE_Link* bleLinkInstance): BLECharacteristicCallbacks(), bleLink{bleLinkInstance} {}
        virtual void onNotify(BLECharacteristic* pCharacteristic);

        private:
        BLE_Link* bleLink;
    };
  
};


class BLE_Link_Callbacks {
    public:
    virtual void bleLinkNotify()=0;

};