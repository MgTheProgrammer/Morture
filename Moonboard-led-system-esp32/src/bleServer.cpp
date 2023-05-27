/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/
#include <bleServer.h>
#include <storage.h>

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

CommandParser parser = CommandParser();
std::string lastString = "";

#define MAX_WAIT_BUFFER pdMS_TO_TICKS(20)
MessageBufferHandle_t xMessageBuffer;
const size_t xMessageBufferSizeBytes = 1000; // The largest (talking abt memory) route possible should take (realistically) roughly 120 bytes. Therfore 1000 Bytes should hold comfortably around 10 routes which should be more than enough
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
uint8_t receivedBuffer[200];
size_t xReceivedBytes;
std::string receivedString = "";
int count = 0;
#define NUM_LEDS 198 // 18 rows on 11 (K) columns on a regular moonboard
#define DATA_PIN 4
CRGB leds[NUM_LEDS];
BoardVisual board;

void MyServerCallbacks::onConnect(BLEServer *pServer)
{
  deviceConnected = true;
  storage::addConnection();
}

void MyServerCallbacks::onDisconnect(BLEServer *pServer)
{
  deviceConnected = false;
}

void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  std::string rxValue = pCharacteristic->getValue();
  std::stringstream hexStream;
  std::string hexString;

  if (rxValue.length() > 0)
  {

    // convert from string to hex string
    for (int i = 0; i < rxValue.length(); i++)
    {
      hexStream << std::hex << int(rxValue[i]);
      if (int(rxValue[i]) == 35)
      {
        count++;
        if (count == 2)
        {
          break;
        }
      }
    }
    hexString = hexStream.str();

    Serial.println("*********");
    Serial.print("Received Value: ");
    Serial.println();
    for (int i = 0; i < hexString.length(); i++)
    {
      Serial.print(hexString[i]);
    }
    Serial.println();
  }

  lastString = lastString + hexString;

  if (lastString.rfind(parser.msgSuffix) == lastString.length() - parser.msgSuffix.length())
  {
    xMessageBufferSendFromISR(xMessageBuffer, lastString.c_str(), strlen(lastString.c_str()), &xHigherPriorityTaskWoken);
    Serial.printf("\nbuffer: \n%s", lastString.c_str());
    lastString = "";
    count = 0;
  }
}

void turnRouteOn(std::string routeString)
{
  FastLED.clear(); // deletes any prior data in the array to show
  board.clearBoard();
  int numHolds = parser.CreateHoldQueue(routeString);
  if (numHolds == -1)
  {
    return;
  }
  for (size_t i = 0; i < numHolds; i++)
  {
    Hold hold = *std::move(parser.getHold());
    Serial.printf("hold index %d: ", hold.index);
    if (hold.index > NUM_LEDS || hold.index < 0)
    { // sometimes a broken message is received and we need to handle it
      continue;
    }
    switch (hold.color)
    {
    case Color::red:
      leds[hold.index] = CRGB::Red;
      break;
    case Color::blue:
      leds[hold.index] = CRGB::Blue;
      break;
    case Color::green:
      leds[hold.index] = CRGB::Green;
      break;
    default:
      break;
    }
    storage::addColor(hold.color); // update the stats
    storage::addHold(hold.index);
    board.enterHoldAtIndex(hold);
  }
  board.printBoard();
  FastLED.show();
  storage::addRoute();
}

void bleServerSetup()
{
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  leds[0] = CRGB::Cornsilk;

  xMessageBuffer = xMessageBufferCreate(xMessageBufferSizeBytes);
  // checks that the buffer has been initialized if not will try to initialize buffer with less memory
  while (xMessageBuffer == NULL)
  {
    int i = 1;
    xMessageBufferCreate(xMessageBufferSizeBytes - i);
    i++;
  }

  // Create the BLE Device
  BLEDevice::init("Onn's moonboard");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  // Start advertising
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void bleServerMain(void *parameters)
{
  storage::setupGeneralStatsStorage();
  bleServerSetup();
  while (1)
  {
    if (deviceConnected)
    {
      pTxCharacteristic->setValue(&txValue, 1);
      pTxCharacteristic->notify();
      txValue++;
      delay(10); // bluetooth stack will go into congestion, if too many packets are sent
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
      delay(500);                  // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
    }
    if (!xMessageBufferIsEmpty(xMessageBuffer))
    {
      xReceivedBytes = xMessageBufferReceive(xMessageBuffer, (void *)receivedBuffer, sizeof(receivedBuffer), MAX_WAIT_BUFFER);
      if (xReceivedBytes > 0)
      {
        receivedString = std::string((char *)receivedBuffer, xReceivedBytes);
        Serial.println("\nrecived data from buffer:");
        Serial.println(receivedString.c_str());
        turnRouteOn(receivedString);
      }
    }
    delay(5); // allows the wdt to reset
  }
}
