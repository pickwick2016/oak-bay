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

#include <oak/data_type.h>

namespace oak {

	class Block;

	// 工作结果.
	enum WorkResult
	{
		Finish,
		Wait,
		Ok,
		Error,
	};
	

	/// 模块端口.
	struct Port
	{
		Port(Block * block = nullptr, unsigned int idx = 0);

		bool operator == (const Port & rhs) const;
		bool operator < (const Port & rhs) const;

		Block * block;		// 端口对应模块
		unsigned int index; // 端口序号.
	};

	DataType GetInputPortType(Port port);
	DataType GetOutputPortType(Port port);

	// 端口签名.
	struct Signature
	{
		Signature(DataType type, bool need = true, int count = 1);

		DataType type;	// 数据类型
		bool need;	// 是否必需
		int count;	// 最小数据数量.
	};

	// 端口签名列表.
	typedef std::vector<Signature> SignatureList;
	
	struct raw_data
	{
		raw_data() : count(0), data(nullptr) {}

		int count;
		void * data;
	};

	typedef std::vector<raw_data> vector_raw_data;

	typedef std::vector<int> vector_int;
	typedef std::vector<void *> vector_pvoid;
	typedef std::vector<const void *> vector_const_pvoid;

	
} // namespace oak

#endif //OAK_BLOCK_DETAIL_H
