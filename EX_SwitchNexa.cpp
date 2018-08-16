//******************************************************************************************
//  File: EX_SwitchNexa.cpp
//  Authors: Kristian Andrews, based on work of Dan G Ogorchock & Daniel J Ogorchock (Father and Son)
//
//  Summary:  EX_SwitchNexa is a class which implements the SmartThings "Switch" device capability.
//        It inherits from the st::Executor class.
//
//        Create an instance of this class in your sketch's global variable section
//        For Example:  st::EX_SwitchNexa executor1("switch1", RF_PIN, 2848576, false, 1, 1);
//
//        st::EX_SwitchNexa() constructor requires the following arguments
//        - String &name - REQUIRED - the name of the object - must match the Groovy ST_Anything DeviceType tile name
//        - byte pin - REQUIRED - the Arduino Pin that is connected to the 433 MHz ASK transmitter
//	  - int RFCode - REQUIRED - The Nexa transmitter ID code, uniquely identifies the transmitter
//        - bool RFGroup - REQUIRED - Transmit a Group code or not
//	  - int RFChan - REQUIRED - The transmitter channel code (1-4)
//	  - int RFUnit - REQUIRED - The transmitter unit code (1-4)
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2015-01-03  Dan & Daniel   Original Creation
//    2018-08-30  Dan Ogorchock  Modified comment section above to comply with new Parent/Child Device Handler requirements
//    2018-08-14  K Andrews      Modified to work as a class for Nexa 433 MHz remotes
//
//
//******************************************************************************************

/*
 * Program to control Nexa 433 MHz remote control sockets/dimmers
 * Protocol information from: http://tech.jolowe.se/home-automation-rf-protocols/
 * 
 * Sent packet description:
 * S HHHH HHHH HHHH HHHH HHHH HHHH HHGO CCUU P
 * 
 * S = Sync bit.
 * H = The first 26 bits are transmitter unique codes, and it is this code that the receiver "learns" to recognise.
 * G = Group code. Set to 0 for on, 1 for off.
 * O = On/Off bit. Set to 0 for on, 1 for off.
 * C = Channel bits. 4 units per channel
 * Channel #1 = 11, #2 = 10, #3 = 01, #4 = 00.
 * U = Unit bits. Device to be turned on or off.
 * Unit #1 = 11, #2 = 10, #3 = 01, #4 = 00.
 * P = Pause bit.
 * 
 * For every button press several identical packets are sent. For my transmitters this was 5 or 8.
 * 
 * Each unique transmitter code can control up to 16 different devices plus a group code
 * Typical usage is:
 * 	Transmitter code: Number between 0 and 67108863
 * 	Group code: off(0)
 * 	Channel and Unit codes between 1-4, for example:
 * 		Channel: 1
 * 		Unit: 1
 * 
 * T = 250us
 * Sync bit is T high + 10T low
 * 1 bit is T high + T low
 * 0 bit is T high + 5T low
 * Pause bit is T high + 40T low
 * 
 * T  Duration (us)
 * 1  250
 * 5  1250
 * 10 2500
 * 40 10000
 */

#include "EX_SwitchNexa.h"

#include "Constants.h"
#include "Everything.h"

namespace st
{
//private

// 433 MHz transmit section
void EX_SwitchNexa::nexaTransmit(String sTransCode, boolean bGroup, boolean bOn, int iChan, int iUnit){
  
  Serial.println("Nexa Transmit");

  // Send the same signal 5 times
  for (int i=0;i<5;i++){
    sendSync();
    
    // Send unique transmitter code
    for (int j=0;j<26;j++){
      switch (sTransCode.charAt(j)){
      case '0':
        send0();
        break;
      case '1':
        send1();
        break;
      }
    }
  
    // Send Group
    if(bGroup) {
      send0();
    } else {
      send1();
    }
  
    // Send On/Off
    if(bOn){
      send0();
    } else {
      send1();
    }
  
    // Send channel bits
    switch (iChan){
    case 1:
      send1();
      send1();
      break;
    case 2:
      send1();
      send0();
      break;
    case 3:
      send0();
      send1();
      break;
    case 4:
      send0();
      send0();
    }
  
    // Send Unit
    switch (iUnit){
    case 1:
      send1();
      send1();
      break;
    case 2:
      send1();
      send0();
      break;
    case 3:
      send0();
      send1();
      break;
    case 4:
      send0();
      send0();
    }
  
    // Send Pause
    sendPause();
  }

  // Turn off LED
  //digitalWrite(LED_BUILTIN, LOW);
}

void EX_SwitchNexa::sendSync(){
  //Serial.println("sendSync");
  //Send HIGH signal
  digitalWrite(m_nPin, HIGH);     
  delayMicroseconds(usDelay);

  //Send LOW signal
  digitalWrite(m_nPin, LOW);
  delayMicroseconds(10*usDelay);
}

void EX_SwitchNexa::send1(){
  //Serial.println("send1");
  //Send HIGH signal
  digitalWrite(m_nPin, HIGH);     
  delayMicroseconds(usDelay);

  //Send LOW signal
  digitalWrite(m_nPin, LOW);
  delayMicroseconds(usDelay);

    //Send HIGH signal
  digitalWrite(m_nPin, HIGH);     
  delayMicroseconds(usDelay);

  //Send LOW signal
  digitalWrite(m_nPin, LOW);
  delayMicroseconds(5*usDelay);
}

void EX_SwitchNexa::send0(){
  //Serial.println("send0");
  //Send HIGH signal
  digitalWrite(m_nPin, HIGH);     
  delayMicroseconds(usDelay);

  //Send LOW signal
  digitalWrite(m_nPin, LOW);
  delayMicroseconds(5*usDelay);

  //Send HIGH signal
  digitalWrite(m_nPin, HIGH);     
  delayMicroseconds(usDelay);

  //Send LOW signal
  digitalWrite(m_nPin, LOW);
  delayMicroseconds(usDelay);
}

void EX_SwitchNexa::sendPause(){
  //Serial.println("sendPause");
  //Send HIGH signal
  digitalWrite(m_nPin, HIGH);     
  delayMicroseconds(usDelay);

  //Send LOW signal
  digitalWrite(m_nPin, LOW);
  delayMicroseconds(40*usDelay);
}
  
  void EX_SwitchNexa::writeStateToPin()
  {
    nexaTransmit(m_sCode,m_bGroup,m_bCurrentState,m_iChan,m_iUnit);
  }


//public
  //constructor
  EX_SwitchNexa::EX_SwitchNexa(const __FlashStringHelper *name, byte pin, int RFCode, bool RFGroup, int RFChan, int RFUnit) :
    Executor(name),
    m_bGroup(RFGroup),
    m_iChan(RFChan),
    m_iUnit(RFUnit)
  {
    setRFCode(RFCode);
    setPin(pin);
  }

  //destructor
  EX_SwitchNexa::~EX_SwitchNexa()
  {
  
  }
  
  void EX_SwitchNexa::init()
  {
    Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH ? F("on") : F("off")));
  }

  void EX_SwitchNexa::beSmart(const String &str)
  {
    String s=str.substring(str.indexOf(' ')+1);
    if (st::Executor::debug) {
      Serial.print(F("EX_Switch2::beSmart s = "));
      Serial.println(s);
    }
    if(s==F("on"))
    {
      m_bCurrentState=HIGH;
    }
    else if(s==F("off"))
    {
      m_bCurrentState=LOW;
    }
    
    writeStateToPin();
    
    Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH?F("on"):F("off")));
  }
  
  void EX_SwitchNexa::refresh()
  {
    Everything::sendSmartString(getName() + " " + (m_bCurrentState == HIGH?F("on"):F("off")));
  }
  
  void EX_SwitchNexa::setPin(byte pin)
  {
    m_nPin=pin;
    pinMode(m_nPin, OUTPUT);
    writeStateToPin();
  }

  void EX_SwitchNexa::setRFCode(int iCode)
  {
    //Convert number to binary string
    m_sCode = String(iCode,BIN);

    // Pad string to 26 bits
    int iLen = 26 - m_sCode.length();
    
    String sFill = "";
    
    for (int i = 0; i < iLen; i++){
      sFill = "0" + sFill;
    }
    
    m_sCode = sFill + m_sCode;
  }
}