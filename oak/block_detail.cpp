/**
 * Copyright Oak Team 2018
 *
 * @file block_detail.cpp
 * Block�ඨ��.
 *
 * @author pickwick
 * @version 1.0 create
 */

#include <oak/block_detail.h>
#include <oak/block.h>

namespace oak {

	DataType GetInputPortType(Port port)
	{
		if (port.block) {
			auto sigs = port.block->inputSignatures();
			if (port.index < sigs.size()) {
				return sigs[port.index].type;
			}
		}

		return DataType::Unknown;
	}

	DataType GetOutputPortType(Port port)
	{
		if (port.block) {
			auto sigs = port.block->outputSignatures();
			if (port.index < sigs.size()) {
				return sigs[port.index].type;
			}
		}

		return DataType::Unknown;
	}

	Port::Port(Block * block, unsigned int index)
	{
		this->block = block;
		this->index = index;
	}

	bool Port::operator == (const Port & rhs) const
	{
		return block == rhs.block && index == rhs.index;
	}

	bool Port::operator < (const Port & rhs) const
	{
		int value1 = (int)((void *)block) + index;
		int value2 = (int)((void *)rhs.block) + rhs.index;

		return value1 < value2;
	}

	Signature::Signature(DataType type, bool need, int count)
	{
		this->type = type;
		this->need = need;
		this->count = count;
	}

} // namespace oak