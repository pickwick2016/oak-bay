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

#include <oak/data_type.h>

namespace oak {

	class Block;

	// �������.
	enum WorkResult
	{
		Finish,
		Wait,
		Ok,
		Error,
	};
	

	/// ģ��˿�.
	struct Port
	{
		Port(Block * block = nullptr, unsigned int idx = 0);

		bool operator == (const Port & rhs) const;
		bool operator < (const Port & rhs) const;

		Block * block;		// �˿ڶ�Ӧģ��
		unsigned int index; // �˿����.
	};

	DataType GetInputPortType(Port port);
	DataType GetOutputPortType(Port port);

	// �˿�ǩ��.
	struct Signature
	{
		Signature(DataType type, bool need = true, int count = 1);

		DataType type;	// ��������
		bool need;	// �Ƿ����
		int count;	// ��С��������.
	};

	// �˿�ǩ���б�.
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
