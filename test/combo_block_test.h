/**
 * Copyright Oak Team 2018
 *
 * @file combo_block_test.h
 * Combo Block ≤‚ ‘¥˙¬Î.
 *
 * @author pickwick
 * @version 1.0 create
 */

#include <vector>

#include <oak/buffer.h>
#include <oak/combo_block.h>

BOOST_AUTO_TEST_SUITE(test_combo_block)

BOOST_AUTO_TEST_CASE(combo_block_create)
{
	using namespace oak;
	using namespace dummy;

	vector_raw_data inputs, outputs;
	int ret;

	{
		ComboBlock cb({ {DataType::Real} }, { {DataType::Real} });

		auto block1 = cb.add(new DummyBlock());
		auto block2 = cb.add(new DummyBlock());
		BOOST_CHECK(cb.connect(block1, block2));
		BOOST_CHECK(cb.checkMap() == false);
		
		BOOST_CHECK(cb.setInput(0, { block1, 0 }));
		BOOST_CHECK(cb.setOutput(0, { block2, 0 }));
		BOOST_CHECK(cb.checkMap() == true);

		auto queue = cb.flatten();
		BOOST_CHECK(queue.size() == 2);
	}

	{
		ComboBlock cb;

		auto block1 = cb.add(new DummySource());
		auto block2 = cb.add(new DummySink());
		BOOST_CHECK(cb.connect(block1, block2));
		
		BOOST_CHECK(cb.checkMap() == true);
	}

	{
		ComboBlock cb({ { DataType::Real } }, { { DataType::Real } });

		auto block1 = cb.add(new DummySource());
		auto block2 = cb.add(new DummySink());
		BOOST_CHECK(cb.connect(block1, block2));

		BOOST_CHECK(cb.checkMap() == false);
	}

	{
		ComboBlock cb({ { DataType::Real } }, {});

		auto block1 = cb.add(new DummyBlock());
		auto block2 = cb.add(new DummySink());
		BOOST_CHECK(cb.connect(block1, block2));

		BOOST_CHECK(cb.checkMap() == false);

		BOOST_CHECK(cb.setInput(0, { block1, 0 }));
		BOOST_CHECK(cb.checkMap() == true);
	}

	{
		ComboBlock cb({ { DataType::Real } , { DataType::Real } }, {});

		auto block0 = cb.add(new DummyBlock());
		auto block1 = cb.add(new DummyBlock());
		auto block2 = cb.add(new DummySink(2));

		BOOST_CHECK(cb.connect(block0, block2));
		BOOST_CHECK(cb.connect(block1, block2) == false);

		BOOST_CHECK(cb.connect(block1, { block2, 1 }));
		
		BOOST_CHECK(cb.setInput(0, { block0, 0 }));
		BOOST_CHECK(cb.setInput(1, { block1, 0 }));

		BOOST_CHECK(cb.checkMap() == true);
	}
}

BOOST_AUTO_TEST_CASE(combo_block_run)
{
	using namespace oak;
	using namespace dummy;

	vector_raw_data inputs, outputs;
	int ret;

	{
		std::vector<float> buffer1(1024);
		std::vector<float> buffer2(1024);

		for (int i = 0; i < buffer1.size(); i++) {
			buffer1[i] = i;
		}

		inputs = { { buffer1.data(), 100 } };
		outputs = { { buffer2.data(), 100 } };

		ComboBlock cb({ DataType::Real } ,{ DataType::Real });

		auto block1 = cb.add(new DummyBlock());
		auto block2 = cb.add(new DummyBlock());
		BOOST_CHECK(cb.connect(block1, block2));
		BOOST_CHECK(cb.setInput(0, block1));
		BOOST_CHECK(cb.setOutput(0, block2));

		ret = cb.work(&inputs, &outputs);
		BOOST_CHECK(ret == WorkResult::Ok);
		BOOST_CHECK(inputs[0].count > 0);
		BOOST_CHECK(outputs[0].count > 0);

	}
}

BOOST_AUTO_TEST_SUITE_END()