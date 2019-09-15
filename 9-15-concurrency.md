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
header | <thread> | <future>
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
