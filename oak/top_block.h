/**
 * Copyright Oak Team 2018
 *
 * @file top_block.h
 * TopBlock�ඨ��.
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

	/** ����ģ��.
	 * 1.����ģ��.
	 * 2.ά�����ӹ�ϵ.
	 * 3.��������.
	 */
	class TopBlock : public Block
	{
	public:
		TopBlock();
		virtual ~TopBlock();

	public:
		// ���� Block

		virtual int work(vector_raw_data * inputs, vector_raw_data * outputs);
		virtual void reset();

	public:
		int start();
		void stop();

	public:
		// ���ģ��(�ӹ�)
		Block * add(Block * block);

		// ɾ��ģ��(����ģ��)
		void remove(Block * block);
		
		// �Ƿ����ģ��
		bool contain(Block * block);
		
		// ������ģ������.
		unsigned int count() const;

		// ���а�����ģ��.
		std::vector<Block *> blocks();

	public:
		// ����ģ��.
		bool connect(Port source, Port dest);
		
		// �Ͽ����ӣ�ɾ�����ӹ�ϵ��.
		void disconnect(Port source, Port dest);

		// �Ƿ��������.
		bool contain(Port source, Port dest);

		// ��ȡ���ӹ�ϵ.
		std::vector<std::pair<Port, Port>> connections() const;

		// ��ȡ����.
		std::vector<Port> getDestPorts(Port source);

		// ��ȡ����.
		Port getSourcePort(Port dest);

		// �������ͼ�Ƿ�����.
		bool checkGraph();

		// �ӿ��Ƿ�ƥ��.
		bool checkConnect(Port source, Port dest);
		
	private:
		std::vector<std::shared_ptr<Block>> m_blocks; // ģ��.
		std::vector<std::pair<Port, Port>> m_connections; // ���ӹ�ϵ.
		
		std::shared_ptr<BlockRuntime> m_runtime; // ���л���.
	};

} // namespace oak

#endif //OAK_TOP_BLOCK_H
