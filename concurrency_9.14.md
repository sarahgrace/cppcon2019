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
std::thread::hardware_concurrency() // hint for \# threads that can run in parallel
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
  * curly braces are important- it denotes scope

```
// use lockguard instead of mutexes so you don't have to explicitly call unlock!
// calling lock()/unlock() is an antipattern
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


