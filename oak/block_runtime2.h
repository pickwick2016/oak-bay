#pragma once

#include <memory>
#include <map>
#include <vector>

#include <oak/block_detail.h>

namespace oak {

	class ComboBlock;
	class FifoBuffer;

	class BlockRuntime2
	{
	public:
		struct BlockState
		{
			vector_raw_data inputs;
			vector_raw_data outputs;
		};

	public:
		BlockRuntime2(ComboBlock * parent = nullptr, unsigned int countHint = 1024);
		~BlockRuntime2();

	public:
		// 执行一步工作.
		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs);

		// 重置状态.
		void reset();
	
	private:
		// 检查运行环境是否完毕.
		bool validate();

		// 状态是否发生改变.
		bool isChanged();

	private:
		// 执行一个模块的操作.
		int runBlock(Block * block, BlockState & state);

		// 为模块准备上下文环境.
		bool prepareBlock(Block * block, BlockState & state);

		// 执行完后更新状态.
		void updateBlock(Block * block, const BlockState & state);

		// 制造缓冲区.
		std::shared_ptr<FifoBuffer> makeBuffer(DataType datatype, unsigned int count);

		FifoBuffer * getInputBuffer(Port port);
		FifoBuffer * getOutputBuffer(Port port);
		std::vector<FifoBuffer *> getInputBuffers(const std::vector<Port> & ports);

		std::vector<Port> getEndPorts(Port start);

	private:
		ComboBlock * m_parent;
		std::vector<std::pair<Port, Port>> m_connections;

		std::map<Port, std::shared_ptr<FifoBuffer>> m_inputBuffers;
		std::map<Port, std::shared_ptr<FifoBuffer>> m_outputBuffers;

		std::vector<Block *> m_queue; // 运行队列.


		unsigned int m_bufferCountHint;
	};

} // namespace oak