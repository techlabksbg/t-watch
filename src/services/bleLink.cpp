#include "bleLink.h"
#include "Arduino.h"

// static
BLE_Link* BLE_Link::getInstance() {
  if (singletonInstance==nullptr) {
    singletonInstance = new BLE_Link();
  }
  return singletonInstance;
}

// Static:
void BLE_Link::scanCompleteCB(BLEScanResults bleScanResults) {
  Serial.println("Scan ist complete");
  if (singletonInstance->state==SCANNING_FOR_SERVER && singletonInstance->myDevice==nullptr) { // No server found
    singletonInstance->startServer();
  }

}

// static!
void BLE_Link::notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)
{
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  Serial.print("data: ");
  Serial.println((char*)pData);
  singletonInstance->bleLinkCallbacks->bleLinkNotify();
  
}

bool BLE_Link::write(std::string message) {
  if (state == CONNECTED_AS_CLIENT) {
    pRemoteCharacteristic->writeValue(message.c_str(), message.length());
    return true;
  } else if (state == CONNECTED_AS_SERVER) {
    pCharacteristic->setValue(message.c_str());
    return true;
  }
  return false;
}

std::string BLE_Link::read() {
  if (state == CONNECTED_AS_CLIENT) {
    return pRemoteCharacteristic->readValue();
  } else if (state == CONNECTED_AS_SERVER) {
    return pCharacteristic->getValue();
  }
  return std::string("");
}

void BLE_Link::startServer() {
  BLEDevice::init((name+"_server").c_str());
  pServer = BLEDevice::createServer();
  pService = pServer->createService(*serviceUUID);
  pCharacteristic = pService->createCharacteristic(
    *charUUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setValue("S:Ready");
  pCharacteristic->setCallbacks(new MyCharacteristicCallback(this));
  pService->start();
  //BLEAdvertising *pAdvertising = pServer->getAdvertising();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(*serviceUUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  //pAdvertising->start();
  Serial.println("Characteristic defined! Now you can read it in the Client!");
}                       


void BLE_Link::begin(std::string connectionName, 
              const char* serviceUUID, 
              const char* characteristicUUID,
              BLE_Link_Callbacks* callbacks) {
    bleLinkCallbacks = callbacks;
    this->serviceUUID = new BLEUUID(serviceUUID);
    this->charUUID = new BLEUUID(characteristicUUID);   
    state = SCANNING_FOR_SERVER;
    myDevice = nullptr; 
    name = connectionName;
    BLEDevice::init((name+"_client").c_str());
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
    pBLEScan->setInterval(300);
    pBLEScan->setWindow(250);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(2, scanCompleteCB);
}

bool BLE_Link::connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());
    
  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback(this));

    /* Connect to the remote BLE Server */
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");

    /* Obtain a reference to the service we are after in the remote BLE server */
  BLERemoteService* pRemoteService = pClient->getService(*serviceUUID);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID->toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");


  /* Obtain a reference to the characteristic in the service of the remote BLE server */
  pRemoteCharacteristic = pRemoteService->getCharacteristic(*charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID->toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  /* Read the value of the characteristic */
  /* Initial value is 'Hello, World!' */
  if(pRemoteCharacteristic->canRead())
  {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }

  if(pRemoteCharacteristic->canNotify())
  {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }
  return true;
}

void BLE_Link::MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
    /* We have found a device, let us now see if it contains the service we are looking for. */
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(*(bleLink->serviceUUID))) {
        bleLink->myDevice = new BLEAdvertisedDevice(advertisedDevice);
        bleLink->state = CONNECTING_AS_CLIENT;
        Serial.println("Stopping scan");
        BLEDevice::getScan()->stop();
        if (bleLink->connectToServer()) {
          bleLink->state = CONNECTED_AS_CLIENT;
        } else {
          bleLink->state = CONNECTION_TO_SERVER_FAILED;
        }
    }
}



void BLE_Link::MyClientCallback::onDisconnect(BLEClient* pclient) {
    bleLink->connected = false;
    Serial.println("BLE_Link::MyClientCallback::onDisconnect");
}
void BLE_Link::MyClientCallback::onConnect(BLEClient* pclient) {
    bleLink->connected = true;
    Serial.println("BLE_Link::MyClientCallback::onConnect");
}


void BLE_Link::MyCharacteristicCallback::onNotify(BLECharacteristic* pCharacteristic) {
  Serial.print("Server notify callback for characteristic ");
  Serial.println(pCharacteristic->getUUID().toString().c_str());
  Serial.println(pCharacteristic->getValue().c_str());
  bleLink->bleLinkCallbacks->bleLinkNotify();
}

BLE_Link* BLE_Link::singletonInstance = nullptr;