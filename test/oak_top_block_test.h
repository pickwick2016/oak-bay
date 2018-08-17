/**
* Copyright Oak Team 2018
*
* @file oak_top_block_test.h
* TopBlock 测试代码.
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

	// 创建部分.
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

	// 连接部分.
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

		int ret = top.work(nullptr, nullptr);
		int count = sink->count();
		BOOST_CHECK(count > 0);
	}

	{
		TopBlock top;

		auto source = top.add(new DummySource());
		auto sink = dynamic_cast<DummySink *>(top.add(new DummySink()));
		BOOST_TEST(sink != nullptr);

		top.connect(source, sink);

		int ret = top.work(nullptr, nullptr);
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

		int ret = top.work(nullptr, nullptr);
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

		int ret = top.work(nullptr, nullptr);
		int count = sink->count();
		BOOST_CHECK(count > 0);
	}
}

BOOST_AUTO_TEST_CASE(top_block_run_2)
{
	using namespace oak;
	using namespace dummy;
	
	{
		TopBlock top;

		auto source = top.add(new DummySource());
		auto block_1 = top.add(new DummyBlock());
		auto block_2 = top.add(new DummyBlock());
		auto sink = dynamic_cast<DummySink *>(top.add(new DummySink(2)));
		BOOST_TEST(sink != nullptr);

		BOOST_CHECK(top.connect(source, block_1));
		BOOST_CHECK(top.connect(source, block_2));
		BOOST_CHECK(top.connect(block_1, { sink, 0 }));
		BOOST_CHECK(top.connect(block_2, { sink, 1 }));

		int ret = top.work(nullptr, nullptr);
		BOOST_CHECK(ret == WorkResult::Ok);

		int count = sink->count();
		BOOST_CHECK(count > 0);
	}

	{
		TopBlock top;

		int sourceCount = 4000;
		auto source = top.add(new DummySource(sourceCount));
		auto block_1 = top.add(new DummyBlock());
		auto block_2 = top.add(new DummyBlock());
		auto sink = dynamic_cast<DummySink *>(top.add(new DummySink(2)));
		BOOST_TEST(sink != nullptr);

		BOOST_CHECK(top.connect(source, block_1));
		BOOST_CHECK(top.connect(source, block_2));
		BOOST_CHECK(top.connect(block_1, { sink, 0 }));
		BOOST_CHECK(top.connect(block_2, { sink, 1 }));

		int ret = top.start();
		BOOST_CHECK(ret == WorkResult::Finish);

		int count = sink->count();
		BOOST_CHECK(count == sourceCount * 2);
	}
}


BOOST_AUTO_TEST_SUITE_END()
