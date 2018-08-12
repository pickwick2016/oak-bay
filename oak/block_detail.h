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
		Error = -1, // ����.
		Ok = 0,		// �ɹ�.
		Finish = 1, // ����.
		Wait = 2,	// �ȴ�.
	};
	

	/// ģ��˿�.
	struct Port
	{
	public:
		Port(Block * block = nullptr, unsigned int idx = 0);

		bool operator == (const Port & rhs) const;
		bool operator < (const Port & rhs) const;

	public:
		Block * block;		// �˿ڶ�Ӧģ��
		unsigned int index; // �˿����.
	};


	// �˿�ǩ��.
	struct Signature
	{
	public:
		Signature(DataType type, bool need = true, int count = 1);

		bool isValid() const;

	public:
		DataType type;	// ��������
		bool need;	// �Ƿ����
		int count;	// ��С��������.
	};

	// �˿�ǩ���б�.
	typedef std::vector<Signature> SignatureList;
	
	// ԭʼ����.
	struct raw_data
	{
	public:
		raw_data() : count(0), data(nullptr) {}

	public:
		int count;	// ��������.
		void * data; // ���ݻ�����.
	};

	typedef std::vector<raw_data> vector_raw_data;

	//typedef std::vector<int> vector_int;
	//typedef std::vector<void *> vector_pvoid;
	//typedef std::vector<const void *> vector_const_pvoid;

	
} // namespace oak

#endif //OAK_BLOCK_DETAIL_H
