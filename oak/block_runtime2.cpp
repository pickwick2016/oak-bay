#include <assert.h>
#include <algorithm>

#include <oak/block_runtime2.h>
#include <oak/combo_block.h>
#include <oak/buffer.h>
#include <oak/tool/math.h>

namespace oak {

	BlockRuntime2::BlockRuntime2(ComboBlock * parent, unsigned int countHint)
		: m_parent(parent)
	{
		assert(m_parent);

		m_bufferCountHint = std::max<unsigned int>(1, countHint);

	}

	BlockRuntime2::~BlockRuntime2()
	{
	}


	int BlockRuntime2::work(vector_raw_data * inputs, vector_raw_data * outputs)
	{
		assert(m_parent);

		// �������״̬�ͻ����Ƿ�׼���׵�.
		if (!validate()) {
			return WorkResult::Error;
		}

		// ��������.
		if (inputs) {
			auto inputSigs = m_parent->inputSignatures();
			for (unsigned int i = 0; i < inputSigs.size(); i++) {
				int inputCount = (*inputs)[i].count;
				void * inputData = (*inputs)[i].data;
				FifoBuffer * buffer = getOutputBuffer({m_parent, i});
			
				if (buffer && inputData) {
					int count = std::min<int>(inputCount, buffer->outputCount());
				
					buffer->push(inputData, inputCount);
					(*inputs)[i].count = count;
				}
				else {
					(*inputs)[i].count = 0;
				}
			}
		}

		// ���δ�������ģ��.
		std::vector<int> results(m_queue.size(), WorkResult::Error);
		for (unsigned int i = 0; i < m_queue.size(); i ++) {
			auto block = m_queue[i];

			BlockState state;
			if (!prepareBlock(block, state))
				return WorkResult::Error;

			results[i] = runBlock(block, state);

			updateBlock(block, state);
		}

		int ret = MergeResults(results);

		// �����������.
		if (outputs) {
			auto outputSigs = m_parent->outputSignatures();
			for (unsigned int i = 0; i < outputSigs.size(); i++) {
				int outputCount = (*outputs)[i].count;
				void * outputData = (*outputs)[i].data;
				FifoBuffer * buffer = getInputBuffer({ m_parent, i });

				if (buffer && outputData) {
					int count = std::min<int>(outputCount, buffer->inputCount());

					buffer->pop(count, outputData);
					(*outputs)[i].count = count;
				}
				else {
					(*outputs)[i].count = 0;
				}
			}
		}

		return ret;
	}
	

	FifoBuffer * BlockRuntime2::getInputBuffer(Port port)
	{
		auto fit = m_inputBuffers.find(port);
		if (fit != m_inputBuffers.end()) {
			return m_inputBuffers[port].get();
		}

		return nullptr;
	}

	std::vector<FifoBuffer *> BlockRuntime2::getInputBuffers(const std::vector<Port> & ports)
	{
		std::vector<FifoBuffer *> ret;
		for (auto port : ports) {
			auto buf = getInputBuffer(port);
			ret.push_back(buf);
		}

		return ret;
	}

	FifoBuffer * BlockRuntime2::getOutputBuffer(Port port)
	{
		auto fit = m_outputBuffers.find(port);
		if (fit != m_outputBuffers.end()) {
			return m_outputBuffers[port].get();
		}

		return nullptr;
	}

	bool BlockRuntime2::isChanged()
	{
		auto connections = m_parent->connections();
		if (m_connections != connections) {
			m_connections = connections;
			return true;
		}

		return false;
	}

	bool BlockRuntime2::validate()
	{
		if (isChanged()) {
			reset();
		}

		return ! m_queue.empty();
	}

	void BlockRuntime2::reset()
	{
		// �ؽ�����״̬.
		m_inputBuffers.clear();
		m_outputBuffers.clear();

		m_queue.clear();

		// �ؽ��������.
		if (m_connections.empty()|| !m_parent->checkMap()) {
			return;
		}

		m_queue = m_parent->flatten();
		if (m_queue.empty()) {
			return;
		}
		
		// �ؽ�������.
		for (auto conn : m_connections) {
			auto sourceBlock = conn.first.block;
			auto destBlock = conn.second.block;

			DataType datatype = sourceBlock->inputSignature(conn.first.index).type;
			auto endPorts = getEndPorts(conn.first);
			assert(!endPorts.empty());

			unsigned int count = 1024; // TODO: ������С.

			auto outputBuf = makeBuffer(datatype, count);
			m_outputBuffers[conn.first] = outputBuf;

			if (endPorts.size() > 1) {
				for (auto port : endPorts) {
					auto inputBuf = makeBuffer(datatype, count);
					m_inputBuffers[port] = inputBuf;
				}
			}
			else {
				m_inputBuffers[conn.second] = outputBuf;
			}
		}
	}

	std::vector<Port> BlockRuntime2::getEndPorts(Port start)
	{
		std::vector<Port> ret;
		for (auto conn : m_connections) {
			if (conn.first == start) {
				ret.push_back(conn.second);
			}
		}

		return ret;
	}

	int BlockRuntime2::runBlock(Block * block, BlockRuntime2::BlockState & state)
	{
		return block->work(&state.inputs, &state.outputs);
	}

	bool BlockRuntime2::prepareBlock(Block * block, BlockState & state)
	{
		state.inputs.resize(block->inputSignatures().size());
		for (unsigned int i = 0; i < state.inputs.size(); i++) {
			auto sig = block->inputSignature(i);
			FifoBuffer * buffer = getInputBuffer(Port(block, i));

			if (sig.need && buffer == nullptr)
				return false;

			if (buffer != nullptr) {
				state.inputs[i].count = buffer->inputCount();
				state.inputs[i].data = buffer->inputData();
			}
		}

		state.outputs.resize(block->outputSignatures().size());
		for (unsigned int i = 0; i < state.outputs.size(); i++) {
			auto sig = block->outputSignature(i);
			FifoBuffer * buffer = getOutputBuffer(Port(block, i));

			if (sig.need && buffer == nullptr)
				return false;

			if (buffer != nullptr) {
				state.outputs[i].count = buffer->outputCount();
				state.outputs[i].data = buffer->outputData();
			}
		}

		return true;
	}

	void BlockRuntime2::updateBlock(Block * block, const BlockState & state)
	{
		//1.���ݴ���������������/���������
		for (unsigned int i = 0; i < state.inputs.size(); i++) {
			if (FifoBuffer * buffer = getInputBuffer(Port(block, i))) {
				buffer->pop(state.inputs[i].count);
			}
		}

		for (unsigned int i = 0; i < state.outputs.size(); i++) {
			if (FifoBuffer * buffer = getOutputBuffer(Port(block, i))) {
				buffer->push(state.outputs[i].data, state.outputs[i].count);
			}
		}

		//2.ģ�黺����֮�䴫������.
		for (unsigned int i = 0; i < state.outputs.size(); i++) {
			Port currPort(block, i);
			FifoBuffer * source = getOutputBuffer(currPort);
			if (source == nullptr) {
				continue;
			}

			// ת������.
			auto destPorts = m_parent->getEndPorts(currPort);
			if (destPorts.size() > 1) {
				// ����ת��������.
				int transCount = source->inputCount();
				std::vector<FifoBuffer *> buffers = getInputBuffers(destPorts);
				for (auto buf : buffers) {
					transCount = std::min<int>(transCount, buf->outputCount());
				}

				// ת������.
				if (transCount > 0) {
					for (auto buf : buffers) {
						buf->push(source->inputData(), transCount);
					}
					source->pop(transCount);
				}
			}
		}
	}

	std::shared_ptr<FifoBuffer> BlockRuntime2::makeBuffer(DataType datatype, unsigned int count)
	{
		int bufferCount = std::max<int>(1, count);
		bufferCount = RoundUp(bufferCount, m_bufferCountHint);

		std::shared_ptr<FifoBuffer> buffer(new VecBuffer(datatype, bufferCount));
		return buffer;
	}

} // namespace oak