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
		BlockRuntime(TopBlock * parent = nullptr, unsigned int countHint = 1024);
		~BlockRuntime();

	public:
		// 执行一步工作.
		virtual int work();

		// 重置状态.
		void reset();

		// 验证状态是否可用.
		bool validate();

	private:
		// 执行一个模块的操作.
		int runBlock(Block * block);
		
		// 为模块准备上下文环境.
		bool prepareBlock(Block * block, BlockState & state);
		
		// 执行完后更新状态.
		void updateBlock(Block * block, const BlockState & state);

		// 将模块图扁平化（队列化）
		std::vector<Block*> flatten();

		// (重新)设置缓冲区.
		bool setupBuffers();

	private:
		// 获取与输入端口相连接的缓冲区.
		FifoBuffer * getInputBuffer(Port port);

		// 获取与输出端口相连接的缓冲区.
		FifoBuffer * getOutputBuffer(Port port);

		// 获取与输出端口相连接的缓冲区. 
		std::vector<FifoBuffer *> getInputBuffers(const std::vector<Port> & ports);

		std::shared_ptr<FifoBuffer> makeBuffer(DataType datatype, int count);
				
	private:
		std::map<Port, std::shared_ptr<FifoBuffer>> m_inputBuffers;
		std::map<Port, std::shared_ptr<FifoBuffer>> m_outputBuffers;

		TopBlock * m_parent;

		std::vector<std::pair<Port, Port>> m_connections;
		std::vector<Block *> m_queue;

		unsigned int m_bufferCountHint;
	};

} // namespace oak

#endif //OAK_BLOCK_RUNTIME_H
