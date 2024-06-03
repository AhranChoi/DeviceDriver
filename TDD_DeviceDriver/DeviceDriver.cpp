#include <exception>
#include "DeviceDriver.h"

class ReadFailException : public std::exception {
public:
    ReadFailException()
        : exception("ReadFailException") { }
};

class WriteFailException : public std::exception {
public:
    WriteFailException()
        : exception("WriteFailException") { }
};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    static const int _ReadRepeat = 5;
    unsigned char result = m_hardware->read(address);

    for (int i = 1; i < _ReadRepeat; i++) {
        int temp = m_hardware->read(address);
        if (result != temp) throw ReadFailException();
    }
    return (int)result;
}

void DeviceDriver::write(long address, int data)
{
    if (m_hardware->read(address) != CLEAN_SPACE) throw WriteFailException();
    m_hardware->write(address, (unsigned char)data);
}
