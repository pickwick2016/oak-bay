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

namespace oak {

	class BlockRuntime;

	/** 顶层模块.
	 * 1.管理模块.
	 * 2.维护连接关系.
	 * 3.负责运行.
	 */
	class TopBlock : public Block
	{
	public:
		TopBlock();
		virtual ~TopBlock();

	public:
		// 重载 Block

		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs);
		virtual void reset();

	public:
		int start();
		void stop();

	public:
		// 添加模块(接管)
		Block * add(Block * block);

		// 删除模块(销毁模块)
		void remove(Block * block);
		
		// 是否包含模块
		bool contain(Block * block);
		
		// 包含的模块数量.
		unsigned int count() const;

		// 所有包含的模块.
		std::vector<Block *> blocks();

	public:
		// 连接模块.
		bool connect(Port source, Port dest);
		
		// 断开连接（删除连接关系）.
		void disconnect(Port source, Port dest);

		// 是否包含连接.
		bool contain(Port source, Port dest);

		// 获取连接关系.
		std::vector<std::pair<Port, Port>> connections() const;

		// 获取连接.
		std::vector<Port> getDestPorts(Port source);

		// 获取连接.
		Port getSourcePort(Port dest);

		// 检查连接图是否完整.
		bool checkGraph();

		// 接口是否匹配.
		bool checkConnect(Port source, Port dest);
		
	private:
		std::vector<std::shared_ptr<Block>> m_blocks; // 模块.
		std::vector<std::pair<Port, Port>> m_connections; // 连接关系.
		
		std::shared_ptr<BlockRuntime> m_runtime; // 运行环境.
	};

} // namespace oak

#endif //OAK_TOP_BLOCK_H
