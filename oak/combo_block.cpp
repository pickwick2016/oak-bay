
#include <algorithm>
#include <assert.h>

#include <oak/combo_block.h>
#include <oak/block_runtime.h>

namespace oak {


	ComboBlock::ComboBlock()
	{
		m_runtime.reset(new BlockRuntime(this));
	}

	ComboBlock::ComboBlock(SignatureList inputSigs, SignatureList outputSigs)
	{
		m_runtime.reset(new BlockRuntime(this));

		m_inputSigs = inputSigs;
		m_outputSigs = outputSigs;
	}

	ComboBlock::~ComboBlock()
	{
		m_blocks.clear();
		m_connections.clear();
	}


	int ComboBlock::work(vector_raw_data * inputs, vector_raw_data * outputs)
	{
		assert(m_runtime.get() != nullptr);

		if (!check(inputs, outputs)) {
			return WorkResult::Error;
		}

		int ret = m_runtime->work(inputs, outputs);

		return ret;
	}

	void ComboBlock::reset()
	{
		assert(m_runtime.get() != nullptr);

		// 模块重置.
		for (auto & block : m_blocks) {
			block->reset();
		}

		//运行环境重置.
		m_runtime->reset();
	}

	bool ComboBlock::setInput(unsigned int index, Port port)
	{
		return connect(Port(this, index), port);
	}

	bool ComboBlock::setOutput(unsigned int index, Port port)
	{
		return connect(port, Port(this, index));
	}

	Block * ComboBlock::add(Block * block)
	{
		if (!contain(block)) {
			if (block) {
				m_blocks.push_back(std::shared_ptr<Block>(block));
			}
		}
		return block;
	}

	void ComboBlock::remove(Block* block)
	{
		// 删除模块.
		auto fit = std::find_if(m_blocks.begin(), m_blocks.end(),
			[=](auto & it) { return it.get() == block; });
		m_blocks.erase(fit);

		// 删除相关连接.
		auto rit = std::remove_if(m_connections.begin(), m_connections.end(),
			[=](auto & it) { return (it.first == block) || (it.second == block); });

		m_connections.erase(rit, m_connections.end());
	}

	bool ComboBlock::contain(Block* block)
	{
		auto fit = std::find_if(m_blocks.begin(), m_blocks.end(),
			[=](auto & it) { return it.get() == block; });

		bool find = (fit != m_blocks.end());
		return find;
	}

	std::vector<Block *> ComboBlock::blocks() const
	{
		std::vector<Block *> blocks;
		for (auto & block : m_blocks) {
			blocks.push_back(block.get());
		}

		return blocks;
	}

	bool ComboBlock::connect(Port source, Port dest)
	{
		if (contain(source, dest)) {
			return true;
		}

		if ((source.block != this && !contain(source.block))
			|| (dest.block != this && !contain(dest.block))) {
			return false;
		}

		if (isEndPortUsed(dest)) {
			return false;
		}

		bool match = allowConnect(source, dest);
		if (match) {
			m_connections.push_back({ source, dest });
		}

		return match;
	}

	void ComboBlock::disconnect(Port source, Port dest)
	{
		if (contain(source, dest)) {
			auto fit = std::remove_if(m_connections.begin(), m_connections.end(),
				[=](auto & it) { return (it.first == source) && (it.second == dest); });

			m_connections.erase(fit, m_connections.end());
		}
	}

	bool ComboBlock::allowConnect(Port source, Port dest)
	{
		if (!source.block || !dest.block || source.block == dest.block) {
			return false;
		}

		auto sourceBlock = makeSubstitute(source.block);
		auto destBlock = makeSubstitute(dest.block);

		auto sourceSig = sourceBlock->outputSignature(source.index);
		auto destSig = destBlock->inputSignature(dest.index);

		bool match = sourceSig.isValid() && destSig.isValid()
			&& sourceSig.type == destSig.type;

		return match;
	}

	bool ComboBlock::isEndPortUsed(Port port)
	{
		for (auto & conn : m_connections) {
			if (conn.second == port) {
				return true;
			}
		}

		return false;
	}

	bool ComboBlock::contain(Port source, Port dest)
	{
		for (auto conn : m_connections) {
			if (conn.first == source && conn.second == dest) {
				return true;
			}
		}

		return false;
	}

	std::vector<std::pair<Port, Port>> ComboBlock::connections() const
	{
		return m_connections;
	}

	bool ComboBlock::checkMap()
	{
		auto blocks = this->blocks();
		blocks.push_back(this);

		// 针对每个模块（不含自身），检查必要连接是否连接上.
		for (auto & block : blocks) {
			auto tempBlock = makeSubstitute(block);

			auto inSigs = tempBlock->inputSignatures();
			for (unsigned int i = 0; i < inSigs.size(); i++) {
				auto fid = std::find_if(m_connections.begin(), m_connections.end(),
					[=](auto & it) { return it.second == Port(block, i); });

				if (inSigs[i].need && fid == m_connections.end()) {
					return false;
				}
			}

			auto outSigs = tempBlock->outputSignatures();
			for (unsigned int i = 0; i < outSigs.size(); i++) {
				auto fid = std::find_if(m_connections.begin(), m_connections.end(),
					[=](auto & it) { return it.first == Port(block, i); });

				if (outSigs[i].need && fid == m_connections.end()) {
					return false;
				}
			}
		}

		return true;
	}

	std::shared_ptr<Block> ComboBlock::makeSubstitute(Block * block)
	{
		assert(block != nullptr);

		std::shared_ptr<Block> ret;

		if (block == this) {
			ret.reset(new Block(this->outputSignatures(), this->inputSignatures()));
		}
		else {
			ret.reset(new Block(block->inputSignatures(), block->outputSignatures()));
		}

		return ret;
	}

	std::vector<Block *> ComboBlock::flatten()
	{
		std::vector<Block *> queue;

		// 根据连接关系，进行队列化（排序）
		for (auto conn : m_connections) {
			if (conn.first.block == this || conn.second.block == this) {
				continue;
			}

			auto fit1 = std::find(queue.begin(), queue.end(), conn.first.block);
			auto fit2 = std::find(queue.begin(), queue.end(), conn.second.block);

			if (fit1 != queue.end() && fit2 != queue.end()) {
				if (std::distance(fit1, fit2) <= 0) {
					queue.clear();
					break;
				}
			}
			else if (fit1 == queue.end() && fit2 == queue.end()) {
				queue.push_back(conn.first.block);
				queue.push_back(conn.second.block);
			}
			else if (fit1 != queue.end() && fit2 == queue.end()) {
				queue.insert(fit1 + 1, conn.second.block);
			}
			else if (fit1 == queue.end() && fit2 != queue.end()) {
				queue.insert(fit2, conn.first.block);
			}
		}

		return queue;
	}

	std::vector<Port> ComboBlock::getEndPorts(Port port)
	{
		std::vector<Port> ret;
		for (auto & conn : m_connections) {
			if (conn.first == port) {
				ret.push_back(conn.second);
			}
		}

		return ret;
	}

	Port ComboBlock::getStartPort(Port port)
	{
		for (auto & conn : m_connections) {
			if (conn.second == port) {
				return conn.first;
			}
		}

		return Port();
	}

	unsigned int ComboBlock::count() const
	{
		return m_blocks.size();
	}

} // namespace oak