#include <condition_variable>
#include <thread>

enum worker_result {
  WORKER_SUCCESS,
  WORKER_IDLE,
  WORKER_WORKING,
  WORKER_CANCELED,
  WORKER_FAIL,
};

void loop() {
  while (true) {
  }
}

template <int numof>
class WorkerPool {
private:
  bool isMovedOut = false;
  std::thread threads[numof];
  std::condition_variable cvs[numof];
  bool busy[numof];

  void (*callbacks[numof])();
  void (*tasks[numof])();

public:
  WorkerPool(const char *x) : isMovedOut(false) {
    for (int i = 0; i < numof; i++) {
      this->threads[i] = new std::thread(loop);
      this->condition_variable[i] = new std::condition_variable;
      this->busy[i] = false;
      this->callbacks[i] = nullptr;
      this->tasks[i] = nullptr;
   }
  }

  ~WorkerPool() {
    for (int i = 0; i < numof; i++) {
      this->threads[i].join();
    }
  }

  int execute() {
    for (int i = 0; i < numof; i++) {

    }
  }
};
