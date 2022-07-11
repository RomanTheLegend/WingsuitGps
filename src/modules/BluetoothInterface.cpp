#include "BluetoothInterface.hpp"

BluetoothSerial SerialBT;

namespace BluetoothInterface {

void init(){
  Serial.begin(115200);
  SerialBT.begin("G-HUD"); //Bluetooth device name
  Serial.println("Bluetooth interface initiated");
}

void sendData(String dataString){
    char dataArray[dataString.length()];
    dataString.toCharArray(dataArray, dataString.length());
    if (SerialBT.available()) {
        SerialBT.write( *dataArray);
    }
}

void sendData(char c ){
    //Serial.write(c);
    SerialBT.write(c);
}

}