/**
 * Copyright Oak Team 2018
 *
 * @file dummy_block.h
 * 测试模块类。包括源、处理和槽 3类.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_DUMMY_BLOCK_H
#define OAK_DUMMY_BLOCK_H

#include <oak/block.h>

namespace oak {

	class DummySource : public Block
	{
	};

	class DummyBlock : public Block
	{
	};

	class DummySink : public Block
	{
	};

} // namespace oak

#endif //OAK_DUMMY_BLOCK_H


