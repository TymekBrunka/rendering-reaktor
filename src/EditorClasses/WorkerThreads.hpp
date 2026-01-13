#include <condition_variable>
#include <functional>
#include <thread>

enum worker_status {
  WORKER_SUCCESS,
  WORKER_IDLE,
  WORKER_WORKING,
  WORKER_CANCELED,
  WORKER_FAIL,
};

class Worker;
void loop(Worker *worker);

class Worker {
private:
  friend void loop(Worker *worker);
  worker_status status;
  std::thread thread;
  std::condition_variable cv;
  void (*callback)();
  void (*task)();

public:
  Worker() {}

  Worker(const char *x) {
    // this->cv = new std::condition_variable;
    // this->thread = std::thread(loop, this);
    status = WORKER_IDLE;
    this->callback = nullptr;
    this->task = nullptr;
  }

  Worker(Worker&& other) {
    
  }
};

inline void loop(Worker *worker) {
  while (true) {
  }
}

template <int numof> class WorkerPool {
private:
  // bool isMovedOut = false;
  Worker workers[numof];

public:
  WorkerPool(const char *x) {
    for (int i = 0; i < numof; i++) {
      this->workers[i] = new Worker();
    }
  }

  ~WorkerPool() {
    for (int i = 0; i < numof; i++) {
    }
  }

  int execute() {
    for (int i = 0; i < numof; i++) {
    }
  }
};
