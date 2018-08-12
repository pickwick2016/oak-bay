#include <algorithm>
#include <set>
#include <iterator>

#include <boost/bind.hpp>

#include <oak/block_runtime.h>
#include <oak/buffer.h>
#include <oak/top_block.h>

namespace oak {

	BlockRuntime::BlockRuntime(TopBlock * parent)
		: m_parent(parent)
	{
	}

	int BlockRuntime::work()
	{
		std::vector<Block *> blocks = flatten();
		if (! blocks.empty()) {
			std::vector<int> results(blocks.size(), WorkResult::Error);
			for (unsigned int i = 0; i < blocks.size(); i++) {
				results[i] = runBlock(blocks[i]);
				if (results[i] == WorkResult::Error) {
					return WorkResult::Error;
				}
			}

			return WorkResult::Ok;
		}

		return WorkResult::Error;

	}

	void BlockRuntime::resetBuffers()
	{
		m_inputBuffers.clear();
		m_outputBuffers.clear();

		std::set<Port> outputPorts, inputPorts;

		auto connections = m_parent->connections();
		for (auto conn : connections) {
			outputPorts.insert(conn.first);
			inputPorts.insert(conn.second);
		}

		for (auto port : outputPorts) {
			auto sig = port.block->outputSignature(port.index);
			m_outputBuffers[port] = std::shared_ptr<VecBuffer>(new VecBuffer(sig.type));
		}

		for (auto port : inputPorts) {
			auto sig = port.block->inputSignature(port.index);
			m_inputBuffers[port] = std::shared_ptr<VecBuffer>(new VecBuffer(sig.type));
		}
	}

	bool BlockRuntime::compareBlock(Block * block1, Block * block2)
	{
		for (auto conn : m_connections) {
			if (conn.first == block1 && conn.second == block2) {
				return true;
			}
		}

		return false;
	}


	std::vector<Block*> BlockRuntime::flatten()
	{
		// 检查状态是否变化.
		auto connections = m_parent->connections();
		if (m_connections == connections) {
			return m_queue;
		}
		
		// 根据连接关系，进行队列化（排序）
		m_connections = connections;
		m_queue.clear();

		std::set<Block *> tempBlocks;
		for (auto conn : m_connections) {
			tempBlocks.insert(conn.first.block);
			tempBlocks.insert(conn.second.block);
		}
		
		std::copy(tempBlocks.begin(), tempBlocks.end(), std::back_inserter(m_queue));
		std::sort(m_queue.begin(), m_queue.end(), 
			boost::bind(&BlockRuntime::compareBlock, this, _1, _2));

		// 检查排序结果是否错误.
		for (auto conn : m_connections) {
			auto fit1 = std::find(m_queue.begin(), m_queue.end(), conn.first.block);
			auto fit2 = std::find(m_queue.begin(), m_queue.end(), conn.second.block);

			if (std::distance(fit1, fit2) <= 0) {
				// 出现顺序不一致的情况
				m_queue.clear();
				break;
			}
		}
		
		// 重新设置缓冲区.
		resetBuffers();

		return m_queue;
	}

	bool IsSignatureMatchBuffer(SignatureList sigs, vector_raw_data data)
	{
		for (int i = 0; i < sigs.size(); i++) {
			if (sigs[i].need == true) {
				bool hasSlot = (data.size() > i);
				if (!hasSlot || data[i].data == nullptr) {
					return false;
				}
			}
		}

		return true;
	}

	int BlockRuntime::runBlock(Block * block)
	{
		BlockState state;
		if (prepareBlock(block, state)) {
			int ret = block->work(state.inputs, state.outputs);
			updateBlock(block, state);
			return ret;
		}

		return WorkResult::Error;
	}

	bool BlockRuntime::prepareBlock(Block * block, BlockRuntime::BlockState & state)
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

	void BlockRuntime::updateBlock(Block * block, const BlockRuntime::BlockState & state)
	{
		//1.根据处理结果，更新输入/输出缓冲区
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
		
		//2.模块缓冲区之间传递数据.
		for (unsigned int i = 0; i < state.outputs.size(); i++) {
			Port currPort(block, i);
			FifoBuffer * source = getOutputBuffer(currPort);
			if (source == nullptr) {
				continue;
			}

			// 计算转移数据量.
			int transCount = source->inputCount();

			auto destPorts = m_parent->getDestPorts(currPort);
			std::vector<FifoBuffer *> buffers = getInputBuffers(destPorts);
			for (auto buf : buffers) {
				transCount = std::min<int>(transCount, buf->outputCount());
			}
			
			// 转移数据.
			if (transCount > 0) {
				for (auto buf : buffers) {
					buf->push(source->inputData(), transCount);
				}
				source->pop(transCount);
			}
		}
	}

	FifoBuffer * BlockRuntime::getInputBuffer(Port port)
	{
		auto fit = m_inputBuffers.find(port);
		if (fit != m_inputBuffers.end()) {
			return (*fit).second.get();
		}

		return nullptr;
	}

	FifoBuffer * BlockRuntime::getOutputBuffer(Port port)
	{
		auto fit = m_outputBuffers.find(port);
		if (fit != m_outputBuffers.end()) {
			return (*fit).second.get();
		}

		return nullptr;
	}

	std::vector<FifoBuffer *> BlockRuntime::getInputBuffers(const std::vector<Port> & ports)
	{
		std::vector<FifoBuffer *> ret;
		for (auto port : ports) {
			auto buf = getInputBuffer(port);
			ret.push_back(buf);
		}

		return ret;
	}

} // namespace oak