#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

// concurrency stuff
std::mutex mutex_;
bool ready_{true};
std::condition_variable cvTrue_;
std::condition_variable cvFalse_;

// values for ping pong
bool value_ = false;
std::atomic<int> iterations_{0};


void setToTrue(){
while (iterations_ < 50) {
    std::unique_lock<std::mutex> myLock(mutex_);
    cvTrue_.wait(myLock, []{ return ready_ == true; });
    std::cout << "setToTrue notified, iterations=" << iterations_ << ", value=" << value_ << std::endl;
    // critical section
    value_ = true;
    ready_ = false;
    iterations_++;
  cvFalse_.notify_one();
  }
}

void setToFalse(){
while (iterations_ < 50) {
    std::unique_lock<std::mutex> myLock(mutex_);
    cvFalse_.wait(myLock, []{ return ready_ == false; });
    std::cout << "setToFalse notified, iterations=" << iterations_ << ", value=" << value_ << std::endl;
    // critical section
    value_ = false;
    ready_ = true;
    iterations_++;
  cvTrue_.notify_one();
  }
}

int main(){
    
  std::cout << std::endl;

  std::thread t1(setToFalse);
  std::thread t2(setToTrue);

  t1.join();
  t2.join();
  
  return 0;
}

