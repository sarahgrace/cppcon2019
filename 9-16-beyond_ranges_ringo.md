# The C++ Standard Library: Beyond Ranges - Jeff Garland

## io - formatted output
### output format {fmt}
- new output string format `std::format`
- based off the python implementation, pretty much what works in python will work here
```
#include <fmt/core.h> // will be <format>
{
  // hello world
  string s = fmt::format("{} there {}\n", "hello", "world");
  cout << s; // hello there world
}
```
#### Can do index parameters, but can't mix index with non-index
```
{
  // indexed parameters -- parm[0] == 42, parm[1] == "hello"
    string s = fmt::Format("{1}and {0}\n", 42, "hello";
    cout << s; // hello and 42
}
```
- escaping in fmt::format is easier, can do double `{{` to get `{`
#### floating point format
```
int width = 10;
int precision = 3;
// format like {0:10.3f}
auto s = fmt::format("{0:{1}.{2}f}, 12.345678, width, precision);
// s = "     12.345";
```
- there is a rich language for fill, alignment and conversion. Can specify width, precision, left align and right align
#### custom type usage
check slides, requires a lot of code to extend fmt for custom types
- Need to open std namespace?? and modify fmt there
#### fmt diagnostics/safety
- all diagnostics are at runtime, not compile-time. For example, **incorrect parameter count** or **mismatched parameter types** (passing in an int as a string) will give runtime exceptions.
- Don't use inside a catch block/destructor because they may throw exceptions

### sync_buf and osyncstream
streams in multi-threaded application can produce garbled output. osyncstream
```
#include <syncstream>
{
  osyncstream buffered_out(cout);
  buffered_output << " the answer might be 42" << endl;
} // on destruction buffered_out now calls emit()
  // emit() flushes the buffer to cout
{
  std::ofstream out_file("my_file");
  osyncstream buffered_out(out_file);
  std::emit_on_flush(buffered_out);
  buffered_out << "hello world" << endl;
  ...
}
```
### efficient stringbuf and stringstream access
Can get access to a stringstream using stringstream::view()
```
#include <sstream>
std::stringstream s;
s << "put some data into the stream";
std::string_view sv = s.view();
```
## container and algorithm updates
### span
- span is a non-owning 'view' over contiguous sequence of an object
- cheap to copy (implementation is a pointer and a size)
- constant time complexity for all member functions
- defined in header <span>
- can mutate what they point to
- constexpr ready
```
vector<int> vi = {1,2, 3,4,5};
span<int> si(vi);
```
Use span as a function parameter
```
void print_reverse(span<int> si) { // by value
  for (auto i : std::ranges::reverse_view{si}) {
    cout << i << " ";
  }
  cout << "\n";
}

int main() {
  vector <int> vi = {1,2,3,4,5};
  print_reverse(vi); // 5, 4, 3, 2, 1
  // span<int> has a constructor that takes vector<int>
}
```
### contains 
Don't need to do the .find() == .end() anymore
```
std::map<string, SomeType> m = initMap();
if (m.find("foo") != m.end()) 
// in c++20
if (m.contains("foo"))
```
### uniform container erasure
```
// pre C++20
std::vector<SomeType> c = ...;
c.erase(remove_if(c.begin(), c.end(), pred), c.end());
// can now call erase(cvalue) or erase_if(predicate)
// this applies to anything that is a container
```

## chrono
- two primary types in chrono:
  - structure/field types like year_month_day
  - calculation types like sys_days, time_point, seconds
- fields types are used more for i/o and conversion
- calculation types - small memory, fast computations
- library is completely constexpr
```
year_month_day ymd(year(2019)/05/07);
sys_days d(ymd);
d += weeks(1);
cout << ymd; // 2019-05-07
cout << d; // 2019-05-14
```
Error handling, no exceptions. Instead, use the ok method. This is an intentional design decision.
```
year_month_day ymd(year(2019)/100/200);
if (!ymd.ok()) {
  cout << "bad ymd\n"; // executes
}
```
### Chrono format output, looks like python
Can do locale aware format output
## concurrency (threading and atomics)
### jthread and stop_token
- jthread is joining thread. 
- **Automatically joins in the destructor** (so you don't have to do thread.join() manually anymore). 
- stop_token provides cooperative shutdown
```
#include <thread>
void do_nothing() {
  this_thread::sleep_for(seconds(1));
}

int main() {
  std::thread t(do_nothing);
} // will get std::terminate since forgot to do t.join()
// using jthread in C++20
int main() {
  std::jthread jt(do_nothing);
} // will not call std::terminate since jthread joins in its destructor
```
### atomic<shared_ptr> and atomic<weak_ptr>
- current atomic_shared_ptr interfaces are error-prone
### atomic_ref\<T>
- high performance computing motivates
- like atomic\<T> but for reference
**will need to check code example**
## misc.
### starts_with and ends_with for string and string_view. 
Add the functionality: is a string or string_view prefixed or suffixed with a certain string?
### source_location
Modern replacement for FILE and LINE 
** I don't really understand this one**
### math constants
- \<math> header 
- has some useful math utility functions, midpoint and linear interpolation
## bit manipulation
### \<bit> header
-`ispow2()`, `ceil2()`, `floor2()`
### rotation and count
### bit_cast
- use instead of reinterpret_cast
### endian
- detect the bit order at compile time or runtime with the endian struct
## stuff not covered
- ranges
- concept support for ranges
- concurrency
- 3way compare support (aka spaceship support)
- char8_t, u8string, u8string_view
- constexpr on algorithms and containers
- noexcept addition
- variant changes
- algorithm vectorization policies



