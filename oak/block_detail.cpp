#include <oak/block_detail.h>

namespace oak {

	Port::Port(Block * block, unsigned int index)
	{
		this->block = block;
		this->index = index;
	}

} // namespace oak