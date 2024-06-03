#include <iostream>

#include "DeviceDriver.h"

class Application {
public:
	Application(DeviceDriver* driver)
		: driver(driver) {
	}

	void ReadAndPrint(long startAddr, long endAddr) {
		for (long curAddr = startAddr; curAddr <= endAddr; curAddr++) {
			std::cout << driver->read(startAddr) << " ";
		}
		std::cout << std::endl;
	}

	void WriteAll(unsigned char value) {

	}

private:
	DeviceDriver* driver;
};