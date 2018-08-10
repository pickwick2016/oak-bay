/**
 * Copyright Oak Team 2018
 *
 * @file block_detail.h
 * Block类定义相关的其他定义.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_BLOCK_DETAIL_H
#define OAK_BLOCK_DETAIL_H

#include <vector>

namespace oak {

	class Block;

	/// 模块端口.
	struct Port
	{
		Port(Block * block = nullptr, unsigned int idx = 0);

		Block * block;		// 端口对应模块
		unsigned int index; // 端口序号.
	};


	// 端口签名.
	struct Signature
	{
		Signature(int type, bool need = true, int count = 1);

		int type;	// 数据类型
		bool need;	// 是否必需
		int count;	// 最小数据数量.
	};

	// 端口签名列表.
	typedef std::vector<Signature> SignatureList;

	
} // namespace oak

#endif //OAK_BLOCK_DETAIL_H
