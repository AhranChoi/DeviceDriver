#pragma once
#include "FlashMemoryDevice.h"

class DeviceDriver
{
public:
    static const unsigned char CLEAN_SPACE = 0xFF;

    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;
};