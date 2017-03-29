#include <gmock/gmock.h>
#include "serializationTest.hpp"

using ::testing::ElementsAreArray;

PackerTest::PackerTest()
	: plEmpty({}), plInt{1, 2, 3, 4, 5}, plDouble{2.87, 3.14, -66.66, 1e-3}, packer(Packer())
{
}

TEST_F(PackerTest, HandlesEmptyMessage)
{
	EXPECT_THROW(packer.append(plEmpty), dss::serialization_error);
}

TEST_F(PackerTest, HandlesSupportedTypes)
{
	EXPECT_NO_THROW(packer.append(plInt));
	EXPECT_NO_THROW(packer.append(plDouble));
}

TEST_F(PackerTest, HandlesReset)
{
	packer.append(plDouble);
	packer.reset();

	std::vector<dss::byte> envelopeOnlyMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(envelopeOnlyMsg.size(), sizeof(Envelope));
}

TEST_F(PackerTest, MessageSize)
{
	// Empty message
	std::vector<dss::byte> envelopeOnlyMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(envelopeOnlyMsg.size(), sizeof(Envelope));

	const size_t sizeEvnHdr = sizeof(Envelope) + sizeof(Header);

	packer.append(plInt);
	std::vector<dss::byte> intMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(intMsg.size(), plInt.size() * sizeof(int) + sizeEvnHdr);

	packer.reset();
	packer.append(plDouble);
	std::vector<dss::byte> doubleMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(doubleMsg.size(), plDouble.size() * sizeof(double) + sizeEvnHdr);

	packer.append(plInt);
	std::vector<dss::byte> doubleIntMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(doubleIntMsg.size(), plInt.size() * sizeof(int) + plDouble.size() * sizeof(double) +
									   sizeof(Envelope) + 2 * sizeof(Header));
}

TEST_F(PackerTest, HandlesExtract)
{
	packer.append(plDouble);
	EXPECT_NO_THROW(packer.extract(Purpose::PRINT));
	std::vector<dss::byte> doubleMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(doubleMsg.size(),
			  plDouble.size() * sizeof(double) + sizeof(Envelope) + sizeof(Header));

	// Extract a second time
	packer.append(plDouble);
	doubleMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(doubleMsg.size(),
			  2 * (plDouble.size() * sizeof(double)) + sizeof(Envelope) + 2 * sizeof(Header));
}

TEST_F(PackerTest, HandlesDispatch)
{
	packer.append(plDouble);
	EXPECT_NO_THROW(packer.dispatch(Purpose::PRINT));
	std::vector<dss::byte> envelopeOnlyMsg = packer.extract(Purpose::PRINT);
	EXPECT_EQ(envelopeOnlyMsg.size(), sizeof(Envelope));
}

TEST_F(PackerTest, EnvelopAndHeader)
{
	packer.append(plInt);
	packer.append(plDouble);
	std::vector<dss::byte> msg = packer.extract(Purpose::PRINT);
	Envelope*              env = reinterpret_cast<Envelope*>(msg.data());

	EXPECT_EQ(env->version, 1);
	EXPECT_EQ(env->size, plInt.size() * sizeof(int) + plDouble.size() * sizeof(double) +
							 sizeof(Envelope) + 2 * sizeof(Header));
	EXPECT_EQ(env->purpose, Purpose::PRINT);

	Header* hdrInt = reinterpret_cast<Header*>(msg.data() + sizeof(Envelope));
	EXPECT_EQ(hdrInt->version, 1);
	EXPECT_EQ(hdrInt->size, plInt.size() * sizeof(int) + sizeof(Header));
	EXPECT_EQ(hdrInt->obj, Objects::INT);

	Header* hdrDouble = reinterpret_cast<Header*>(msg.data() + sizeof(Envelope) + hdrInt->size);
	EXPECT_EQ(hdrDouble->version, 1);
	EXPECT_EQ(hdrDouble->size, plDouble.size() * sizeof(double) + sizeof(Header));
	EXPECT_EQ(hdrDouble->obj, Objects::DOUBLE);

	dss::byte* msgEnd = reinterpret_cast<dss::byte*>(msg.data() + sizeof(Envelope) + hdrInt->size +
													 hdrDouble->size);
	EXPECT_EQ(msgEnd, msg.data() + env->size);
}

UnpackerTest::UnpackerTest()
	: plInt{1, 2, 3, 4, 5},
	  plDouble{2.87, 3.14, 66.66, 1e-3},
	  packer(Packer()),
	  unpacker(Unpacker())
{
	messageEmpty = packer.extract(Purpose::PRINT);

	packer.append(plDouble);
	messageDouble = packer.extract(Purpose::PRINT);

	packer.reset();
	packer.append(plInt);
	messageInt = packer.extract(Purpose::PRINT);

	packer.append(plDouble);
	messageIntDouble = packer.extract(Purpose::PRINT);
}

TEST_F(UnpackerTest, TestEnvelope)
{
	unpacker.load(messageInt);
	EXPECT_EQ(unpacker.purpose(), Purpose::PRINT);
}

TEST_F(UnpackerTest, HandleEmptyMessage) { EXPECT_EQ(messageEmpty.size(), sizeof(Envelope)); }

TEST_F(UnpackerTest, LoadSupportedTypes)
{
	EXPECT_NO_THROW(unpacker.load(messageEmpty));
	EXPECT_NO_THROW(unpacker.load(messageInt));
	EXPECT_NO_THROW(unpacker.load(messageDouble));
}

TEST_F(UnpackerTest, BuiltTocForSupportedTypes)
{
	unpacker.load(messageInt);
	auto objList = unpacker.objects();
	EXPECT_EQ(objList.size(), 1);
	EXPECT_EQ(objList[0], Objects::INT);

	unpacker.load(messageDouble);
	objList = unpacker.objects();
	EXPECT_EQ(objList.size(), 1);
	EXPECT_EQ(objList[0], Objects::DOUBLE);

	unpacker.load(messageIntDouble);
	objList = unpacker.objects();
	EXPECT_EQ(objList.size(), 2);
	EXPECT_EQ(objList[0], Objects::INT);
	EXPECT_EQ(objList[1], Objects::DOUBLE);
}

TEST_F(UnpackerTest, ExtractSupportedTypes)
{
	unpacker.load(messageInt);
	auto intData = unpacker.get<int>(0);
	EXPECT_THAT(intData, ElementsAreArray(plInt));

	unpacker.load(messageDouble);
	auto doubleData = unpacker.get<double>(0);
	EXPECT_THAT(doubleData, ElementsAreArray(plDouble));

	unpacker.load(messageIntDouble);
	intData    = unpacker.get<int>(0);
	doubleData = unpacker.get<double>(1);
	EXPECT_THAT(intData, ElementsAreArray(plInt));
	EXPECT_THAT(doubleData, ElementsAreArray(plDouble));
}

TEST_F(UnpackerTest, ThrowOnTypeMissmatch)
{
	unpacker.load(messageInt);
	EXPECT_THROW(unpacker.get<double>(0), dss::serialization_error);
}

TEST_F(UnpackerTest, ThrowOnIndexOutOfRange)
{
	unpacker.load(messageInt);
	EXPECT_THROW(unpacker.get<int>(1), std::out_of_range);
}
