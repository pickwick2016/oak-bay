
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

		outputs[0].count = count;
		m_totalCount += count;
		
		return WorkResult::Ok;
	}

	void DummySource::reset()
	{
		m_totalCount = 0;
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

		inputs[0].count = count;
		outputs[0].count = count;

		return WorkResult::Ok;
	}
	
	DummySink::DummySink()
	{
		m_inputSigs = oak::SignatureList({ { oak::DataType::Real, true } });
		m_count = 0;
	}

	int DummySink::work(vector_raw_data & inputs, vector_raw_data & outputs)
	{
		int count = inputs[0].count;
		if (count <= 0) {
			return WorkResult::Wait;
		}

		inputs[0].count = count;
		m_count += count;

		return WorkResult::Ok;
	}

	void DummySink::reset()
	{
		m_count = 0;
	}

	int DummySink::count()
	{
		return m_count;
	}

} // namespace oak