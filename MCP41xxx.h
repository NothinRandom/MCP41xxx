#ifndef MCP41xxx_h
#define MCP41xxx_h

#include "Arduino.h"

class MCP41xxx
{
	public:
		MCP41xxx(byte csPin);
		void begin();	
		void init(); //for MCP41xxx
		void init(uint8_t shdn, uint8_t rs); //for MCP42xxx

		void setValue(uint8_t value);  //0-255
		void setValue(uint8_t value, int potentiometer); 

		void shutdown(boolean data);
		void reset();
		uint8_t getValue();
		uint8_t prevValue; //default to 127 as mid range

	private:
		uint8_t _cs;
		uint8_t _shdn;
		uint8_t _rs;
		uint8_t transfer(uint8_t data);
};

#endif
