/**
 * Copyright Oak Team 2018
 *
 * @file dummy_block.h
 * 测试模块类。包括源、处理和槽 3类.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef DUMMY_BLOCK_H
#define DUMMY_BLOCK_H

#include <oak/block.h>

namespace dummy {

	class DummySource : public oak::Block
	{
	};

	class DummyBlock : public oak::Block
	{
	};

	class DummySink : public oak::Block
	{
	};

} // namespace dummy

#endif // DUMMY_BLOCK_H


