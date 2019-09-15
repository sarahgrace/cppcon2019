#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

static const int NUM = 100000000;

long long getDotProduct(std::vector<int>& v, std::vector<int>& w, int startOffset, int endOffset) {
  return std::inner_product(
    v.begin() + startOffset, 
    v.begin() + endOffset,
    w.begin() + startOffset, 
    0LL
  );
}

int main(){

  std::cout << std::endl;

  // get NUM random numbers from 0 .. 100
  std::random_device seed;

  // generator
  std::mt19937 engine(seed());

  // distribution
  std::uniform_int_distribution<int> dist(0,100);

  // fill the vectors
  std::vector<int> v, w;
  v.reserve(NUM);
  w.reserve(NUM);
  for (int i=0; i< NUM; ++i){
    v.push_back(dist(engine));
    w.push_back(dist(engine));
  }

  const auto nProcs = std::thread::hardware_concurrency();
  const auto len = NUM / std::thread::hardware_concurrency();

  std::vector<std::future<long long>> futures;
  futures.reserve(nProcs);
  for (int i = 0; i < nProcs; i++)  {
    auto startOffset = i * len;
    auto endOffset = (i + 1) * len;
    futures.push_back(std::async(std::launch::async, [&v, &w, startOffset, endOffset]{ 
      return getDotProduct(v, w, startOffset, endOffset);
    }));
  }

  // measure the execution time
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  long long sum = 0LL;
  for (auto& fut : futures) {
    sum += fut.get();
  }
  std::cout << "getDotProduct(v,w): " << sum << std::endl;
  std::chrono::duration<double> dur  = std::chrono::system_clock::now() - start;
  std::cout << "Parallel Execution: "<< dur.count() << std::endl;

  std::cout << std::endl;
}

