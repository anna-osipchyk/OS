#include <vector>
#include <mutex>
#include <condition_variable>
#include <memory>

struct threadInfo
{
	int size;
	int idx;
	bool actions[2];
	std::condition_variable start;
	std::mutex actionMutex;
};
