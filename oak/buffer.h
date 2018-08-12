/**
* Copyright Oak Team 2018
*
* @file buffer.h
* �������ඨ��.
*
* @author pickwick
* @version 1.0 create
*/

#ifndef OAK_BUFFER_H
#define OAK_BUFFER_H

#include <vector>
#include <oak/data_type.h>

namespace oak {

	/** ������.
	 * ʵ����FIFO�����������ڲ�������������.
	 */
	class FifoBuffer
	{
	public:
		virtual ~FifoBuffer() {}

	public:
		// ��Ԫ������.
		virtual int count() = 0;

		// Ԫ�ش�С���ֽڣ�.
		virtual int elementSize() = 0;

		// �ܵ��ֽ���.
		virtual int totalSize() { return count() * elementSize(); }

		// ���뻺����.
		virtual void * inputData() = 0;

		// ������������.
		virtual int inputCount() = 0;

		// ���������.
		virtual void * outputData() = 0;

		// �������������.
		virtual int outputCount() { return count() - inputCount(); }

		// ��������.
		virtual int push(void * data, int count) = 0;

		// ��������.
		virtual int pop(int count, void * data = nullptr) = 0;
	};


	/** �ڴ�Fifo������.
	 * ����vectorʵ�ֵ�FifoBuffer.
	 */
	class VecBuffer : public FifoBuffer
	{
	public:
		VecBuffer(DataType datatype, unsigned int count = 1024);
		VecBuffer(unsigned int elementSize, unsigned int count = 1024);

	public:
		// ��Ԫ������.
		virtual int count();

		// Ԫ�ش�С���ֽڣ�.
		virtual int elementSize();
		
		// ���뻺����.
		virtual void * inputData();
		
		// ������������.
		virtual int inputCount();

		// ���������.
		virtual void * outputData();
		
		// ��������.
		virtual int push(void * data, int count);
		
		// ��������.
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