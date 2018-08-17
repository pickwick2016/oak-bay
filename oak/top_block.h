/**
 * Copyright Oak Team 2018
 *
 * @file top_block.h
 * TopBlock类定义.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_TOP_BLOCK_H
#define OAK_TOP_BLOCK_H

#include <memory>
#include <vector>

#include <oak/block.h>
#include <oak/block_detail.h>
#include <oak/combo_block.h>

namespace oak {

	class BlockRuntime;

	/** 顶层模块.
	 * 1.管理模块.
	 * 2.维护连接关系.
	 * 3.负责运行.
	 */
	class TopBlock : public ComboBlock
	{
	public:
		TopBlock();
		virtual ~TopBlock();

	public:
		int start();
		void stop();
	};

} // namespace oak

#endif //OAK_TOP_BLOCK_H
