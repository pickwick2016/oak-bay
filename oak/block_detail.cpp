/**
 * Copyright Oak Team 2018
 *
 * @file block_detail.cpp
 * Block¿‡∂®“Â.
 *
 * @author pickwick
 * @version 1.0 create
 */

#include <algorithm>

#include <oak/block_detail.h>
#include <oak/block.h>
#include <oak/buffer.h>

namespace oak {


	Signature Signature::InvalidSigature()
	{
		return Signature(DataType::Unknown, true, 0);
	}

	int MergeResults(const std::vector<int> & results)
	{
		if (results.empty()) {
			return WorkResult::Error;
		}
		
		int error = std::count(results.begin(), results.end(), WorkResult::Error);
		if (error > 0)
			return WorkResult::Error;

		int finish = std::count(results.begin(), results.end(), WorkResult::Finish); 
		int wait = std::count(results.begin(), results.end(), WorkResult::Wait); 
		int ok = std::count(results.begin(), results.end(), WorkResult::Ok);

		if (finish > 0 && (finish + wait) == results.size()) {
			return WorkResult::Finish;
		}

		if (wait > 0 && (wait + ok) == results.size()) {
			return WorkResult::Wait;
		}

		return WorkResult::Ok;
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
		int value1 = (int)((void *)block) * 10 + index;
		int value2 = (int)((void *)rhs.block) * 10 + rhs.index;

		return value1 < value2;
	}

	Signature::Signature(DataType type, bool need, int count)
	{
		this->type = type;
		this->need = need;
		this->count = count;
	}

	bool Signature::isValid() const
	{
		return ! (count < 0 && type == DataType::Unknown);
	}

	int GetRawDataCount(vector_raw_data & data, unsigned int index)
	{
		if (index < data.size()) {
			return data[index].count;
		}

		return -1;
	}

	int GetRawDataCount(vector_raw_data * data, unsigned int index)
	{
		if (data != nullptr) {
			if (index < (*data).size()) {
				return (*data)[index].count;
			}
		}
		
		return -1;
	}

	int SetRawDataCount(vector_raw_data * data, unsigned int index, int count)
	{
		if (data != nullptr) {
			if (index < (*data).size()) {
				(*data)[index].count = count;
				return count;
			}
		}

		return -1;
	}

	int GetRawDataMinCount(vector_raw_data * data)
	{
		if (data == nullptr) {
			return -1;
		}

		bool init = false;
		int ret = 0;
		for (unsigned int i = 0; i < data->size(); i++) {
			RawData raw = (*data)[i];
			if (raw.data != nullptr && raw.count >= 0) {
				ret = init ? std::min<int>(ret, raw.count) : raw.count;
				init = true;
			}
		}

		return ret;
	}


	int SetRawDataCount(vector_raw_data * data, int count)
	{
		if (data == nullptr) {
			return -1;
		}

		for (unsigned int i = 0; i < data->size(); i++) {
			RawData & raw = (*data)[i];
			if (raw.data != nullptr) {
				raw.count = count;
			}
		}

		return count;
	}

	vector_raw_data MakeOutputRawData(const std::vector<FifoBuffer *> & buffers)
	{
		vector_raw_data ret(buffers.size());
		for (unsigned int i = 0; i < buffers.size(); i++) {
			if (buffers[i] != nullptr) {
				ret[i].count = buffers[i]->outputCount();
				ret[i].data = buffers[i]->outputData();
			}
		}

		return ret;
	}

	vector_raw_data MakeInputRawData(const std::vector<FifoBuffer *> & buffers)
	{
		vector_raw_data ret(buffers.size());
		for (unsigned int i = 0; i < buffers.size(); i++) {
			if (buffers[i] != nullptr) {
				ret[i].count = buffers[i]->inputCount();
				ret[i].data = buffers[i]->inputData();
			}
		}

		return ret;
	}

} // namespace oak