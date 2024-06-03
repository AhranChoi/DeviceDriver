#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TDD_DeviceDriver/DeviceDriver.cpp"

class TestFlashMemoryDevice : public FlashMemoryDevice {
public:
	static const int NormalData = 0xAA;
	static const int AbnormalData = 0xBB;
	static const int AbnormalDataAddress = 0x1;

	virtual unsigned char read(long address) {
		if (address == AbnormalDataAddress) {
			static int i = 0;
			i++;
			if (i % 5 == 0) return AbnormalData;
			return NormalData;
		}
		return NormalData;
	}

	virtual void write(long address, unsigned char data) {

	}
};

class MockFlashMemoryDevice : public TestFlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
};

class DeviceDriverFixture : public testing::Test {
public:
	MockFlashMemoryDevice device;
	DeviceDriver driver{ &device };
};

TEST_F(DeviceDriverFixture, ReadSuccess) {
	long addr = 0x0;

	EXPECT_CALL(device, read, (_), ())
		.Times(5)
		.WillRepeatedly(testing::Return(MockFlashMemoryDevice::NormalData));

	EXPECT_EQ(driver.read(addr), MockFlashMemoryDevice::NormalData);
}

TEST_F(DeviceDriverFixture, ReadException) {
	long addr = MockFlashMemoryDevice::AbnormalDataAddress;

	EXPECT_CALL(device, read, (_), ())
		.Times(5)
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::AbnormalData));

	try {
		driver.read(addr);
		FAIL();
	}
	catch (...) { }
}
