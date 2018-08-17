#include <oak/tool/math.h>

namespace oak {

	int RoundUp(int val, int m)
	{
		return (val + m - 1) / m * m;
	}

	int RoundDown(int val, int m)
	{
		return val / m * m;
	}

} // namespace oak