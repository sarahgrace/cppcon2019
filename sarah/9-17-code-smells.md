# c++ best practices
* c++ core guidelines: 496 (herb sutter)
* effective modern c++: 42 (scott meyers)
* c++ best practices: 109 (jason turner)
* c++ coding standards: 101 (herb sutter)

# smells

1. construction separate from assignment
```
std::string s;
s = "hello world";

// construct and assign together
const std::string = "hello world";
```

2. don't use output variables
```
// function changes input argument
void get_value(std::string& s); 
std::string s;
get_value(s);

// function returns value
std::string get_value();
const auto value = get_value();
```

3. raw loops
```
// two separate loops
int value = 0;
for (int i = 0; i < pipes.size(); ++i) {
  value += pipes[i].radius * pipes[i].radius * M_PI;
}
for (int i = 0; i < pipes.size(); ++i) {
  value += hose[i].radius * pipes[i].radius * M_PI;
}

// use STL algorithms where possible
const auto accumulate_area = [](const auto lhs, const auto rhs) {
  return lhs + area(rhs);
}
const auto total_area = [&](const auto& container) {
  return std::accumulate(begin(container), end(container), 0.0, accumulate_area);
}
return total_area(pipes) + total_area(hoses);
```

4. non-canonical operators
```
struct Data {
  int x;
  int y;
};

// == operator needs to be const
bool operator==(Data& rhs) {
  return x == rhs.x && y == rhs.h;
}
```

5. code with conversions: if adding many ints, probably need result to be int64

6. code with implicit constructors
```
void use_string(const std::string& s);
const std::string s = "hello world";
// implicitly converts const char * back to a string
use_string(s.c_str());


// prevents move elision
std::string get_value() {
  std::string s = "hello world";
  return std::move(s);
}
```

7. casting away const :)

8. weak typing (aka "which int is which" interface)
```
bool doStuff(int one, int two, int three, int four);
```

9. `static const` -> `constexpr static`
```
void foo(const bool error) {
  // can use constexpr
  static std::string const FunctionName("foo");
  if (error) {
    logError(FunctionName, "Error!");
  }
}
```

10. `extern const`
```
// value is known at compile time
// constexpr int value = 5;
extern int const value = 5;
int getValue() {
  return value;
}
```

11. raw `new` and `delete`




