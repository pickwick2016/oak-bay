/**
 * Copyright Oak Team 2018
 *
 * @file data_type.h
 * ����������ص���������.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef OAK_DATA_TYPE_H
#define OAK_DATA_TYPE_H

namespace oak {

	// ��������.
	enum class DataType
	{
		Char,
		Short,
		Int,
		Real,
		Complex,
		Unknown,
	};


	// ��ȡ�������͵Ĵ�С(�ֽ���).
	unsigned int DtSize(DataType type);

} // namespace oak

#endif //OAK_DATA_TYPE_H
