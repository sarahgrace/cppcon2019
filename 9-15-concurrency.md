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
* unlike promise/future, can be reset and reused (with reset()

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
## std::promise, std::futures, std::shared_future
```
// promise interface
p.swap(p2) // swap 2 promises
p.get_future() // return std::future
p.set_value(val) // make value available to future
p.set_exception(ex) // make exception available to future
p.set_value_at_thread_exit(val) // store value and set to valid when thread is done
p.set_exception_at_thread_exit(ex) // store exception and set to valid when thread is done

// future interface
f.share() // return std::shared_future (after calling f.share(), f.valid() returns false)
f.get() // returns value, if exception, throws
f.valid() // check if shared state is available
f.wait() // wait until shared state is available
f.wait_for(rel_time) // wait at most for some time
f.wait_until(abs_time) // wait until a certain time
```
* nearly all async entities are not copyable (only movable)
* exception is std::shared_future (created by f.share())
  * how to create:
    * `f.share()`
    * `std::shared_future<int> result = p.get_future()`
  * can independently ask std::promise for the value
  * same interface as std::future

```
// while waiting for a future, do something else
std::future_status status{};
  do {
    status = fut.wait_for(0.2s);
    std::cout << "... doing something else" << std::endl;
  } while (status != std::future_status::ready);
```
## promise/future vs. condition variables

characteristic | condition variable | task
--- | --- | ---
multiple synchronizations | yes | no
critical region | yes | no
exception handling in receiver | no | yes
spurious wakeup | yes | no
lost wakeup | yes | no

# the memory model

## the contract
* developer respects the rules
  * atomic operations
  * partial ordering of operations
  * visible effects of operations
* system wants to optimize
  * compile
  * proessor
  * memory system

1. single threaded: one control flow
2. multi-threaded: tasks, threads, condition variables
3. atomic: sequential consistency, acquire-release semantic, relaxed semantic

## atomics
* foundation of the c++ memory model
* synchronization & ordering guarantees established for atomics & non-atomics

### std::atomic_flag
  * interface: clear(), test_and_set()
  * the only lock-free data structure (all other types may internally use a lock)
  * building block for higher abstractions, such as a spinlock

```
class Spinlock {
  std::atomic_flag flag;
public:
  Spinlock() : flag(ATOMIC_FLAG_INIT) {}
  void lock() {
    // busy wait, CPU goes to 100%
    while (flag.test_and_set());
  }
  void unlock() {
    flag.clear();
  }
};
```
### std::atomic<bool>
* explicitly set to true or false
* supports compare_exchange_strong (compare and swap)
  * fundamental function for atomic operations
  * compares and sets a value in an atomic operation
  * `bool compare_exchange_strong(expected, updated)
* can be used for implementing a condition variable
 
```
atom.compare_exchange_strong(expected, updated)
*atom == expected -> *atom = updated -> return true
*atom != expected -> exp = *atom (change expected?) -> return false;
```











