/**
 * Copyright Oak Team 2018
 *
 * @file flowgraph.h
 * 流程图定义.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_BLOCK_RUNTIME_H
#define OAK_BLOCK_RUNTIME_H

#include <utility>
#include <vector>
#include <map>
#include <memory>

#include <oak/block.h>
#include <oak/block_detail.h>
#include <oak/buffer.h>

namespace oak {

	class VecBuffer;
	class TopBlock;

	/**
	 * 流程图.
	 * 维护 1)连接关系 以及 2)运行时状态.
	 */
	class BlockRuntime
	{
	public:
		struct BlockState
		{
			vector_raw_data inputs;
			vector_raw_data outputs;
		};

	public:
		BlockRuntime(TopBlock * parent = nullptr);

	public:
		virtual int work();

	private:
		// 执行一个模块的操作.
		int runBlock(Block * block);
		
		// 为模块准备上下文环境.
		bool prepareBlock(Block * block, BlockState & state);

		// 检查模块.
		bool checkBlock(Block * block, const BlockState & state);

		// 执行完后更新状态.
		void updateBlock(Block * block, const BlockState & state);

	public:
		std::vector<Block*> flatten();
		bool compareBlock(Block * block1, Block * block2);

	public:
		void resetBuffers();

	private:
		// 获取与输入端口相连接的缓冲区.
		FifoBuffer * getInputBuffer(Port port);

		// 获取与输出端口相连接的缓冲区.
		FifoBuffer * getOutputBuffer(Port port);

		// 获取与输出端口相连接的缓冲区. 
		std::vector<FifoBuffer *> getInputBuffers(const std::vector<Port> & ports);
				
	private:
		std::map<Port, std::shared_ptr<VecBuffer>> m_inputBuffers;
		std::map<Port, std::shared_ptr<VecBuffer>> m_outputBuffers;

		TopBlock * m_parent;

		std::vector<std::pair<Port, Port>> m_connections;
		std::vector<Block *> m_queue;
	};

} // namespace oak

#endif //OAK_BLOCK_RUNTIME_H
