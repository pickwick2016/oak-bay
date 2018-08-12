
#include <algorithm>

#include <oak/dummy/dummy_block.h>
#include <oak/data_type.h>

namespace dummy {

	DummySource::DummySource(int count)
	{
		m_outputSigs = oak::SignatureList({ {oak::DataType::Real, true} });
		m_currentCount = 0;
		m_totalCount = std::max<int>(count, 1);
	}

	int DummySource::work(vector_raw_data & inputs, vector_raw_data & outputs)
	{
		int remainCount = m_totalCount - m_currentCount;
		if (remainCount <= 0) {
			return WorkResult::Finish;
		}

		int count = std::min<int>(outputs[0].count, remainCount);
		if (count <= 0) {
			return WorkResult::Wait;
		}

		float * output = (float *)(outputs[0].data);
		for (int i = 0; i < count; i++) {
			output[i] = m_currentCount + i;
		}
		outputs[0].count = count;
		
		m_currentCount += count;
		
		return WorkResult::Ok;
	}

	void DummySource::reset()
	{
		m_currentCount = 0;
	}

	DummyBlock::DummyBlock()
	{
		m_inputSigs = oak::SignatureList({ { oak::DataType::Real, true } });
		m_outputSigs = oak::SignatureList({ { oak::DataType::Real, true } });
	}


	int DummyBlock::work(vector_raw_data & inputs, vector_raw_data & outputs)
	{
		int count = std::min<int>(inputs[0].count, outputs[0].count);
		if (count <= 0) {
			return WorkResult::Wait;
		}

		float * input = (float *)(inputs[0].data);
		float * output = (float *)(outputs[0].data);
		for (int i = 0; i < count; i++) {
			output[i] = input[i];
		}
		inputs[0].count = count;
		outputs[0].count = count;

		return WorkResult::Ok;
	}
	
	DummySink::DummySink()
	{
		m_inputSigs = oak::SignatureList({ { oak::DataType::Real, true } });
	}

	int DummySink::work(vector_raw_data & inputs, vector_raw_data & outputs)
	{
		int count = inputs[0].count;
		if (count <= 0) {
			return WorkResult::Wait;
		}

		float * input = (float *)(inputs[0].data);
		for (int i = 0; i < count; i++) {
			m_buffer.push_back(input[i]);
		}
		inputs[0].count = count;

		return WorkResult::Ok;
	}

	void DummySink::reset()
	{
		m_buffer.clear();
	}

	int DummySink::count()
	{
		return m_buffer.size();
	}

} // namespace oak