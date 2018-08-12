/**
 * Copyright Oak Team 2018
 *
 * @file dummy_block.h
 * ����ģ���ࡣ����Դ������Ͳ� 3��.
 *
 * @author pickwick
 * @version 1.0 create
 */

#ifndef DUMMY_BLOCK_H
#define DUMMY_BLOCK_H

#include <oak/block.h>

namespace dummy {

	using namespace oak;

	class DummySource : public oak::Block
	{
	public:
		DummySource(int count = 1024);

	public:
		virtual int work(vector_raw_data & inputs, vector_raw_data & outputs);
		virtual void reset();

	private:
		int m_totalCount;
		int m_currentCount;
	};

	class DummyBlock : public oak::Block
	{
	public:
		DummyBlock();

	public:
		virtual int work(vector_raw_data & inputs, vector_raw_data & outputs);
	};

	/**
	 * ģ�����ݲ�.
	 * ���ܣ���¼���յ�����������.
	 */
	class DummySink : public oak::Block
	{
	public:
		DummySink();

	public:
		virtual int work(vector_raw_data & inputs, vector_raw_data & outputs);
		virtual void reset();

	public:
		int count();

	private:
		int m_count;
	};

} // namespace dummy

#endif // DUMMY_BLOCK_H


