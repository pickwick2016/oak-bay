#pragma once

#include <vector>

namespace oak {

	// 从数组中安全获取数据.
	template <class T>
	T vector_get(const std::vector<T> & vec, unsigned int index, T val) 
	{
		if (index >= vec.size()) {
			return val;
		}

		return vec[index];
	}

	// 判断数组中是否含有指定元素.
	template <class T>
	bool vector_has(const std::vector<T> & vec, T val)
	{
		auto it = std::find(vec.begin(), vec.end(), val);
		return it != vec.end();
	}

	// 数组删除.
	template <class T>
	void vector_erase(std::vector<T> & vec, T val)
	{
		auto it = std::remove(vec.begin(), vec.end(), val);
		vec.erase(it, vec.end());
	}


} // namespace oak