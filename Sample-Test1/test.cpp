#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TDD_DeviceDriver/DeviceDriver.cpp"

class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
	static const int NormalData = 0xAA;
	static const int AbnormalData = 0xBB;

	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DeviceDriverFixture : public testing::Test {
public:
	MockFlashMemoryDevice device;
	DeviceDriver driver{ &device };
};

TEST_F(DeviceDriverFixture, ReadSuccess) {
	EXPECT_CALL(device, read, (_), ())
		.Times(5)
		.WillRepeatedly(testing::Return(MockFlashMemoryDevice::NormalData));

	EXPECT_EQ(driver.read(0), MockFlashMemoryDevice::NormalData);
}

TEST_F(DeviceDriverFixture, ReadException) {
	EXPECT_CALL(device, read, (_), ())
		.Times(5)
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::NormalData))
		.WillOnce(testing::Return(MockFlashMemoryDevice::AbnormalData));

	try {
		driver.read(0);
		FAIL();
	}
	catch (ReadFailException e) { }
	catch (...) {
		FAIL();
	}
}

TEST_F(DeviceDriverFixture, WriteSuccess) {
	EXPECT_CALL(device, read, (_), ())
		.WillRepeatedly(testing::Return(DeviceDriver::CLEAN_SPACE));

	EXPECT_CALL(device, write, (_, _), ())
		.Times(1);

	try {
		driver.write(0, (unsigned char)MockFlashMemoryDevice::NormalData);
	}
	catch (std::exception e) {
		FAIL();
	}
}

TEST_F(DeviceDriverFixture, WriteException) {
	EXPECT_CALL(device, read, (_), ())
		.WillRepeatedly(testing::Return(MockFlashMemoryDevice::NormalData));

	try {
		driver.write(0, (unsigned char)MockFlashMemoryDevice::NormalData);
		FAIL();
	}
	catch (WriteFailException e) {

	}
	catch (...) {
		FAIL();
	}
}
