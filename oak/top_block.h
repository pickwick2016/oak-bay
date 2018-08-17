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
#include <oak/combo_block.h>

namespace oak {

	class BlockRuntime;

	/** ����ģ��.
	 * 1.����ģ��.
	 * 2.ά�����ӹ�ϵ.
	 * 3.��������.
	 */
	class TopBlock : public ComboBlock
	{
	public:
		TopBlock();
		virtual ~TopBlock();

	public:
		int start();
		void stop();
	};

} // namespace oak

#endif //OAK_TOP_BLOCK_H
