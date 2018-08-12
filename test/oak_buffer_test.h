/**
 * Copyright Oak Team 2018
 *
 * @file oak_buffer_test.h
 * »º³åÇø²âÊÔ´úÂë.
 *
 * @author pickwick
 * @version 1.0 create
 */

#include <vector>

#include <oak/buffer.h>
#include <oak/data_type.h>

BOOST_AUTO_TEST_SUITE(test_oak_buffer)

BOOST_AUTO_TEST_CASE(vec_buffer_create)
{
	//BOOST_TEST_MESSAGE("running test_case3");
	//BOOST_TEST(true);

	using namespace oak;
	
	{
		oak::VecBuffer buffer(DataType::Char);

		BOOST_CHECK(buffer.count() > 0);

		BOOST_CHECK(buffer.inputCount() == 0);
		BOOST_CHECK(buffer.inputData() != nullptr);
		BOOST_CHECK(buffer.outputCount() > 0);
		BOOST_CHECK(buffer.outputData() != nullptr);
	}

	{
		oak::VecBuffer buffer(DataType::Int);

		BOOST_CHECK(buffer.count() > 0);

		BOOST_CHECK(buffer.inputCount() == 0);
		BOOST_CHECK(buffer.inputData() != nullptr);
		BOOST_CHECK(buffer.outputCount() > 0);
		BOOST_CHECK(buffer.outputData() != nullptr);
	}

	{
		oak::VecBuffer buffer(DataType::Unknown);

		BOOST_CHECK(buffer.count() == 0);

		BOOST_CHECK(buffer.inputCount() == 0);
		BOOST_CHECK(buffer.inputData() == nullptr);
		BOOST_CHECK(buffer.outputCount() == 0);
		BOOST_CHECK(buffer.outputData() == nullptr);
	}

}

BOOST_AUTO_TEST_CASE(vec_buffer_operate)
{
	using namespace oak;

	std::vector<int> data(256);
	for (int i = 0; i < data.size(); i++) {
		data[i] = i;
	}

	VecBuffer buffer(DataType::Int, 100);

	BOOST_CHECK(buffer.count() == 100);

	BOOST_CHECK(buffer.inputCount() == 0);
	BOOST_CHECK(buffer.inputData() != nullptr);
	BOOST_CHECK(buffer.outputCount() == 100);
	BOOST_CHECK(buffer.outputData() != nullptr);

	int ret = 0;

	ret = buffer.push(data.data(), 50);
	BOOST_CHECK(ret == 50);
	BOOST_CHECK(buffer.inputCount() == 50);
	BOOST_CHECK(buffer.inputData() != nullptr);
	BOOST_CHECK(buffer.outputCount() == 50);
	BOOST_CHECK(buffer.outputData() != nullptr);


	ret = buffer.pop(30);
	BOOST_CHECK(ret == 30);
	BOOST_CHECK(buffer.inputCount() == 20);
	BOOST_TEST(buffer.inputData() != nullptr);
	BOOST_CHECK(((int *)buffer.inputData())[0] == 30);
	BOOST_CHECK(buffer.outputCount() == 80);
	BOOST_CHECK(buffer.outputData() != nullptr);

	ret = buffer.pop(30);
	BOOST_CHECK(ret == 20);
	BOOST_CHECK(buffer.inputCount() == 0);
	BOOST_TEST(buffer.inputData() != nullptr);
	BOOST_CHECK(buffer.outputCount() == 100);
	BOOST_CHECK(buffer.outputData() != nullptr);

	ret = buffer.push(data.data(), 150);
	BOOST_CHECK(ret == 100);
	BOOST_CHECK(buffer.inputCount() == 100);
	BOOST_TEST(buffer.inputData() != nullptr);
	BOOST_CHECK(buffer.outputCount() == 0);
	BOOST_CHECK(buffer.outputData() != nullptr);

	ret = buffer.pop(-1);
	BOOST_CHECK(ret == 100);
	BOOST_CHECK(buffer.inputCount() == 0);
	BOOST_CHECK(buffer.inputData() != nullptr);
	BOOST_CHECK(buffer.outputCount() == 100);
	BOOST_CHECK(buffer.outputData() != nullptr);

	BOOST_TEST(buffer.push(data.data(), 50) == 50);
	
	BOOST_TEST(buffer.push(data.data(), 60) == 50);
	BOOST_CHECK(buffer.inputCount() == 100);
	BOOST_TEST(buffer.inputData() != nullptr);
	BOOST_CHECK(((int *)buffer.inputData())[50] == 0);
	
	BOOST_TEST(buffer.pop(30) == 30);
	BOOST_TEST(buffer.inputData() != nullptr);
	BOOST_CHECK(((int *)buffer.inputData())[0] == 30);
	BOOST_CHECK(((int *)buffer.inputData())[20] == 0);

}

BOOST_AUTO_TEST_SUITE_END()
