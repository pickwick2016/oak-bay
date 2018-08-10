#include <oak/top_block.h>

#include <algorithm>

namespace oak {

	TopBlock::TopBlock() 
	{
		m_flowgraph.reset(new Flowgraph());
	}

	TopBlock::~TopBlock()
	{
		m_flowgraph.reset();
	}

	void TopBlock::add(Block* block) 
	{
		if (block && !contain(block)) {
			m_blocks.push_back(std::shared_ptr<Block>(block));
		}
	}

	void TopBlock::remove(Block* block) 
	{
		auto fit = std::find_if(m_blocks.begin(), m_blocks.end(),
			[=](auto & it) { return it.get() == block; });

		if (fit != m_blocks.end()) {
			m_blocks.erase(fit);
		}
	}

	bool TopBlock::contain(Block* block) 
	{
		auto fit = std::find_if(m_blocks.begin(), m_blocks.end(),
			[=](auto & it) { return it.get() == block; });

		bool find = (fit != m_blocks.end());
		return find;
	}

	int TopBlock::count() const
	{
		return m_blocks.size();
	}

	void TopBlock::connect(Port source, Port dest)
	{
		if (m_flowgraph) {
			m_flowgraph->connect(source, dest);
		}
	}

	void TopBlock::disconnect(Port source, Port dest)
	{
		if (m_flowgraph) {
			m_flowgraph->disconnect(source, dest);
		}
	}

	int TopBlock::start() 
	{
		return 0; 
	}

	void TopBlock::stop()
	{
	}

} // namespace oak