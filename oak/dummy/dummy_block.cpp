
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

	int DummySource::work(vector_raw_data * inputs, vector_raw_data * outputs)
	{
		if (!check(inputs, outputs)) {
			return WorkResult::Error;
		}

		int remainCount = m_totalCount - m_currentCount;
		if (remainCount <= 0) {
			SetRawDataCount(outputs, 0, 0);
			return WorkResult::Finish;
		}

		int count = std::min<int>(GetRawDataCount(outputs, 0), remainCount);
		if (count <= 0) {
			SetRawDataCount(outputs, 0, 0);
			return WorkResult::Wait;
		}

		float * output = GetRawData<float>(outputs, 0);
		for (int i = 0; i < count; i++) {
			output[i] = m_currentCount + i;
		}

		m_currentCount += count;

		SetRawDataCount(outputs, 0, count);
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


	int DummyBlock::work(vector_raw_data * inputs, vector_raw_data * outputs)
	{
		if (!check(inputs, outputs)) {
			return WorkResult::Error;
		}

		int count = std::min<int>(GetRawDataCount(inputs, 0), GetRawDataCount(outputs, 0));
		if (count <= 0) {
			SetRawDataCount(inputs, 0, 0);
			SetRawDataCount(outputs, 0, 0);
			return WorkResult::Wait;
		}

		float * input = GetRawData<float>(inputs, 0);
		float * output = GetRawData<float>(outputs, 0);

		for (int i = 0; i < count; i++) {
			output[i] = input[i];
		}

		SetRawDataCount(inputs, 0, count);
		SetRawDataCount(outputs, 0, count);

		return WorkResult::Ok;
	}
	
	DummySink::DummySink(unsigned int port)
	{
		m_ports = std::max<int>(1, port);

		m_inputSigs = oak::SignatureList(m_ports, { oak::DataType::Real, true });
	}

	int DummySink::work(vector_raw_data * inputs, vector_raw_data * outputs)
	{
		if (!check(inputs, outputs)) {
			return WorkResult::Error;
		}

		int count = GetRawDataMinCount(inputs);
		if (count <= 0) {
			SetRawDataCount(inputs, 0);
			return WorkResult::Wait;
		}
		
		std::vector<float *> data(m_ports);
		for (unsigned int i = 0; i < m_ports; i++) {
			data[i] = GetRawData<float>(inputs, i);
		}

		for (int i = 0; i < count; i++) {
			for (unsigned int j = 0; j < m_ports; j++) {
				m_buffer.push_back(data[j][i]);
			}
		}

		SetRawDataCount(inputs, count);
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