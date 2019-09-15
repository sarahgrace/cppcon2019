#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

void product(std::promise<int>&& intPromise, int a, int b){
  intPromise.set_value(a*b);
}

struct Div{

  void operator() (std::promise<int>&& intPromise, int a, int b) const {
    if (b == 0) {
      intPromise.set_exception(
        std::make_exception_ptr(std::invalid_argument("Divide by 0 not allowed.")));
      return;
    }
    intPromise.set_value(a/b);
  }

};

int main(){

  int a= 20;
  int b= 10;

  std::cout << std::endl;

  // define the promises
  std::promise<int> prodPromise;
  std::promise<int> divPromise;

  // get the futures
  std::future<int> prodResult= prodPromise.get_future();
  std::future<int> divResult= divPromise.get_future();

  // calculate the result in a separate thread
  std::thread prodThread(product,std::move(prodPromise),a,b);
  Div div;
  std::thread divThread(div,std::move(divPromise),a,b);

  // get the result
  std::cout << "20*10= " << prodResult.get() << std::endl;
  std::cout << "20/10= " << divResult.get() << std::endl;

  prodThread.join();
  divThread.join();

  std::promise<int> divZeroPromise;
  std::future<int> divZeroFuture = divZeroPromise.get_future();
  std::thread divZeroThread(div, std::move(divZeroPromise), 5, 0);

  try {
    auto result = divZeroFuture.get();
  } catch (std::invalid_argument& e) {
    std::cout << "Error while dividing: " << e.what();
  }

  divZeroThread.join();

  std::cout << std::endl;
}


