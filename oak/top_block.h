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
		virtual int work(vector_raw_data & inputs, vector_raw_data & outputs);

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

		// �ӿ��Ƿ�ƥ��.
		bool isMatch(Port source, Port dest);

		// ��ȡ����.
		std::vector<Port> getDestPorts(Port source);

		Port getSourcePort(Port dest);


	public:
		int start();
		void stop();

	private:
		std::vector<std::shared_ptr<Block>> m_blocks;
		std::vector<std::pair<Port, Port>> m_connections;
		
		std::shared_ptr<BlockRuntime> m_runtime;
	};

} // namespace oak

#endif //OAK_TOP_BLOCK_H
