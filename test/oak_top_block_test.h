/**
* Copyright Oak Team 2018
*
* @file oak_top_block_test.h
* TopBlock ���Դ���.
*
* @author pickwick
* @version 1.0 create
*/

#include <oak/dummy/dummy_block.h>
#include <oak/top_block.h>

BOOST_AUTO_TEST_SUITE(test_oak_top_block)

BOOST_AUTO_TEST_CASE(top_block_create)
{
	using namespace oak;
	using namespace dummy;

	TopBlock top;

	// ��������.
	auto source = top.add(new DummySource());
	auto block = top.add(new DummyBlock());
	auto sink = top.add(new DummySink());

	BOOST_CHECK(source != nullptr);
	BOOST_CHECK(block != nullptr);
	BOOST_CHECK(sink != nullptr);
	BOOST_CHECK(top.contain(source));
	BOOST_CHECK(top.contain(block));
	BOOST_CHECK(top.contain(sink));

	BOOST_CHECK(top.count() == 3);

	// ���Ӳ���.
	BOOST_CHECK(top.connect(source, block));
	BOOST_CHECK(top.contain(source, block));

	BOOST_CHECK(top.connect(block, sink));
	BOOST_CHECK(top.contain(block, sink));
	BOOST_CHECK(top.connections().size() == 2);

	top.remove(block);
	BOOST_CHECK(top.count() == 2);
	BOOST_CHECK(top.connections().size() == 0);
	

	//BOOST_TEST_MESSAGE("running test_case3");
	//BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(top_block_run)
{
	using namespace oak;
	using namespace dummy;

	{
		TopBlock top;

		auto source = top.add(new DummySource(100));
		auto sink = dynamic_cast<DummySink *>(top.add(new DummySink()));
		BOOST_TEST(sink != nullptr);

		top.connect(source, sink);

		vector_raw_data temp1, temp2;
		int ret = top.work(temp1, temp2);
		int count = sink->count();
		BOOST_CHECK(count == 100);
	}

	{
		TopBlock top;

		auto source = top.add(new DummySource());
		auto sink = dynamic_cast<DummySink *>(top.add(new DummySink()));
		BOOST_TEST(sink != nullptr);

		top.connect(source, sink);

		vector_raw_data temp1, temp2;
		int ret = top.work(temp1, temp2);
		int count = sink->count();
		BOOST_CHECK(count > 0);
	}
	
	{
		TopBlock top;

		auto source = top.add(new DummySource());
		auto block = top.add(new DummyBlock());
		auto sink = dynamic_cast<DummySink *>(top.add(new DummySink()));
		BOOST_TEST(sink != nullptr);

		top.connect(source, block);
		top.connect(block, sink);

		vector_raw_data temp1, temp2;
		int ret = top.work(temp1, temp2);
		int count = sink->count();
		BOOST_CHECK(count > 0);
	}

	{
		TopBlock top;

		auto source = top.add(new DummySource(100));
		auto block = top.add(new DummyBlock());
		auto sink = dynamic_cast<DummySink *>(top.add(new DummySink()));
		BOOST_TEST(sink != nullptr);

		top.connect(source, block);
		top.connect(block, sink);

		vector_raw_data temp1, temp2;
		int ret = top.work(temp1, temp2);
		int count = sink->count();
		BOOST_CHECK(count == 100);
	}
}

BOOST_AUTO_TEST_SUITE_END()
