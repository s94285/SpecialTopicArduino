#include <modbusSlave.h>
#include <modbusRegBank.h>
#include <modbusDevice.h>
#include <modbus.h>

modbusDevice regBank;
modbusSlave slave;
bool input[8];

#define RS485RxTxEnablePin 2
#define RS485Baud 9600
#define RS485Format SERIAL_8E1

void setup()
{
	for (int pin = 22; pin <= 29; pin++) {
		pinMode(pin, INPUT);
	}

	for (int pinOut = 31; pinOut <= 44; pinOut++) {
		pinMode(pinOut, OUTPUT);
		digitalWrite(pinOut, LOW);
	}
	regBank.setId(1);
	regBank.add(40001);

	slave._device = &regBank;

	slave.setBaud(&Serial3, RS485Baud, RS485Format, RS485RxTxEnablePin);	//modbusSlave.setBaud( Serial, BaudRate, Format, EnablePin);

}

long unsigned boolArrayToLong(bool boolArray[8]) {
	long unsigned value = 0;
	for (int j = 0; j < 8; j++) {
		value += boolArray[j] << j;
	}
	return value;
}

void loop()
{	
	int i = 0;
	int pin = 22;
	while(i<8){
			
			input[i] = digitalRead(pin);
			Serial.print(i);
			Serial.println(input[i]);
			
			i++;
			pin++;
	}
	regBank.set(40001, boolArrayToLong(input));
	slave.run();
}