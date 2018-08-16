//******************************************************************************************
//  File: EX_SwitchNexa.h
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
#ifndef ST_EX_SWITCHNEXA
#define ST_EX_SWITCHNEXA

#include "Executor.h"

namespace st
{
	class EX_SwitchNexa: public Executor
	{
		private:
			bool m_bCurrentState;	//HIGH or LOW
			byte m_nPin;		//Arduino Pin used to transmit the 433 MHz signal
			String m_sCode;		//The binary ID code of the transmitter
			bool m_bGroup;		//Whether the code is a group code or not
			int m_iChan;		//Channel ID
			int m_iUnit;		//Unit ID
			
			static const int usDelay = 250;  // Basic time unit of the transmission

			void nexaTransmit(String sTransCode, boolean bGroup, boolean bOn, int iChan, int iUnit);
			void sendSync();
			void send1();
			void send0();
			void sendPause();
			void writeStateToPin();	//function to update the Arduino Digital Output Pin
		
		public:
			//constructor - called in your sketch's global variable declaration section
			EX_SwitchNexa(const __FlashStringHelper *name, byte pin, int RFCode, bool RFGroup, int RFChan, int RFUnit);
			
			//destructor
			virtual ~EX_SwitchNexa();

			//initialization routine
			virtual void init();

			//SmartThings Shield data handler (receives command to turn "on" or "off" the switch (digital output)
			virtual void beSmart(const String &str);
			
			//called periodically to ensure state of the switch is up to date in the SmartThings Cloud (in case an event is missed)
			virtual void refresh();
			
			//gets
			virtual byte getPin() const {return m_nPin;}
			
			virtual bool getStatus() const { return m_bCurrentState; }	//whether the switch is HIGH or LOW

			//sets
			virtual void setPin(byte pin);
                        virtual void setRFCode(int iCode);
		
	};
}

#endif