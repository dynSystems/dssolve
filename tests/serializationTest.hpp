#pragma once

#include <string>
#include <gtest/gtest.h>
#include "serialize.hpp"

namespace
{

class PackerTest : public ::testing::Test
{
  protected:
	std::vector<int>    plEmpty;
	std::vector<int>    plInt;
	std::vector<double> plDouble;
	Packer              packer;

  public:
	PackerTest();
};

class UnpackerTest : public ::testing::Test
{
  protected:
	std::vector<int>       plInt;
	std::vector<double>    plDouble;
	std::vector<dss::byte> messageEmpty;
	std::vector<dss::byte> messageInt;
	std::vector<dss::byte> messageDouble;
	std::vector<dss::byte> messageIntDouble;
	Packer                 packer;
	Unpacker               unpacker;

  public:
	UnpackerTest();
};
}
