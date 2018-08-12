/**
* Copyright Oak Team 2018
*
* @file buffer.h
* 缓冲区类定义.
*
* @author pickwick
* @version 1.0 create
*/

#ifndef OAK_BUFFER_H
#define OAK_BUFFER_H

#include <vector>
#include <oak/data_type.h>

namespace oak {

	/** 缓冲区.
	 * 实现了FIFO缓冲区，用于产生和消耗数据.
	 */
	class FifoBuffer
	{
	public:
		virtual ~FifoBuffer() {}

	public:
		// 总元素数量.
		virtual int count() = 0;

		// 元素大小（字节）.
		virtual int elementSize() = 0;

		// 总的字节数.
		virtual int totalSize() { return count() * elementSize(); }

		// 输入缓冲区.
		virtual void * inputData() = 0;

		// 输入数据数量.
		virtual int inputCount() = 0;

		// 输出缓冲区.
		virtual void * outputData() = 0;

		// 输出数据区数量.
		virtual int outputCount() { return count() - inputCount(); }

		// 插入数据.
		virtual int push(void * data, int count) = 0;

		// 弹出数据.
		virtual int pop(int count, void * data = nullptr) = 0;
	};


	/** 内存Fifo缓冲区.
	 * 利用vector实现的FifoBuffer.
	 */
	class VecBuffer : public FifoBuffer
	{
	public:
		VecBuffer(DataType datatype, unsigned int count = 1024);
		VecBuffer(unsigned int elementSize, unsigned int count = 1024);

	public:
		// 总元素数量.
		virtual int count();

		// 元素大小（字节）.
		virtual int elementSize();
		
		// 输入缓冲区.
		virtual void * inputData();
		
		// 输入数据数量.
		virtual int inputCount();

		// 输出缓冲区.
		virtual void * outputData();
		
		// 插入数据.
		virtual int push(void * data, int count);
		
		// 弹出数据.
		virtual int pop(int count, void * data = nullptr);

	private:
		unsigned int m_elementSize;
		DataType m_datatype;

		unsigned int m_count;
		unsigned int m_dataCount;

		std::vector<char> m_buffer;
	};

} // namespace oak


#endif //OAK_BUFFER_H