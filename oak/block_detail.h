/**
 * Copyright Oak Team 2018
 *
 * @file block_detail.h
 * Block�ඨ����ص���������.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_BLOCK_DETAIL_H
#define OAK_BLOCK_DETAIL_H

#include <vector>

namespace oak {

	class Block;

	/// ģ��˿�.
	struct Port
	{
		Port(Block * block = nullptr, unsigned int idx = 0);

		Block * block;
		unsigned int index;
	};


	// �˿�ǩ��.
	struct Signature
	{

	};

	// �˿�ǩ���б�.
	typedef std::vector<Signature> SignatureList;

	
} // namespace oak

#endif //OAK_BLOCK_DETAIL_H
