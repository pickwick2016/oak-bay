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
		BlockRuntime(TopBlock * parent = nullptr);

	public:
		virtual int work();

	private:
		// ִ��һ��ģ��Ĳ���.
		int runBlock(Block * block);
		
		// Ϊģ��׼�������Ļ���.
		bool prepareBlock(Block * block, BlockState & state);

		// ���ģ��.
		bool checkBlock(Block * block, const BlockState & state);

		// ִ��������״̬.
		void updateBlock(Block * block, const BlockState & state);

	public:
		std::vector<Block*> flatten();
		bool compareBlock(Block * block1, Block * block2);

	public:
		void resetBuffers();

	private:
		// ��ȡ������˿������ӵĻ�����.
		FifoBuffer * getInputBuffer(Port port);

		// ��ȡ������˿������ӵĻ�����.
		FifoBuffer * getOutputBuffer(Port port);

		// ��ȡ������˿������ӵĻ�����. 
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
