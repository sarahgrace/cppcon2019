# threads

* differentiate thread vs. execution, thread is just the "handle" which runs the function
* thread creator
  * t.join() waits for child
    * this is a blocking call
  * t.detach() detaches from child (thread becomes daemon thread)
    * does not block
    * only do this if you don't have a timing issue
* thread lifetime
  * a thread starts executing as soon as it's created!!
  * a thread is joinable if join() or detach() not performed
  * joinable thread calls std::terminate in its destructor
* thread args
  * threads should get its arguments by copy (so validity of data is ensured)

```
// thread interface
t.joinable() // check if supports join() or detach()
t.get_id() or std::this_thread::get_id() // return id of thread
std::thread::hardware_concurrency() // hint for # threads that can run in parallel
std::this_thread::sleep_until(abs_time)
std::this_thread::sleep_for(rel_time)
std::this_thread::yield() // offers the system to execute another thread
t.swap(t2) or std::swap(t1, t2) // swap threads
```

# locking mechanisms

* shared data
  * thread sanitizer: -fsanitize=thread
  * data race: at least 2 threads access a variable at the same time, at least one is a write
  * critical section: region which at most 1 thread can have access to the data
    * to establish this: mutex (mutual exclusion) 
    * mutex.lock() to go inside the region, mutex.unlock() to leave
  * multiple threads writing to std::cout- NOT a data race! characters are atomic, in order

## mutexes

```
// mutex interface
m.lock() // enter
m.unlock() // leave
m.try_lock() // try it once, otherwise leave
m.try_lock_for(rel_t) // wait at most for some time
m.try_lock_until(abs_t)
```

  * std::shared_timed_mutex (c++14): there can be more than 1 person in the room at the same time if neither are doing something critical
  * optimize contention points: allow as many readers as you want, but kick the readers out when someone writes
* deadlocks
  * if a thread holding the mutex dies, it will never unlock the mutex -> deadlock
  * if the thread holding the mutex forgets to unlock the mutex (flow issue)
  * if a thread holding the mutex tries to get the same mutex again (locking a mutex from the same thread more than once is undefined behavior!)
    * recursive mutex allows the same thread to acquire multiple levels of ownership over the mutex

```
// don't do this
mutex m;
m.lock();
sharedVar = getVar(); // never call unknown code while holding a lock (don't know how long it will take, what it will do)
m.unlock();

// do this!
mutex m;
auto res = getVar(); // create variable on the stack, do getVar() outside
m.lock();
sharedVar = res; // make critical region as short as possible!!
m.unlock();
```

## locks

* lock_guard: non-copyable, automatically unlock when it goes out of scope
  * RAII (resource acquisition is initialization): resource guard, releases the resource in its destructor
  * declare resource guards locally so the resource will guaranteed to be released once it goes out of scope
  * calling lock()/unlock() is an antipattern
  * curly braces are important- it denotes scope

```
// use lockguard instead of mutexes so you don't have to explicitly call unlock!
std::mutex coutMutex;
std::lock_guard<std::mutex> lockCount(coutMutex); // instead of coutMutex.lock() and unlock()
std::cout << "hi" << std::endl;
```

* unique_lock
  * creating with std::defer_lock does not acquire the mutex on construction
* std::lock(...) 
  * lock arbitrary number of mutexes atomically
  * uses a deadlock-avoidance algorithm
  * takes at least 2 Lockable (lock() and unlock())

```
// unique_lock interface
<mutex interface>
lk.release() // release lock without unlocking
lk.swap(lk2) or std::swap(lk, lk2) // swap locks
lk.mutex() // release pointer to the mutex
lk.owns_lock() // test if lock has a mutex

// mutexes a and b
// threads t1 and t2 try to acquire a and b together

std::unique_lock<std::mutex> guard1(a.mut, std::defer_lock);
std::unique_lock<std::mutex> guard2(b.mut, std::defer_lock);
std::lock(guard1, guard2);
// critical region

OR

{ 
  std::scoped_lock scoLock(a.mut, b.mut); 
  // critical region
} // curly braces for scope
```

# thread safe data

* rules of concurrency
  1. be as local as possible (don't share)
  2. be as const as possible

## constexpr (since c++11)
* evaluated/initialized during compile time
* implicitly thread-safe
* flavors
  1. variables
    * implicit const
    * when you initialize a const with a literal, it becomes a constexpr (`const int a = 10 -> constexpr int a = 10`)
  2. functions
    * gives the function the POTENTIAL to run at compile time, but may also run at runtime (depending on where it's used)
    * constexpr return value
    * function body can only be a return statement (in c++14, can have loops, variables but make sure the function is pure)
    * implicitly inline
  3. user-defined type
    * constructor has to be empty and constexpr
    * can have methods which are constexpr
    * instances can be instantiated at compile time

```
// constexpr variable
constexpr double myDouble = 10.5;

// constexpr function
constexpr int fac(int n) {
  return n > 0 ? n * fac(n - 1) : 1;
}

// constexpr user-defined stype 
struct MyDouble {
  constexpr MyDouble(double v): val(v) {}
  constexpr double getValue() { return val; }
  private:
    double val;
};

constexpr MyDouble myDouble(10.5);
```

* thread safe initialization
  * if your variable is read only (const), the only thing that needs to be thread safe is during initialization
  1. constant expressions
  2. std::call_once + std::once_flag (kind of like atomci bool): ensures only one thread can run the function
    * call_once: registers a callable unit
    * once_flag: guarantees one of the registered functions will be called once
  3. static variables with block scope (2 curly braces): example-Singleton pattern
  4. initialize before you start threading

```
// call_once + once_flag
std::once_flag onceFlag;

void doOnce() {
  std::call_once(onceFlag, [](){
    std::cout << "once" << std::endl;
  });
}

void doOnce2() {
  std::call_once(onceFlag, [](){
    std::cout << "once2" << std::endl;
  });
}

// only one line will be printed because doOnce() and doOnce2() both use the same once_flag
std::thread t1(doOnce);
std::thread t2(doOnce2);
std::thread t3(doOnce);
std::thread t4(doOnce2);

// static variables
void blockScope() {
  static int mySharedData = 2019;
}
```

* double lock checking (order of operations)
  * 1. memory is allocated
  * 2. assignment happens
  * 3. constructor runs
  * this is broken, suppose getInstance() returns a pointer to an object which isn't fully constructed yet

```
// this doesn't work
static MySingleton* getInstance() {
  if (!instance) {
    std::lock_guard<std::mutex> myLock(myMutex);
    if (!instance) {
      instance = new MySingleton();
    }
  }
  return instance;
}

// this works! static variable with block scope
static MySingleton* getInstance() {
  static MySingleton mySingleton;
  return &mySingleton;
}

// this also works
std::once_flag onceFlag;
static MySingleton* getInstance() {
  std::call_once(onceFlag, []() {
    instance = new MySingleton();
  });
  return instance;
}
```



