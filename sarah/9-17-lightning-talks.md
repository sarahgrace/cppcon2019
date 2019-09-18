# macros that do nothing
* define input and output parameters (do nothing, just annotation)

```
#define IN // input parameter
#define OUT // output parameter
```

# std::any_of
* make simple things simple
* even if c++ might seem complicated, it still includes tools to get day to day work done efficiently (STL algorithms)
* challenge: any function that follows the range for loop pattern -> translate it to STL
```
vector<int> values{1, 2, 3, 4, 5};
auto pred = [](int value) { value == 5; };
bool result = std::any_of(values.begin(), values.end(), pred);
```

# make_shared vs shared_ptr
* strong_count: # shared_ptrs
* weak_count: # weak_ptrs
* implemented using memory_order_relaxed
* make_shared > shared_ptr
  * reduced # of separate allocations
  * cache locality: actions that work with the structure and object itself
  * order of execution + exception safety
* shared_ptr > make_shared
  * make_shared needs access to the object's constructor
  * lifetime of the object storage
  * can specify a custom deleter
