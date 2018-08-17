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
		// ִ��һ������.
		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs);

		// ����״̬.
		void reset();
	
	private:
		// ������л����Ƿ����.
		bool validate();

		// ״̬�Ƿ����ı�.
		bool isChanged();

	private:
		// ִ��һ��ģ��Ĳ���.
		int runBlock(Block * block, BlockState & state);

		// Ϊģ��׼�������Ļ���.
		bool prepareBlock(Block * block, BlockState & state);

		// ִ��������״̬.
		void updateBlock(Block * block, const BlockState & state);

		// ���컺����.
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

		std::vector<Block *> m_queue; // ���ж���.


		unsigned int m_bufferCountHint;
	};

} // namespace oak