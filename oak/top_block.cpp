
#include <algorithm>
#include <assert.h>

#include <oak/top_block.h>
#include <oak/block_runtime.h>

namespace oak {

	TopBlock::TopBlock()
	{
	}

	TopBlock::~TopBlock()
	{
	}

	

	int TopBlock::start()
	{
		reset();

		int ret = WorkResult::Error;
		while (true) {
			ret = work(nullptr, nullptr);
			if (ret == WorkResult::Finish || ret == WorkResult::Error) {
				break;
			}
		}

		return ret;
	}

	void TopBlock::stop()
	{
	}
	

} // namespace oak