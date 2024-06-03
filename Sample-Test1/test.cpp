#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TDD_DeviceDriver/DeviceDriver.cpp"
#include "../TDD_DeviceDriver/Application.cpp"

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
	Application app{ &driver };
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

	EXPECT_THROW(driver.read(0), ReadFailException);
}

TEST_F(DeviceDriverFixture, WriteSuccess) {
	EXPECT_CALL(device, read, (_), ())
		.WillRepeatedly(testing::Return(DeviceDriver::CLEAN_SPACE));

	EXPECT_CALL(device, write, (_, _), ())
		.Times(1);

	driver.write(0, (unsigned char)MockFlashMemoryDevice::NormalData);
}

TEST_F(DeviceDriverFixture, WriteException) {
	EXPECT_CALL(device, read, (_), ())
		.WillRepeatedly(testing::Return(MockFlashMemoryDevice::NormalData));

	EXPECT_THROW(driver.write(0, (unsigned char)MockFlashMemoryDevice::NormalData), WriteFailException);
}

TEST_F(DeviceDriverFixture, ApplicationReadAndPrint0to0) {
    EXPECT_CALL(device, read, (_), ())
		.Times(5)
        .WillRepeatedly(testing::Return(MockFlashMemoryDevice::NormalData));

    app.ReadAndPrint(0, 0);
}

TEST_F(DeviceDriverFixture, ApplicationReadAndPrint0to4) {
    EXPECT_CALL(device, read, (_), ())
		.Times(25)
        .WillRepeatedly(testing::Return(MockFlashMemoryDevice::NormalData));

	app.ReadAndPrint(0, 4);
}

TEST_F(DeviceDriverFixture, ApplicationWriteAll) {
	EXPECT_CALL(device, read, (_), ())
		.Times(5)
		.WillRepeatedly(testing::Return(DeviceDriver::CLEAN_SPACE));

    EXPECT_CALL(device, write, (_, _), ())
		.Times(5);

	app.WriteAll(MockFlashMemoryDevice::NormalData);
}
