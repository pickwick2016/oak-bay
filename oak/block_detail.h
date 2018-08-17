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
	class FifoBuffer;

	// 工作结果.
	enum WorkResult
	{
		Error = -1, // 错误.
		Ok = 0,		// 成功.
		Finish = 1, // 结束.
		Wait = 2,	// 等待.
	};

	int MergeResults(const std::vector<int> & results);


	/// 模块端口.
	struct Port
	{
	public:
		Port(Block * block = nullptr, unsigned int idx = 0);

		bool operator == (const Port & rhs) const;
		bool operator < (const Port & rhs) const;

	public:
		Block * block;		// 端口对应模块
		unsigned int index; // 端口序号.
	};


	// 端口签名.
	struct Signature
	{
	public:
		Signature(DataType type, bool need = true, int count = 1);

		bool isValid() const;

	public:
		DataType type;	// 数据类型
		bool need;	// 是否必需
		int count;	// 最小数据数量.
	};

	// 端口签名列表.
	typedef std::vector<Signature> SignatureList;

	// 原始数据.
	struct RawData
	{
	public:
		RawData() : count(0), data(nullptr) {}
		RawData(void * buf, int cnt) : data(buf), count(cnt) {}

	public:
		int count;	// 数据数量.
		void * data; // 数据缓冲区.
	};

	// 原始数据列表.
	typedef std::vector<RawData> vector_raw_data;


	vector_raw_data MakeOutputRawData(const std::vector<FifoBuffer *> & buffers);
	
	vector_raw_data MakeInputRawData(const std::vector<FifoBuffer *> & buffers);
	
	// 获取原始数据.
	RawData GetRawData(const vector_raw_data & data, unsigned int index);

	// 从原始数据列表中提取内存指针.
	template<class T>
	T * GetRawData(vector_raw_data & data, unsigned int index)
	{
		if (index < data.size()) {
			return (T *)data[index].data;
		}

		return (T *) nullptr;
	}

	// 从原始数据列表中提取内存指针.
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

	// 从原始数据列表中提取内存数量.
	int GetRawDataCount(vector_raw_data & data, unsigned int index);

	// 从原始数据列表中提取内存数量.
	int GetRawDataCount(vector_raw_data * data, unsigned int index);

	// 从原始数据列表中提取最少内存数量.
	int GetRawDataMinCount(vector_raw_data * data);

	// 设置原始数据列表中的数量.
	int SetRawDataCount(vector_raw_data * data, unsigned int index, int count);

	// 设置原始数据列表中的数量.
	int SetRawDataCount(vector_raw_data * data, int count);

} // namespace oak

#endif //OAK_BLOCK_DETAIL_H
