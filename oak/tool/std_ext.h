#pragma once

#include <vector>

namespace oak {

	// �������а�ȫ��ȡ����.
	template <class T>
	T vector_get(const std::vector<T> & vec, unsigned int index, T val) 
	{
		if (index >= vec.size()) {
			return val;
		}

		return vec[index];
	}

	// �ж��������Ƿ���ָ��Ԫ��.
	template <class T>
	bool vector_has(const std::vector<T> & vec, T val)
	{
		auto it = std::find(vec.begin(), vec.end(), val);
		return it != vec.end();
	}

	// ����ɾ��.
	template <class T>
	void vector_erase(std::vector<T> & vec, T val)
	{
		auto it = std::remove(vec.begin(), vec.end(), val);
		vec.erase(it, vec.end());
	}


} // namespace oak