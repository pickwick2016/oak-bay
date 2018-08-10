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
#include <oak/flowgraph.h>

namespace oak {

	/**
	 * ����ģ��.
	 */
	class TopBlock : public Block
	{
	public:
		TopBlock();
		virtual ~TopBlock();

	public:
		void add(Block * block);
		void remove(Block * block);
		bool contain(Block * block);
		int count() const;

		void connect(Port source, Port dest);
		void disconnect(Port source, Port dest);

	public:
		int start();
		void stop();

	private:
		std::vector<std::shared_ptr<Block>> m_blocks;
		std::shared_ptr<Flowgraph> m_flowgraph;
	};

} // namespace oak

#endif //OAK_TOP_BLOCK_H
