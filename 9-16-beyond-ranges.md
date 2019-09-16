# formatting

## std::format \<format\>
* can't mix positional and indexed parameters
* runtime exceptions
  * wrong parameter count
  * mismatched parameter types

```
// positional parameters
string s = fmt::format("{} {}\n", "hello", "world");
std::cout << s; // hello world

// indexed parameters
s = fmt::format("{1} {0}\n", "hello", "world");
std::cout << s; // world hello

// double curly braces: use {{}} to output {}
s = fmt::format("{{ in brace {} }}", 42);
std::cout << s; // { in brace 42 }

// floats
s = fmt::format(
  "{0{1}.{2}f}", 
  12.345678, 
  10 /* width of the output */, 
  3 /* number past the decimal */
);
std::cout << s; // "    12.345"

// specify parameter types
s = fmt::format("{1:f} and {0:d}", 3, 4.56);
std::cout << s; // 4.56 and 3
```
## syncbuf and osyncstream
*wasn't paying attention*

## stringstream and basic_stringbuf
* `starts_with` and `ends_with`

```
std::stringstream s;
s << "stuff";
std::string_view sv = s.view() // reference to the internal buffer
```

# container and algorithms

## span \<span\>
* non-owning view over contiguous sequence of objects
* cheap to copy (implementation is pointer and size)
* O(1) time for all member functions
* constexpr ready

``` 
void print_reverse(span<int> s) {
  for (auto i : std::ranges::reverse_view{s}) {
    std::cout << i << ",";
  }
}

vector<int> v = {1, 2, 3};
print_reverse(v); // 1, 2, 3,

int a[] = {1, 2, 3}
print_reverse(a); // 1, 2, 3,

span<int> s(v);
print_reverse(s.first(2)); // 1, 2,
print_reverse(s.last(2)); // 2, 3,
```

## contains
* for maps only? 

```
std::map<string, int> myMap;
if (myMap.contains("key")) { // instead of myMap.find("key") != myMap.end()
  ...
}
```

## uniform container erasure
* erase: basic_string, deque, list, vector
* erase_if: associative containers (map, multimap, set, etc.)

# chrono
* structure/field types (`year_month_day`): used for io, conversions
* calculation types (`sys_days`, `time_point`, `seconds`): used for computation
* supports constexpr
* no error handling!
  * ok() will tell you if a value is good

```
year_month_day ymd(year(2019)/05/07);
sys_days d(ymd);
d += weeks(1);
std::cout << ymd; // 2019-05-07
std::cout << d; // 2019-05-14
```

# conurrency: threading and atomics

## jthread
* replacement for std::thread
* automatically joins in the destructor
* cooperative shutdown with `stop_token`
  * ex: `while (!token.requested_stop()) { // do stuff }`

## atomic
* atomic `shared_ptr` and `weak_ptr`
  * removes `atomic_shared_ptr<T>` and `atomic_weak_ptr<T>`
* `atomic_ref<T>`
