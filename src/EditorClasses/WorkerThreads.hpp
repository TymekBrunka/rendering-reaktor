#include <condition_variable>
#include <thread>

template <int numof>
class WorkerPool {
private:
  std::thread threads[numof];
  std::condition_variable cvs[numof];
  bool busy[numof];

public:
  WorkerPool() = default;
};
