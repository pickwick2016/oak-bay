/**
 * Copyright Oak Team 2018
 *
 * @file flowgraph.h
 * ����ͼ����.
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
	 * ����ͼ.
	 * ά�� 1)���ӹ�ϵ �Լ� 2)����ʱ״̬.
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
		// ִ��һ������.
		virtual int work();

		// ����״̬.
		void reset();

		// ��֤״̬�Ƿ����.
		bool validate();

	private:
		// ִ��һ��ģ��Ĳ���.
		int runBlock(Block * block);
		
		// Ϊģ��׼�������Ļ���.
		bool prepareBlock(Block * block, BlockState & state);
		
		// ִ��������״̬.
		void updateBlock(Block * block, const BlockState & state);

		// ��ģ��ͼ��ƽ�������л���
		std::vector<Block*> flatten();

		// (����)���û�����.
		bool setupBuffers();

	private:
		// ��ȡ������˿������ӵĻ�����.
		FifoBuffer * getInputBuffer(Port port);

		// ��ȡ������˿������ӵĻ�����.
		FifoBuffer * getOutputBuffer(Port port);

		// ��ȡ������˿������ӵĻ�����. 
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
