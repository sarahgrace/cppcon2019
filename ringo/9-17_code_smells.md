# C++ Code Smells by Jason Turner
## C++ Best Practices
- C++ Core guidelines: 496 
- Effective Modern C++: 42 
- C++ Best Practices: 109
- C++ Coding standards: 101
748 best practices, it's too many. So instead, let's look for things to avoid,
## Code smells
1. Construction separate from assignment
```
#include <string>

void do_work() 
{
  std::string str;
  str = "Hello World";
  // prefer below instead
  // const std::string str = "Hello World";
}
```
2. Don't use output variables - look for Sean Parent's article on this topic
```
#include <string>
std::string get_value(std::string& out);
int main()
{
  std::string out;
  get_value(out);
  // prefer below instead
  const auto value = get_value();
}
```
3. Raw loops don't express intent, but algorithms can. Replace for loop with STL algorithms
4. Multi-step functions. Instead, decompose steps into functions/lambdas. 
5.  Functions should be const
```
struct Data {
  int x;
  int y;

  // this function should be const and other should be const
  bool operator==(Data& other) {
    return x == other.x && y == other.y;
  }
};
```
6. Code with implicit constructors
```
void use_string(const std::string& s);
const std::string s = "hello world";
use_string(s.c_str());
```
7. Don't disable return value optimization
```
std::string get_value()
{
  std::string s = "Hello There World";
  return std::move(s);
}
```
8. Undefined behavior: don't modify const or use const_cast
```
int main()
{
  const int i = 4;
  const_cast<int &>(i) = 13;
  return i;
}
```
9. Implications of using static variables - static const is a code smell that should probably be `constexpr`
```
void log_error(std::string const &location, std::string const &desc);
void do_things(bool const error)
{
  // add a constexpr to the beginning of this and change it to string_view
  static std::string const FunctionName{"do_things"};
  if (error) {
    log_error(FunctionName, "ErrorOccurred");
  }
}
``` 
Each time FunctionName is accessed, the guard must be checked (guard is generated for static variables by the C++ compiler)

10. extern const should be constexpr
```
extern int const Value; // should make this constexpr instead
int getValue() {
  return Value; 
}
```
## Conclusions
use const whenever possible
- any lack of const is a code smell
- const forces us into more organized code
- const prevents common error

Do you const temporary values? - You can but better to inline them
Do you const value return types? 

**Any lack of const is a code smell**

Member variables shouldn't be const (this prevents moving the enclosing struct)
