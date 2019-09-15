# tasks

* promise
  * sends the data
  * may serve many futures
  * can send values, exceptions, and notifications
* future
  * receives the data
  * calls get() or wait() and blocks on this
* it's a one-way, one-time communication channel! promise can send only once, future can receive only once

characteristic | thread | task
--- | --- | ---
header | \<thread\> | \<future\>
participants | creator/child thread | promise/future
commmunication | shared variable | communication channel
thread creation | obligatory | optional
synchronization | join() blocks | get()/wait() blocks
exception in child | child/creator terminate | exception sent to future

```
// thread
int res;
std::thread t([&]{ res = 3 + 4; });
t.join();
std::cout << res << endl;

// task
auto fut = std::async([]{ return 3 + 4; });
cout << fut.get() << endll;
```

## std::async
* behaves like asynchronous function call
* gets a callable as an argument
* returns std::future object for getting result of the function
* launch policies:
  * std::launch::async - eager (in another thread)
  * std::launch::deferred - lazy (in the same thread)
* c++ runtime will decide if std::async should be executed in another thread
* fire and forget: if you call std::async without storing/getting the future, the function will run SYNCHRONOUSLY in the same thread

```
// synchronous
// first thread
// second thread
// main thread
std::async(std::launch::async,[]{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "first thread" << std::endl;
});
    
std::async(std::launch::async,[]{
  std::this_thread::sleep_for(std::chrono::seconds(1));  
  std::cout << "second thread" << std::endl;
});
  
  std::cout << "main thread" << std::endl;  

// asynchronous
// main thread
// second thread
// first thread
auto fut1 = std::async(std::launch::async,[]{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "first thread" << std::endl;
});
    
auto fut2 = std::async(std::launch::async,[]{
  std::this_thread::sleep_for(std::chrono::seconds(1));  
  std::cout << "second thread" << std::endl;
});
  
std::cout << "main thread" << std::endl;  

fut1.get();
fut2.get();
```
## std::packaged_task
* simple wrapper for a callable function that can be executed later

```
// 1. declare function
int add(int a, int b){ return a + b; } 

// 2. wrap the function in a std::packaged_task
std::packaged_task<int(int, int)> sumT(add); 

// 3. get the future from the function
std::future<int> sumRes = sumT.get_future();

// 4. execute function
sumT(2000, 11);

// 5. print result
std::cout << sumResult.get() << std::endl;
```























