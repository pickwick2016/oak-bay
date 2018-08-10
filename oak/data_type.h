/**
 * Copyright Oak Team 2018
 *
 * @file data_type.h
 * 数据类型相关的其他定义.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_DATA_TYPE_H
#define OAK_DATA_TYPE_H

namespace oak {

	// 数据类型.
	enum class DataType
	{
		Char,
		Short,
		Int,
		Real,
		Complex,
		Unknown,
	};


	// 获取数据类型的大小(字节数).
	unsigned int DtSize(DataType type);

} // namespace oak

#endif //OAK_DATA_TYPE_H
