#include <exception>
#include "DeviceDriver.h"

class ReadFailException : public std::exception {
public:
    ReadFailException()
        : exception("ReadFailException") { }
};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int result = (int)(m_hardware->read(address));
    for (int i = 1; i < 5; i++) {
        int temp = (int)(m_hardware->read(address));
        if (result != temp) throw ReadFailException();
    }
    return result;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}
