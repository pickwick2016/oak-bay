/**
 * Copyright Oak Team 2018
 *
 * @file dummy_block_test.h
 * Dummy Block ≤‚ ‘¥˙¬Î.
 *
 * @author pickwick
 * @version 1.0 create
 */

#include <vector>

#include <oak/buffer.h>
#include <oak/dummy/dummy_block.h>

BOOST_AUTO_TEST_SUITE(test_dummy_block)

BOOST_AUTO_TEST_CASE(dummy_source_test)
{
	using namespace oak;
	using namespace dummy;

	vector_raw_data inputs, outputs;
	int ret;

	{
		VecBuffer buffer(DataType::Real, 100);

		DummySource source(32);

		outputs = MakeOutputRawData({ &buffer });
		ret = source.work(nullptr, & outputs);

		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(outputs[0].count == 32);

		outputs = MakeOutputRawData({ &buffer });
		ret = source.work(nullptr, & outputs);

		BOOST_CHECK(ret == WorkResult::Finish);
		BOOST_CHECK(outputs[0].count == 0);
	}

	{
		VecBuffer buffer(DataType::Real, 100);

		DummySource source(132);

		outputs = MakeOutputRawData({ &buffer });
		ret = source.work(nullptr, &outputs);

		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(outputs[0].count == 100);

		outputs = MakeOutputRawData({ &buffer });
		ret = source.work(nullptr, &outputs);

		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(outputs[0].count == 32);

		
		outputs = MakeOutputRawData({ &buffer });
		ret = source.work(nullptr, &outputs);

		BOOST_CHECK(ret == WorkResult::Finish);
		BOOST_CHECK(outputs[0].count == 0);
	}
}


BOOST_AUTO_TEST_CASE(dummy_sink_test)
{
	using namespace oak;
	using namespace dummy;

	vector_raw_data inputs, outputs;
	int ret;

	{
		std::vector<float> buffer(1024);

		inputs = { {buffer.data(), 1024} };
		DummySink sink(1);

		ret = sink.work(&inputs, nullptr);
		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(inputs[0].count == 1024);
		BOOST_CHECK(sink.count() == 1024);

		ret = sink.work(nullptr, &inputs);
		BOOST_CHECK(ret == WorkResult::Error);
		BOOST_CHECK(sink.count() == 1024);
	}

}


BOOST_AUTO_TEST_CASE(dummy_block_test)
{
	using namespace oak;
	using namespace dummy;
	
	vector_raw_data inputs, outputs;
	int ret;

	{
		std::vector<float> buffer1(1024);
		std::vector<float> buffer2(1024);

		inputs = { { buffer1.data(), 100 } };
		outputs = { {buffer2.data(), 1024} };
		DummyBlock block;

		ret = block.work(&inputs, & outputs);
		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(inputs[0].count == 100);
		BOOST_CHECK(outputs[0].count == 100);

		inputs = { { buffer1.data(), 1024 } };
		outputs = { { buffer2.data(), 100 } };

		ret = block.work(&inputs, &outputs);
		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(inputs[0].count == 100);
		BOOST_CHECK(outputs[0].count == 100);

		inputs = { { buffer1.data(), 100 } };
		outputs = { { buffer2.data(), 100 } };

		ret = block.work(&inputs, &outputs);
		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(inputs[0].count == 100);
		BOOST_CHECK(outputs[0].count == 100);

		ret = block.work(&inputs, nullptr);
		BOOST_CHECK(ret == WorkResult::Error);

		ret = block.work(nullptr, &outputs);
		BOOST_CHECK(ret == WorkResult::Error);

	}
}


BOOST_AUTO_TEST_SUITE_END()

