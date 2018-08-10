#include <oak/block.h>

namespace oak {

	Block::Block()
	{
	}

	Block::~Block()
	{
	}

	SignatureList Block::inputSignatures()
	{
		return m_inputSigs;
	}

	SignatureList Block::outputSignatures()
	{
		return m_outputSigs;
	}

} // namespace oak