
#include <algorithm>
#include <assert.h>

#include <oak/top_block.h>
#include <oak/block_runtime.h>

namespace oak {

	TopBlock::TopBlock() 
	{
		m_runtime.reset(new BlockRuntime(this));
	}

	TopBlock::~TopBlock()
	{
		m_runtime.reset();
	}

	Block * TopBlock::add(Block* block) 
	{
		if (! contain(block)) {
			if (block) {
				m_blocks.push_back(std::shared_ptr<Block>(block));
			}
		}

		return block;
	}

	void TopBlock::remove(Block* block) 
	{
		// 删除模块.
		auto fit = std::find_if(m_blocks.begin(), m_blocks.end(),
			[=](auto & it) { return it.get() == block; });

		if (fit != m_blocks.end()) {
			m_blocks.erase(fit);
		}

		// TODO:删除相关连接.
		auto rit = std::remove_if(m_connections.begin(), m_connections.end(),
			[=](auto & it) { return (it.first == block) || (it.second == block); });

		m_connections.erase(rit, m_connections.end());
	}

	bool TopBlock::contain(Block* block) 
	{
		auto fit = std::find_if(m_blocks.begin(), m_blocks.end(),
			[=](auto & it) { return it.get() == block; });

		bool find = (fit != m_blocks.end());
		return find;
	}

	unsigned int TopBlock::count() const
	{
		return m_blocks.size();
	}

	std::vector<Block *> TopBlock::blocks()
	{
		std::vector<Block *> blocks;
		for (auto & block : m_blocks) {
			blocks.push_back(block.get());
		}

		return blocks;
	}
	
	bool TopBlock::isMatch(Port source, Port dest)
	{
		if (!source.block || !dest.block) {
			return false;
		}

		bool match = false;

		auto fromSigs = source.block->outputSignatures();
		unsigned int fromPort = source.index;

		auto toSigs = dest.block->inputSignatures();
		unsigned int toPort = dest.index;

		if (fromPort < fromSigs.size() && toPort < toSigs.size()) {
			if (fromSigs[fromPort].type == toSigs[toPort].type) {
				match = true;
			}
		}

		return match;
	}

	bool TopBlock::connect(Port source, Port dest)
	{
		bool noBlocks = !contain(source.block) || !contain(dest.block);
		bool sameBlock = (source.block == dest.block);
		
		if (noBlocks || sameBlock) {
			return false;
		}

		if (contain(source, dest)) {
			return true;
		}
		
		bool match = isMatch(source, dest);
		if (match) {
			m_connections.push_back({ source, dest });
		}

		return match;
	}

	void TopBlock::disconnect(Port source, Port dest)
	{
		if (contain(source, dest)) {
			auto fit = std::find_if(m_connections.begin(), m_connections.end(),
				[=](auto & it) { return (it.first == source) && (it.second == dest); });
			
			if (fit != m_connections.end()) {
				m_connections.erase(fit);
			}
		}
	}


	bool TopBlock::contain(Port source, Port dest)
	{
		for (auto conn : m_connections) {
			if (conn.first == source && conn.second == dest) {
				return true;
			}
		}

		return false;
	}

	std::vector<std::pair<Port, Port>> TopBlock::connections() const
	{
		return m_connections;
	}

	int TopBlock::work(vector_raw_data & inputs, vector_raw_data & outputs)
	{
		assert(m_runtime != nullptr);

		if (m_runtime->validate()) {
			int ret = m_runtime->work();
			return ret;
		}
		
		return WorkResult::Error;
	}

	
	int TopBlock::start() 
	{
		assert(m_runtime != nullptr);

		return 0; 
	}

	void TopBlock::stop()
	{
	}

	std::vector<Port> TopBlock::getDestPorts(Port source)
	{
		std::vector<Port> ret;
		for (auto conn : m_connections) {
			if (conn.first == source) {
				ret.push_back(conn.second);
			}
		}

		return ret;
	}

	Port TopBlock::getSourcePort(Port dest)
	{
		for (auto conn : m_connections) {
			if (conn.second == dest) {
				return conn.first;
			}
		}

		return Port(nullptr, 0);
	}

} // namespace oak