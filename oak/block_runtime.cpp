#include <algorithm>
#include <set>
#include <iterator>
#include <assert.h>

#include <boost/bind.hpp>

#include <oak/block_runtime.h>
#include <oak/buffer.h>
#include <oak/top_block.h>
#include <oak/tool/math.h>

namespace oak {

	BlockRuntime::BlockRuntime(TopBlock * parent, unsigned int countHint)
		: m_parent(parent)
	{
		m_bufferCountHint = std::max<int>(1, countHint);
	}

	BlockRuntime::~BlockRuntime()
	{
		m_inputBuffers.clear();
		m_outputBuffers.clear();
	}

	void BlockRuntime::reset()
	{
		m_queue.clear();
		m_connections.clear();
		
		validate();
	}


	bool BlockRuntime::validate()
	{
		assert(m_parent);

		// 检查状态是否变化.
		auto connections = m_parent->connections();
		if (m_connections == connections) {
			return ! m_queue.empty();
		}

		if (m_parent->checkGraph()) {
			m_connections = connections;

			auto blocks = flatten();
			if (! blocks.empty()) {
				// 重新设置缓冲区.
				setupBuffers();
				return true;
			}
		}

		return false;
	}

	int BlockRuntime::work()
	{
		assert(!m_queue.empty());

		std::vector<int> results(m_queue.size(), WorkResult::Error);
		for (unsigned int i = 0; i < m_queue.size(); i++) {
			results[i] = runBlock(m_queue[i]);
			if (results[i] == WorkResult::Error) {
				return WorkResult::Error;
			}
		}

		int ret = MergeResults(results);
		return ret;
	}

	bool BlockRuntime::setupBuffers()
	{
		m_inputBuffers.clear();
		m_outputBuffers.clear();

		// 1.记录输入、输出端口.
		std::set<Port> sourcePorts;
		auto connections = m_parent->connections();
		for (auto conn : connections) {
			sourcePorts.insert(conn.first);
		}

		// 2.设置输入/输出端口的缓冲区.
		for (auto port : sourcePorts) {
			auto sig = port.block->outputSignature(port.index);
			auto destPorts = m_parent->getDestPorts(port);
			
			// 确定缓冲区大小.
			int bufferCount = std::max<int>(1, sig.count);
			for (auto dest : destPorts) {
				auto sig2 = dest.block->inputSignature(dest.index);
				bufferCount = std::max<int>(bufferCount, sig2.count);
			}

			// 分配source端缓冲区.
			std::shared_ptr<FifoBuffer> buf = makeBuffer(sig.type, bufferCount);
			m_outputBuffers[port] = buf;

			// 分配dest端缓冲区.
			if (destPorts.size() == 1) {
				m_inputBuffers[destPorts[0]] = buf;
			}
			else {
				for (auto dest : destPorts) {
					m_inputBuffers[dest] = makeBuffer(sig.type, bufferCount);
				}
			}			
		}

		return true;
	}

	std::vector<Block*> BlockRuntime::flatten()
	{
		// 根据连接关系，进行队列化（排序）
		m_queue.clear();


		for (auto conn : m_connections) {
			auto fit1 = std::find(m_queue.begin(), m_queue.end(), conn.first.block);
			auto fit2 = std::find(m_queue.begin(), m_queue.end(), conn.second.block);

			if (fit1 != m_queue.end() && fit2 != m_queue.end()) {
				if (std::distance(fit1, fit2) <= 0) {
					m_queue.clear();
					break;
				}
			}
			else if (fit1 == m_queue.end() && fit2 == m_queue.end()) {
				m_queue.push_back(conn.first.block);
				m_queue.push_back(conn.second.block);
			}
			else if (fit1 != m_queue.end() && fit2 == m_queue.end()) {
				m_queue.insert(fit1 + 1, conn.second.block);
			}
			else if (fit1 == m_queue.end() && fit2 != m_queue.end()) {
				m_queue.insert(fit2, conn.first.block);
			}
		}


		//// 获取所有模块.
		//std::set<Block *> tempBlocks;
		//for (auto conn : m_connections) {
		//	tempBlocks.insert(conn.first.block);
		//	tempBlocks.insert(conn.second.block);
		//}
		//std::copy(tempBlocks.begin(), tempBlocks.end(), std::back_inserter(m_queue));

		//// 排序.
		//std::sort(m_queue.begin(), m_queue.end(), 
		//	boost::bind(&BlockRuntime::compareBlock, this, _1, _2));

		//// 检查排序结果是否错误.
		//for (auto conn : m_connections) {
		//	auto fit1 = std::find(m_queue.begin(), m_queue.end(), conn.first.block);
		//	auto fit2 = std::find(m_queue.begin(), m_queue.end(), conn.second.block);

		//	if (std::distance(fit1, fit2) <= 0) {
		//		// 出现顺序不一致的情况
		//		m_queue.clear();
		//		break;
		//	}
		//}
		
		return m_queue;
	}

	bool IsSignatureMatchBuffer(SignatureList sigs, vector_raw_data data)
	{
		for (unsigned int i = 0; i < sigs.size(); i++) {
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
			int ret = block->work(& state.inputs, & state.outputs);
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

			// 转移数据.
			auto destPorts = m_parent->getDestPorts(currPort);
			if (destPorts.size() > 1) {
				// 计算转移数据量.
				int transCount = source->inputCount();
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

	std::shared_ptr<FifoBuffer> BlockRuntime::makeBuffer(DataType datatype, int count)
	{
		int bufferCount = std::max<int>(1, count);
		bufferCount = RoundUp(bufferCount, m_bufferCountHint);

		std::shared_ptr<FifoBuffer> buffer(new VecBuffer(datatype, bufferCount));
		return buffer;
	}

} // namespace oak