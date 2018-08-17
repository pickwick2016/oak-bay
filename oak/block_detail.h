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
	class FifoBuffer;

	// �������.
	enum WorkResult
	{
		Error = -1, // ����.
		Ok = 0,		// �ɹ�.
		Finish = 1, // ����.
		Wait = 2,	// �ȴ�.
	};

	int MergeResults(const std::vector<int> & results);


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
	struct RawData
	{
	public:
		RawData() : count(0), data(nullptr) {}
		RawData(void * buf, int cnt) : data(buf), count(cnt) {}

	public:
		int count;	// ��������.
		void * data; // ���ݻ�����.
	};

	// ԭʼ�����б�.
	typedef std::vector<RawData> vector_raw_data;


	vector_raw_data MakeOutputRawData(const std::vector<FifoBuffer *> & buffers);
	
	vector_raw_data MakeInputRawData(const std::vector<FifoBuffer *> & buffers);
	
	// ��ȡԭʼ����.
	RawData GetRawData(const vector_raw_data & data, unsigned int index);

	// ��ԭʼ�����б�����ȡ�ڴ�ָ��.
	template<class T>
	T * GetRawData(vector_raw_data & data, unsigned int index)
	{
		if (index < data.size()) {
			return (T *)data[index].data;
		}

		return (T *) nullptr;
	}

	// ��ԭʼ�����б�����ȡ�ڴ�ָ��.
	template<class T>
	T * GetRawData(vector_raw_data * data, unsigned int index)
	{
		if (data != nullptr) {
			if (index < (*data).size()) {
				return (T *)(*data)[index].data;
			}
		}

		return (T *) nullptr;
	}

	// ��ԭʼ�����б�����ȡ�ڴ�����.
	int GetRawDataCount(vector_raw_data & data, unsigned int index);

	// ��ԭʼ�����б�����ȡ�ڴ�����.
	int GetRawDataCount(vector_raw_data * data, unsigned int index);

	// ��ԭʼ�����б�����ȡ�����ڴ�����.
	int GetRawDataMinCount(vector_raw_data * data);

	// ����ԭʼ�����б��е�����.
	int SetRawDataCount(vector_raw_data * data, unsigned int index, int count);

	// ����ԭʼ�����б��е�����.
	int SetRawDataCount(vector_raw_data * data, int count);

} // namespace oak

#endif //OAK_BLOCK_DETAIL_H
