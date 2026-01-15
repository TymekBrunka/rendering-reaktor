#pragma once
#include "utils/Logger.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>

enum worker_status {
  WORKER_SUCCESS,
  WORKER_PROCEED,
  WORKER_PLZ_DIE,
  WORKER_CANCELED,
  WORKER_FAIL,
};

inline std::mutex cv_mutex;

class Worker;
void loop(Worker *worker);
inline void empty_handler(worker_status status) {}

typedef void (*PostWorkHandler)(worker_status);
typedef void (*Task)();

class Worker {
private:
  bool isMovedOut = true;

public:
  // friend void loop(Worker *worker);
  worker_status status;
  bool busy = false;
  std::thread thread;
  std::condition_variable cv;
  Task task;
  PostWorkHandler postWorkHandler;

  // not meant to copy, only moving bc cpp doesnt use regular constuctor in object name = Object(); notation
  Worker &operator=(Worker &other) noexcept {
    if (this != &other) {
      this->thread = std::move(other.thread);
      this->isMovedOut = false;
      other.isMovedOut = true;
    }
    return *this;
  }
  // Worker() {}

  Worker() : isMovedOut(false), status(WORKER_SUCCESS), postWorkHandler(empty_handler), task(nullptr) {
    // this->cv = new std::condition_variable;
    // this->thread = std::thread(loop, this);
  }

  ~Worker() {
    if (!this->isMovedOut) {
      this->status = WORKER_PLZ_DIE; // idk, may couse race conditions but we want to exit anyway
      this->cv.notify_all();
      // Logger<>::warn("WORKER") << "PLZ die already, we need to exit.\n";
      this->thread.join();
    }
  }

  void operator~() {
    if (!this->isMovedOut) {
      this->status = WORKER_PLZ_DIE; // idk, may couse race conditions but we want to exit anyway
      this->cv.notify_all();
      // Logger<>::warn("WORKER") << "PLZ die already, we need to exit.\n";
      this->thread.join();
    }
  }

  void run() { this->thread = std::thread(loop, this); }
};

inline void loop(Worker *worker) {
  std::unique_lock lk(cv_mutex);
  while (true) {
    worker->cv.wait(lk, [worker] { return worker->status == WORKER_PROCEED || worker->status == WORKER_PLZ_DIE; });
    if (worker->status == WORKER_PLZ_DIE) break;
    worker->status = WORKER_SUCCESS;
    // worker->busy = true;
    worker->task();
    worker->busy = false;
  }
}

template <int numof> class WorkerPool {
private:
  // bool isMovedOut = false;
  bool isMovedOut = true;
  Worker workers[numof];

public:
  WorkerPool() = default;

  WorkerPool(const char *x) {
    for (int i = 0; i < numof; i++) {
      Worker worker;
      this->workers[i] = worker;
      this->workers[i].run();
    }
  }

  ~WorkerPool() {
    for (int i = 0; i < numof; i++) {
      ~this->workers[i];
    }
  };

  int execute(Task task, PostWorkHandler postWorkHandler) {
    for (int i = 0; i < numof; i++) {
      if (!this->workers[i].busy) {
        this->workers[i].status = WORKER_PROCEED;
        this->workers[i].task = task;
        this->workers[i].postWorkHandler = postWorkHandler;
        this->workers[i].busy = true;
        this->workers[i].cv.notify_all();
        Logger<>::info("POOL") << "Executing task on worker [" << i << "/" << numof << "]\n";
        return i;
      }
    }
    Logger<>::info("POOL") << "No free worker threads found\n";
    return -1;
  }

  void handle(int single) {
    if (!this->workers[single].busy) {
      this->workers[single].postWorkHandler(this->workers[single].status);
      this->workers[single].postWorkHandler = empty_handler;
    }
  }

  void handle() {
    for (int single = 0; single < numof; single++) {
      if (!this->workers[single].busy) {
        this->workers[single].postWorkHandler(this->workers[single].status);
        this->workers[single].postWorkHandler = empty_handler;
      }
    }
  }
};

inline WorkerPool<5> *workers;
