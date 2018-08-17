/**
 * Copyright Oak Team 2018
 *
 * @file block.cpp
 * Block类定义.
 *
 * @author pickwick
 * @version 1.0 create
 */

#include <oak/block.h>
#include <oak/tool/std_ext.h>

namespace oak {

	Block::Block()
	{
	}

	Block::Block(SignatureList inputSigs, SignatureList outputSigs)
		: m_inputSigs(inputSigs), m_outputSigs(outputSigs)
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
		return vector_get(
			inputSignatures(), index, Signature::InvalidSigature());
	}

	Signature Block::outputSignature(unsigned int index)
	{
		return vector_get(
			outputSignatures(), index, Signature::InvalidSigature());
	}

	bool IsSigMatchData(const SignatureList & sigs, vector_raw_data * data)
	{
		// 提供数据数量不许比签名多.
		if (data && (data->size() > sigs.size())) {
			return false;
		}

		// 根据签名，依次检查数据是否匹配.
		for (unsigned int i = 0; i < sigs.size(); i ++) {
			auto sig = sigs[i];
			RawData raw;
			if (data != nullptr) {
				raw = vector_get<RawData>(*data, i, RawData());
			}
				
			if (sig.need && raw.data == nullptr) {
				return false;
			}
		}

		return true;
	}

	bool Block::check(vector_raw_data * inputs, vector_raw_data * outputs)
	{
		bool inputMatch = IsSigMatchData(m_inputSigs, inputs);
		bool outputMatch = IsSigMatchData(m_outputSigs, outputs);

		return inputMatch && outputMatch;
	}

} // namespace oak