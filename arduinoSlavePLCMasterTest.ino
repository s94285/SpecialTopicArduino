#include <Wire.h>
#include <dht.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <modbusSlave.h>
#include <modbusRegBank.h>
#include <modbusDevice.h>
#include <modbus.h>

modbusDevice regBank;
modbusSlave slave;
dht DHT;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
bool input[8];

#define RS485RxTxEnablePin 2
#define RS485Baud 9600
#define RS485Format SERIAL_8E1 //define for Serial 8 even 1

void setup()
{
	lcd.begin(16, 2);
	lcd.clear();

	for (byte i = 0; i<3; i++) {  //flash LCD backlight 3 times for boot indicating
		lcd.noBacklight();
		delay(250);
		lcd.backlight();
		delay(250);
	}

	for (int pinOut = 31; pinOut <= 44; pinOut++) {
		pinMode(pinOut, OUTPUT);
		digitalWrite(pinOut, HIGH);
	}

	regBank.setId(1); //Set Modbus Slave ID
	regBank.add(40001); //Add hoding register 40001 = temperature
	regBank.add(40002); //40002 = target Temp
	regBank.add(40003); //40003 = LED state

	slave._device = &regBank;

	slave.setBaud(&Serial3, RS485Baud, RS485Format, RS485RxTxEnablePin);	//modbusSlave.setBaud( Serial, BaudRate, Format, EnablePin);

}


void loop()
{	
	for (int i = 0; i <=10; i++) { //LCD refresh every 500ms
		DHT.read11(A0);
		regBank.set(40001, (word)DHT.temperature); //put temperature in register
		slave.run();
		toggleLED();
		delay(50);
	}
	readTempToLCD();
}


void readTempToLCD(){  //show informations on LCD
	lcd.setCursor(0, 0);
	lcd.print("Now = ");
	lcd.print((float)DHT.humidity, 0);
	lcd.print(" %  ");
	lcd.print((float)DHT.temperature, 0);
	lcd.print(" C");
	lcd.setCursor(0, 1);
	lcd.print("Target = ");
	lcd.print(regBank.get(40002));
	lcd.print(" C");
}

void toggleLED() {  //Change LED state by reading register
	word toggleWord = regBank.get(40003);
	for (int j = 0; j <= 8; j++) {
		input[j] = toggleWord % 2;
		toggleWord /= 2;
	}
	digitalWrite(31, (input[0]) ? LOW : HIGH);
	digitalWrite(32, (input[1]) ? LOW : HIGH);
	digitalWrite(33, (input[2]) ? LOW : HIGH);
	digitalWrite(34, (input[2]) ? LOW : HIGH);
	digitalWrite(35, (input[2]) ? LOW : HIGH);
	digitalWrite(36, (input[3]) ? LOW : HIGH);
	digitalWrite(37, (input[3]) ? LOW : HIGH);
	digitalWrite(38, (input[3]) ? LOW : HIGH);
	digitalWrite(39, (input[3]) ? LOW : HIGH);
	digitalWrite(40, (input[4]) ? LOW : HIGH);
	digitalWrite(41, (input[5]) ? LOW : HIGH);
	digitalWrite(42, (input[6]) ? LOW : HIGH);
	digitalWrite(43, (input[7]) ? LOW : HIGH);
	digitalWrite(44, (input[7]) ? LOW : HIGH);
}