#include "utils.h"
#include <ctime>

namespace utils
{
	std::string make_daytime_string()
	{
		using namespace std; // For time_t, time and ctime;
		time_t now = time(0);
		return ctime(&now);
	}
}
