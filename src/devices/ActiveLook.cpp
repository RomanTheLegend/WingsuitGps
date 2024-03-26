#pragma once
#include <NimBLEDevice.h>
#include <math.h>
#include <vector>
#include <string>
#include "../config.h"
#include "DisplayDevice.hpp"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define DEVICE_ADDRESS "80:21:30:04:94:9a"

// UUID of the service and characteristic
#define SERVICE_UUID "0783B03E-8535-B5A0-7140-A304D2495CB7"
#define CHARACTERISTIC_UUID "0783B03E-8535-B5A0-7140-A304D2495CBA"

#define FRAME_HEADER 0xFF
#define FRAME_FOOTER 0xAA
#define FRAME_FMT_LEN_2BYTES 0x10
#define FRAME_W 304
#define FRAME_H 256

class MyBLEClientCallbacks : public NimBLEClientCallbacks
{
  void onConnect(NimBLEClient *pClient)
  {
    Serial.println("Connected to server");
  }

  void onDisconnect(NimBLEClient *pClient)
  {
    Serial.println("Disconnected from server");
  }
};

class ActiveLookDisplay : public DisplayDevice
{

private:
  NimBLERemoteCharacteristic *pRemoteCharacteristic;
  NimBLEClient *pClient;
  bool connected = false;
  const char *LOG_TAG = "ActiveLookDevice";

  uint8_t *uShortToList(short value)
  {
    uint8_t *bt = new uint8_t[2];
    bt[0] = (value >> 8) & 0xFF;
    bt[1] = value & 0xFF;
    return bt;
  }

  uint8_t *strToList(std::string str, int maxLen)
  {
    uint8_t *lst = new uint8_t[str.length() + 1];
    for (int i = 0; i < str.length(); i++)
    {
      lst[i] = str[i];
    }
    if (maxLen == -1 || str.length() < maxLen)
    {
      lst[str.length()] = 0;
    }
    return lst;
  }

  void img(short id, int x, int y, std::vector<uint8_t> *payload)
  {

    uint8_t *xBytes = uShortToList(x);
    uint8_t *yBytes = uShortToList(y);

    (*payload)[0] = id;
    (*payload)[1] = xBytes[0];
    (*payload)[2] = xBytes[1];
    (*payload)[3] = yBytes[0];
    (*payload)[4] = yBytes[1];

    delete[] xBytes;
    delete[] yBytes;
  }

  void text(int x0, int y0, int rot, int font, int color, std::string str, std::vector<uint8_t> *payload)
  {

    uint8_t *x0Bytes = uShortToList(x0);
    uint8_t *y0Bytes = uShortToList(y0);

    (*payload)[0] = x0Bytes[0];
    (*payload)[1] = x0Bytes[1];
    (*payload)[2] = y0Bytes[0];
    (*payload)[3] = y0Bytes[1];
    (*payload)[4] = rot;
    (*payload)[5] = font;
    (*payload)[6] = color;

    uint8_t *strBytes = strToList(str, -1);
    for (int i = 0; i < str.length(); i++)
    {
      (*payload)[7 + i] = strBytes[i];
    }
    delete[] x0Bytes;
    delete[] y0Bytes;
    delete[] strBytes;
  }

  std::vector<uint8_t> formatFrame(uint8_t cmdId)
  {
    std::vector<uint8_t> payload;
    std::vector<uint8_t> queryId;
    return formatFrame(cmdId, &payload, &queryId);
  }

  std::vector<uint8_t> formatFrame(uint8_t cmdId, std::vector<uint8_t> *payload)
  {
    std::vector<uint8_t> queryId;
    return formatFrame(cmdId, payload, &queryId);
  }

  std::vector<uint8_t> formatFrame(uint8_t cmdId, std::vector<uint8_t> *payload, std::vector<uint8_t> *queryId)
  {
    uint8_t payloadSize = payload->size();
    uint8_t querySize = queryId->size();
    int frameLen = 5 + payloadSize + querySize; // 5 = header + cmdId + fmt + len + footer
    int lenNbByte = 1;
    if (frameLen > 0xFF)
    {
      frameLen += 1;
      lenNbByte = 2;
    }

    std::vector<uint8_t> frame(frameLen); // Adjust the size according to your requirement
    int idx = 0;
    frame[idx++] = FRAME_HEADER; // header
    frame[idx++] = cmdId;        // cmdId
    if (lenNbByte == 1)
    {
      uint8_t fmt = querySize;
      frame[idx++] = fmt;      // fmt
      frame[idx++] = frameLen; // len
    }
    else
    {
      uint8_t fmt = FRAME_FMT_LEN_2BYTES | querySize;
      frame[idx++] = fmt; // fmt
      uint8_t *lenBytes = uShortToList(frameLen);
      frame[idx++] = lenBytes[0]; // len MSB
      frame[idx++] = lenBytes[1]; // len LSB
    }

    if (querySize > 0)
    {
      for (int i = 0; i < querySize; i++)
      {
        frame[idx++] = (*queryId)[i];
      }
    }

    for (int i = 0; i < payloadSize; i++)
    {
      frame[idx++] = (*payload)[i];
    }

    frame[idx++] = FRAME_FOOTER; // footer

    return frame;
  }

  void displayDemo()
  {
    std::string data = "Demo";
    std::vector<uint8_t> payload(data.length() + 8);
    text(200, 180, 4, 3, 1, data, &payload);
    const std::vector<uint8_t> command2 = formatFrame(0x37, &payload);
    pRemoteCharacteristic->writeValue(command2, false);
  }

  bool connect()
  {

    NimBLEAddress address(DEVICE_ADDRESS);
    try
    {
      pClient->connect(address);
    }
    catch (...)
    {
      return false;
    }

    ESP_LOGD(LOG_TAG, "Connected to ActiveLook device");

    NimBLEUUID serviceUUID(SERVICE_UUID);
    NimBLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
      ESP_LOGE(LOG_TAG, "Failed to find service");
      return false;
    }

    // Get the characteristic
    NimBLEUUID charUUID(CHARACTERISTIC_UUID);
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr)
    {
      ESP_LOGE(LOG_TAG, "Failed to find characteristic");
      return false;
    }
    return true;
  }

public:
  void init()
  {
    NimBLEDevice::init("");
    pClient = NimBLEDevice::createClient();

    pClient->setClientCallbacks(new MyBLEClientCallbacks());

    connected = connect();
  }

  void clearScreen()
  {
    if (connected){
      const uint8_t command[] = {0xFF, 0x01, 0x00, 0x05, 0xAA};
      pRemoteCharacteristic->writeValue(command, sizeof(command), false);
    } else{
      ESP_LOGE(LOG_TAG, "Activelook not connected");
    }
  }

  void setFontId(int id)
  {
  }

  void setColor(int id)
  {
    std::vector<uint8_t> payload(1);
    payload[0] = id;
    const std::vector<uint8_t> command = formatFrame(0x30, &payload);
    pRemoteCharacteristic->writeValue(command, false);
  }

  void displayIcon(int id, int x, int y)
  {
    std::vector<uint8_t> payload(5);
    img(id, FRAME_W - x, FRAME_H - y, &payload);
    const std::vector<uint8_t> command = formatFrame(0x42, &payload);
    pRemoteCharacteristic->writeValue(command, false);
  }

  void displayDigits(int current, int previous)
  {
    displayDemo();
  }

  void displayDigits(int current)
  {
    displayDemo();
  }

  void displayString(int num, int x, int y)
  {
    std::string data = std::to_string(num);
    displayString(data, x, y);
  }

  void displayString(char *string, int x, int y)
  {
    std::string data(string);
    displayString(data, x, y);
  }

  void displayString(std::string data, int x, int y)
  {
    displayString(data, 1, x, y);
   }

  void displayString(std::string data, int color, int x, int y)
  {
    displayString(data, 2, 1, x, y);
  }

  void displayString(std::string data, int size, int color, int x, int y)
  {
    if (connected){
      std::vector<uint8_t> payload(data.length() + 8);
      std::vector<uint8_t> queryId;
      text(FRAME_W - x, FRAME_H - y, 4, size, color, data, &payload);
      const std::vector<uint8_t> command = formatFrame(0x37, &payload, &queryId);
      pRemoteCharacteristic->writeValue(command, false);
    } else {
      ESP_LOGE(LOG_TAG, "Activelook not connected");
    }
  }

  void drawRect(int x, int y, int h, int w, int c)
  {
  }

  void drawLine(int x, int y, int x1, int y1, int c)
  {
  }

  void drawArrow(int cX, int cY, int angle)
  {
    int c = 2;
    int m = 3;
    int x1, x2, x3, x4, x5, x6, x7;
    int y1, y2, y3, y4, y5, y6, y7;

    double radians = angle * M_PI / 180.0;
    double sin_angle = sin(radians);
    double cos_angle = cos(radians);

    x1 = -2 * m;
    x2 = -2 * m;
    x3 = -4 * m;
    x4 = 0;
    x5 = +4 * m;
    x6 = +2 * m;
    x7 = +2 * m;

    y1 = +5 * m;
    y2 = -2 * m;
    y3 = -2 * m;
    y4 = -7 * m;
    y5 = -2 * m;
    y6 = -2 * m;
    y7 = +5 * m;

    double new_x1 = x1 * cos_angle - y1 * sin_angle + cX;
    double new_y1 = x1 * sin_angle + y1 * cos_angle + cY;
    double new_x2 = x2 * cos_angle - y2 * sin_angle + cX;
    double new_y2 = x2 * sin_angle + y2 * cos_angle + cY;
    double new_x3 = x3 * cos_angle - y3 * sin_angle + cX;
    double new_y3 = x3 * sin_angle + y3 * cos_angle + cY;
    double new_x4 = x4 * cos_angle - y4 * sin_angle + cX;
    double new_y4 = x4 * sin_angle + y4 * cos_angle + cY;
    double new_x5 = x5 * cos_angle - y5 * sin_angle + cX;
    double new_y5 = x5 * sin_angle + y5 * cos_angle + cY;
    double new_x6 = x6 * cos_angle - y6 * sin_angle + cX;
    double new_y6 = x6 * sin_angle + y6 * cos_angle + cY;
    double new_x7 = x7 * cos_angle - y7 * sin_angle + cX;
    double new_y7 = x7 * sin_angle + y7 * cos_angle + cY;

    // videoOut.drawLine(new_x1, new_y1, new_x2, new_y2, c);
    // videoOut.drawLine(new_x2, new_y2, new_x3, new_y3, c);
    // videoOut.drawLine(new_x3, new_y3, new_x4, new_y4, c);
    // videoOut.drawLine(new_x4, new_y4, new_x5, new_y5, c);
    // videoOut.drawLine(new_x5, new_y5, new_x6, new_y6, c);
    // videoOut.drawLine(new_x6, new_y6, new_x7, new_y7, c);
    // videoOut.drawLine(new_x7, new_y7, new_x1, new_y1, c);
  }

  void setRotation(int r)
  {
  }

  void waitForFrame()
  {
    // std::vector<uint8_t> payload({0});
    // std::vector<uint8_t> queryId;
    // const std::vector<uint8_t> command = formatFrame(0x37, &payload, &queryId);
    // pRemoteCharacteristic->writeValue(command, false);
  }

  void releaseFrame()
  {
    std::vector<uint8_t> payload({1});
    std::vector<uint8_t> queryId;
    const std::vector<uint8_t> command = formatFrame(0x37, &payload, &queryId);
    pRemoteCharacteristic->writeValue(command, false);
  }
};