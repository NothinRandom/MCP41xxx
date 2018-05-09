#include "MCP41xxx.h"

MCP41xxx::MCP41xxx(byte csPin)
{
	_cs = csPin; 
	pinMode(_cs, OUTPUT); 
	digitalWrite(_cs, HIGH);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);	
}

//POSSIBLE TO OPERATE MCP41xxx in MODE3 from page 22
	//interrupt disabled, spi enabled, msb 1st, master, clk low when idle,
	//sample on leading edge of clk, system clock/4
void MCP41xxx::begin()
{
	SPCR = (1<<SPE)|(1<<MSTR); //enable spi, master, mode0
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)||(1<<CPHA)|(1<<SPR0); //enable spi, master, mode3
	
	// digitalWrite(SCK, HIGH);
	// digitalWrite(MOSI, HIGH);	
}

void MCP41xxx::init()
{
	prevValue = 127; //default already
	setValue(prevValue);  //0-255
}

void MCP41xxx::init(uint8_t shdn, uint8_t rs)
{
	_shdn = shdn;
	_rs = rs;

	pinMode(_shdn, OUTPUT); 
	pinMode(_rs, OUTPUT); 
	digitalWrite(_shdn,HIGH); 
	digitalWrite(_rs, HIGH); 
	prevValue = 127;
}

//setValue for MCP41xxx
void MCP41xxx::setValue(byte value) //0-255
{
	digitalWrite(_cs, LOW);
    //b00010001 = 18d  MCP41xxx 0x12
    //b00010011 = 19d  MCP42xxx	0x13
    //b00cc00pp   //cc comand bytes, pp potentiometer selection
	transfer(0x13); //write config 
	transfer(value); //write value
	digitalWrite(_cs, HIGH);
    prevValue = value;
} 

//return the previous set value
byte MCP41xxx::getValue()
{
	return prevValue; 
}

byte MCP41xxx::transfer(volatile byte data)
{
	begin();
	SPDR = data;
	while (!(SPSR & (1<<SPIF))); //wait for transfer to complete
	return SPDR;
}

//setValue for MCP42xxx
void MCP41xxx::setValue(byte value, int potentiometer)
{
	byte settings;
	digitalWrite(_cs, LOW);
	if (potentiometer == 0) 
		settings = B00010001; //pot0
	else if (potentiometer == 1) 
		settings = B00010010; //pot1
	else 
		settings = B00010011; //both pots	
	transfer(settings); //write config 
	transfer(value); //write value
	digitalWrite(_cs, HIGH);
	prevValue = value;
} 

//shutdown MCP42xxx
void MCP41xxx::shutdown(boolean data)
{
   digitalWrite(_shdn, data); 
}

//reset MCP42xxx
void MCP41xxx::reset()
{
	digitalWrite(_rs, LOW); 
	delay(1);
	digitalWrite(_rs, HIGH); 
}