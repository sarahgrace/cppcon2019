# goal for async interfaces
* composable
* low abstraction overhead
* works with coroutines, fibers, threads
* extensible to multiple execution environments (threadpools, CPUs, GPUs)

# background

## parallelism vs. concurrency
* concurrency: multiple logical threads with unknown inter-task dependencies
  * imposes extra requirements on the scheduler
* parallelism: multiple logical threads with no inter-task dependencies
  * more universal
  * using concurrent features to express parallelism imposes unreasonable overheads
  * why is it fast? allows user to tell the scheduler critical info about lack of cross-task dependencies

scheduling guarantees (lease to most)
1. parallelism: less scheduling guarantees than serial
2. serial execution
3. concurrency: more scheduling guarantees than serial

## senders/receivers

```
future<int> async() {
  promise<int> p;
  auto f = p.get_future();
  thread t = {[p = move(p)]() mutable {
    p.set_value(5);
  };
  t.detach();
  return f;
}

// get future
auto f = async_algo();

// add random number to future
auto f2 = f.then([](int i) {
    return i + rand();
});

// print result
print(f2.get());
```

* why are futures/promises slow?
  * need shared state in order to communicate
  * 2 threads need to access the shared state at once
  * execute continuation field on the shared state -> need synchronization
* what's the shared state?
  * value
  * continuation
  * mutex/condition variable
  * ref count (not exactly sure why though)

```
// better implementation
template <class Cont>
future<int> async(Cont c) {
  return [](auto p) {
    thread t { [p = move(p), c]() mutable {
      p.set_value(c(5));
    }};
  t.detach();
  };
}

auto then(auto task, auto fun) {
  return [=](auto p) {
    struct promise {
      decltype(p) p_;
      decltype(fun) fun_;
      void set_value(auto ...vs) { p_.set_value(fun_(vs...))); }
      void set_exception(auto e) { p.set_ezception(e); }
    };
  task(promise{p, fun});
  };
}

// get a lambda
auto f = async();

// call then() with 2 lambdas
auto f2 = then(f, [](int i) {
    return i + rand();
});

// print result
print(f2.get());
```

* passing continuation avoids some synchornizaton overhaed because it removes the race on reading/writing the conitnuation
* achieve the same result by starting async work suspended and letting the caller add the continuation before launching the work

## co-routines=receivers, co-awaitables=senders
* everything after a co_await or a co_yield is implicitly a callback

```
task<int> async_helper() { ... }

task<void> async_algo() {
  int result = co_await async_helper();
  print(result);
}
```

* some senders are awaitable
* all awaitable types satisfy the requirements of the Sender concept

```
inline namespace awaitable_senders {
  template<Sender s>
  auto operator co_await(S&& s) {
    return _awaiter_sndr{(S&&) s};
  };
}
```
