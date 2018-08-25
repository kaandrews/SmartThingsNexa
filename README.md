# SmartThingsNexa
Integrating Nexa 433 MHz switches into SmartThings using the ESP8266 NodeMCU

This library is to extend the capabilities of the ST Anything project to support controlling Nexa 433 MHz RF switches through the Samsung SmartThings ecosystem.

This library extends the work done in the repository below:
https://github.com/DanielOgorchock/ST_Anything

Follow the Readme file provided with the ST Anything library to get up and running before attempting to add this functionality.

Introduction:
The Samsung SmartThings hub can integrate devices through the use of community provided device handlers.
The ST Anything library allows integrating Arduino and Arduino like devices into SmartThings to control devices or monitor sensors.
The ESP8266 NodeMCU can be used to run the ST Anything code and can integrate with SmartThings over a WiFi network.

This add on library allows control of Nexa brand 433 MHz RF power sockets by connecting the NodeMCU to a cheap 433 MHz transmitter.

Nexa brand sockets are sold by Clas Ohlson in the UK:
https://www.clasohlson.com/uk/Nexa-Remote-Switch-Set-3-pack/18-2650

The transmitter I use is linked below as an example:
https://www.amazon.co.uk/gp/product/B00R2U8OEU

To use this library add the EX_SwitchNexa.h and EX_SwitchNexa.cpp files to your Arduino library.
The included Arduino sketch ST_Anything_Nexa433MHz_ESP8266WiFi.ino provides an example of how to use the library.

To use the RF Transmitter I connect as below:

NodeMCU D1  <---> RF Module DATA

+3.3v or 5v <---> RF Module VCC

Ground      <---> RF Module GND

To improve range I recommend to use an external power source for the RF module rather than drawing power from the 3.3v pin of the NodeMCU.  Make sure you don't connect the NodeMCU to a 5v supply, only to 3.3v.

As well as the RF Transmitter module I made the following connections to power the NodeMCU:

+3.3v     <---> NodeMCU 3V

Ground    <---> NodeMCU G

The Nexa protocol is described in the library file, for more details please refer to my other Github project:
https://github.com/kaandrews/Arduino-Nexa-Control

Each socket can learn up to 3 codes, so you can choose which of the following you prefer:
1) Use a new transmitter code and teach it to your sockets
2) Copy the code from your existing transmitter(s)

For option 2 you can use my Nexa_RX_Serial_v1.0 Arduino sketch and one of the receives shown in the Amazon link above.

I think option 2 is better for the following reasons:
1) I can control the switches with the remote and also with SmartThings
2) I haven't figure out how to un-program a switch with a new code, I think you need to press the off button twice which you can't do with my current code as it always toggles on and off.



Improvements to make:
The transmit code relies on delays to control the process, this will block the processing of other commands on the NodeMCU.
This may interefe with other functions of the NodeMCU such as monitoring a button press.
