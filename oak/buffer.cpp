/**
 * Copyright Oak Team 2018
 *
 * @file buffer.cpp
 * Buffer¿‡ µœ÷.
 *
 * @author pickwick
 * @version 1.0 create
 */

#include <algorithm>

#include <oak/buffer.h>

namespace oak {


	VecBuffer::VecBuffer(DataType datatype, unsigned int count)
	{
		m_datatype = datatype;
		m_dataCount = 0;
		m_elementSize = DtSize(m_datatype);
		
		if (m_elementSize > 0) {
			m_count = std::max<int>(1, count);
		}
		else {
			m_count = 0;
		}

		m_buffer.resize(m_count * m_elementSize);
	}

	VecBuffer::VecBuffer(unsigned int elementSize, unsigned int count)
	{
		m_datatype = DataType::Unknown;
		m_elementSize = std::max<int>(1, elementSize);
		m_dataCount = 0;

		if (m_elementSize > 0) {
			m_count = std::max<int>(1, count);
		}
		else {
			m_count = 0;
		}

		m_buffer.resize(m_count * m_elementSize);
	}

	int VecBuffer::count()
	{
		return m_count;
	}

	int VecBuffer::elementSize()
	{
		return m_elementSize;
	}

	void * VecBuffer::inputData()
	{
		return count() > 0 ? m_buffer.data() : nullptr;
	}

	int VecBuffer::inputCount()
	{
		return m_dataCount;
	}

	void * VecBuffer::outputData()
	{
		if (count() > 0) {
			char * output = m_buffer.data();
			output += inputCount() * m_elementSize;

			return (void *) output;
		}

		return nullptr;
	}


	int VecBuffer::push(void * data, int count)
	{
		if (count <= 0)
			return 0;

		int bufCount = outputCount();
		int pushCount = std::min<int>(bufCount, count);
		if (pushCount > 0) {
			memcpy(outputData(), data, pushCount * m_elementSize);
			m_dataCount += pushCount;
		}

		return pushCount;
	}

	int VecBuffer::pop(int count, void * data)
	{
		int bufCount = inputCount();
		int count2 = (count < 0) ? bufCount : count;
		int popCount = std::min<int>(bufCount, count2);
		if (popCount > 0) {
			if (data != nullptr) {
				memcpy(data, inputData(), popCount * m_elementSize);
			}
			
			m_dataCount -= popCount;
			if (m_dataCount > 0) {
				int offset = popCount * m_elementSize;
				int moveSize = m_dataCount * m_elementSize;
				memcpy(inputData(), (const char *)inputData() + offset, moveSize);
			}
		}

		return popCount;
	}

} // namespace oak