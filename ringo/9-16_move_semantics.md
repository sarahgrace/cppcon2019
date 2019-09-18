# Move Semantics
```
std::vector<int> createVector() {
  return std::vector<int>{1,2,3,4,5};
}
std::vector<int> v2;
v2 = createVector(); // return value optimization
```
Example of move semantics:
```
std::vector<int> v1{1,2,3,4,5};
std::vector<int> v2{};
v2 = std::move(v1); // moves contents of v1 to v2
```
## lvalues vs rvalues
lvalues are things that you can name or take a reference to
```
s+s = s;
```
the left hand expression is the rvalue while the right hand side is an lvalue
```
v2 = std::move(v1); // xvalue, moved from object
```
there are only three things to do to a moved from object: destruct, copy assign to it or move assign to it

The definition of move involves a static_cast. Using static_cast is saying that you know better than the compiler so compiler will not warn you about things you incorrectly static cast.

## New special member functions
```
class Widget {
 private:
  int i{0};
  std::string s{};
  unique_ptr<int> pi{};
 public:
  // Move ctor
  Widget(Widget&& w) = default;
  // Move assignment operator
  Widget& operator=(Widget&& w) = default;
};
```
**Rule of Zero: If you can avoid defining default operations, do.**
In the below case, we can't default the move ctor and assignment operator
### Move constructor
```
 class Widget {
 private:
  int i{0};
  std::string s{};
  int* pi{nullptr};
 public:
  // Move ctor
  // w is an rvalue in the calling scope (when this ctor is called) otherwise this ctor wouldn't get called
  // but inside the function w is an lvalue
  Widget(Widget&& w) noexcept // w is an lvalue
    : 
    i(std::move(w.i)), // correct, but no speed up
    s(std::move(w.s)), // need to move since w is an lvalue so w.s would also be an lvalue
    pi(std::move(w.pi)) 
    {
      w.pi = nullptr;
      w.i = 0;
    }
    // alternative to moving pi and setting to nullptr
    // can use std::exchange(w.pi, nullptr)
  // Move assignment operator
  Widget& operator=(Widget&& w) = default;
};
```
Core guideline: Make move operations noexcept. Why make move constructors noexcept? It increases performance because it enables move to happen in more scenarios. **See push_back example in slides**

Core guideline: a move operation should move and leave its source in a valid state.

The default move constructor is noexcept. Also, if all you have to do is memberwise move, then you can let the compiler generate the default.

### Move assignment operator
```
class Widget {
 private:
  int i{0};
  std::string s{};
  int* pi{nullptr};
 public:
  // Move assignment operator
  Widget& operator=(Widget&& w) {
    delete pi;
    i = std::move(w.i);
    s = std::move(w.s);
    pi = std::exchange(w.pi, nullptr);
    // can also do std::swap(pi, w.pi); but less deterministic
    return *this;
  }
};
```
Overall Impressions: content was too basic so it wasn't as interesting.
