#include <oak/data_type.h>

namespace oak {

	unsigned int DtSize(DataType type)
	{
		switch (type) {
		case DataType::Char: 
			return 1;

		case DataType::Short: 
			return 2;

		case DataType::Int: 
			return 4;

		case DataType::Real:
			return 4;

		case DataType::Complex:
			return 8;

		default:
			return 0;
		}
	}

} // namespace oak