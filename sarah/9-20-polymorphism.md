# static polymorphism (overloading)

## value semantics
* nice to reason about thread-safeness and concurrency

## parent's polymorphic principle (PPP)
* requirements of a polymorphic type comes from its use
* there are no polymorphic types, only a polymorphic use of similar types
* "inheritance is the base class of evil" - Sean Parent talk

## benefits
1. low boilerplate
2. easy adaptation of existing classes
3. value semantics
4. low coupling
5. PPP
6. performance (static dispatch)

# dynamic polymorphism (virtual)
* Shape interface
* Drawing interface
* Circle class implements Shape, Drawing

## drawbacks
1. low boilerplate -> need inheritance hierarchy
2. easy adaptation of existing class -> have to inherit from base
3. value semantics -> pointer semantics (need to worry about concurrency now)
4. low coupling (only if we're lucky)
5. PPP -> types have to inherit from base
6. performance -> inherent overhead in runtime dispatch

# solution

## our constraints
* NO MACROS!
* c++17 only

## functions
* use `std::function<void()>`
* use `function_ref`
* problem: these only support one operation

## naming operations
```
// as a function
void rotate(double degrees)

// as a type
struct rotate{};
using rotate_algo = rotate.(...);
```

# how to call multiple operations?

## `polymorphic::ref`
```
struct translate{};
void spin(polymorphic::ref<void(rotate, double x),
                           void(translate, double x, double x)) {
  call<rotate>(...);
  call<translate>(...);
}
```

## `const polymorphic::ref`
* as long as the function types are const the ... (too fast)
* gist is that this is already taken care of
