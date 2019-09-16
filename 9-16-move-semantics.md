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
int i, std::string s, int* pi

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
  return *this;
}

// copy + move assignment operator
Widget& operator=(Widget w) {
  swap(w); // your own implementation
  return *this;
}

void swap(Widget& w) {
  ...
}
```

* default move operations generated if no copy operation or destructor is user-defined
* default copy operations generated if no move operation is user-defined
* `=default` and `=delete` count as user defined!






