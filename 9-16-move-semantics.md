# basics

```
// deep copy
std::vector<int> v1{1, 2, 3, 4, 5};
std::vector<int> v2;
v2 = v1;

// transfer of content
std::vector<int> createVector() {
  return std::vector<int>{1, 2, 3, 4, 5};
}
std::vector<int> v2;
// createVector() returns tmp vector
// tmp pointers are transferred to v2 (this is cheap)
// tmp goes out of scope and is destroyed
v2 = createVector();

// transfer of content using std::move
std::vector<int> v1{1, 2, 3, 4, 5};
std::vector<int> v2;
v2 = std::move(v1);
```

# lvalues and rvalues
```
std::string s{};
s + s = s;
// s + s is an rvalue
// s (right side) is an lvalue
```
* how to remember this?
  * lvalues have names
  * rvalues do not have names
```
// rhs is an lvalue
vector& operator=(const vector& rhs);
// rhs is an rvalue
vector& operator=(vector&& rhs);

v2 = v1; // v1 is an lvalue -> copy
v2 = createVector(); // createVector() returns an rvalue -> move
// v1 is an xvalue (expiring value)
v2 = std::move(v1) // std::move() returns an rvalue -> move
```
* operations you can do on moved-on objects
  * destructor
  * copy assignment
  * move assignment

# std::move
* unconditionally casts its input into an rvalue reference
* doesn't actually move anything

```
template <typename T>
std::remove_reference_t<T>&& move(T&& t) noexcept {
  return static_cast<std::remove_reference_t<T>&&>(t);
}
```

# specal member functions
```
int i, std::string s, std::unique_ptr<int> pi

// move constructor
Widget(Widget&& w) = default;

// move assignment operator
Widget& operator=(Widget&& w) = default;
```

* **RULE OF ZERO: if you can avoid defining default operations, do so**
* default behavior: works if all member variables are move-able
* w was already cast to an rvalue or else we wouldn't have entered this function
* exceptions
  * make your operations noexcept
  * this allows functions to invoke std:move() instead of copying the object

goal
1. clean up all visible resources
2. transfer content of w into this
3. leave w in a valid but undefined state

```
int i, std::string s, int * pi

// Move constructor
Widget(Widget&& w) noexcept 
  : i(std::move(w.i)), // works but does nothing
    s(std::move(w.s)),
    pi(std::exchange(w.pi, nullptr)) {} // reset w's pi or else it would break

// Move assignment operator
Widget& operator=(Widget&& w) noexcept {
  delete pi; // cleanup
  i = std::move(w.i);
  s = std::moe(w.s);
  // pi = std::swap(pi, w.pi); , this is less deterministic
  w.pi = nullptr; // reset
  return * this;
}

// copy + move assignment operator
Widget& operator=(Widget w) {
  swap(w); // your own implementation
  return * this;
}

void swap(Widget& w) {
  ...
}
```

* default move operations generated if no copy operation or destructor is user-defined
* default copy operations generated if no move operation is user-defined
* `=default` and `=delete` count as user defined!
* **if you =define or =delete any default operation, =define or =delete them all**

# forwarding references
* reference collapsing
  * & &, && &, && & -> &
  * && && -> &&

```
template <typename T>
void foo(T&&) {
  print("foo(T&&)");
}

// ex 1
Widget w;
foo(w); // prints

// ex 2
foo(Widget()) // prints
```

# std::forward
* conditionally casts its input into an rvalue reference
  * if given value is lvalue -> cast to lvalue reference
  * if given value is rvalue -> cast to rvalue reference

```
template <typename T>
T&& forward(remove_reference_t<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T, typename... Args>
unique_ptr<T> make_unique(Arg&& arg) {
  return unique_ptr<T>(new T(std::forward<Args>(args)));
}
```

# perils of forwarding references
```
struct Person {
  Person(const std::string& name);
  template <typename T> Person(T&&);
};

// calls ctor(2)
Person p1("Bjarne"); // char[7]

// calls ctor(2)
std::string name("Herb");
Person p2(name) // argument is not const (std::string&)

// calls ctor(2)
Person p3(p1); // argument is not const (Person&)
```
## overloading on forwarding references
* if template is instantiated and becomes exactly like a non-templated function, the non-templated function is chosen

1. lvalue reference
* `void f(Widget&);`
2. lvalue reference-to-const
* `void f(const Widget&);`
3. rvalue reference
* `void f(Widget&&);`
4. rvalue reference-to-const
* `void f(const Widget&&);`
5. template with forwarding reference
* `template <typename T> void f(T&&);`
6. template with rvalue reference-to-const
* `template <typename T> void f(const T&&);`

```
// (1) Widget&
// (5) T&&
// (2) const Widget&
Widget w;
f(w);

// (2) const Widget&
// (5) T&&
// (6) DOESN'T WORK because it's an rvalue reference
const Widget w;
f(w);

// (3) Widget&&
// (5) T&&
// (4) const Widget&&
// (6)
// (2) const Widget&
Widget getWidget()
f(getWidget());

// (4) const Widget&&
// (6) const T&&
// (5) T&&
// (2) const Widget&
const Widget getWidget()
f(getWidget());
```

## move semantics pitfalls
* if function is templated with T and function argument is T&&
  * argument is a forwarding reference (can be lvalue or rvalue)
  * don't unconditionally move (std::move), use std::forward<T>
* if class is templated with T and function argument is T&&
  * argument is rvalue reference, use std::move
* no double std::move or std::forward
* copy elision (return value optimization): if you return something from a function, compiler will optimize
  * **don't return T&&**
  * std::move() for a return statement will prevent RVO







