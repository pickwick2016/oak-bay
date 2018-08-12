/**
 * Copyright Oak Team 2018
 *
 * @file block.cpp
 * Block¿‡∂®“Â.
 *
 * @author pickwick
 * @version 1.0 create
 */

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


	Signature Block::inputSignature(unsigned int index)
	{
		if (index < m_inputSigs.size()) {
			return m_inputSigs[index];
		}

		return Signature(DataType::Unknown, false, -1);
	}

	Signature Block::outputSignature(unsigned int index)
	{
		if (index < m_outputSigs.size()) {
			return m_outputSigs[index];
		}

		return Signature(DataType::Unknown, false, -1);
	}

} // namespace oak