# warm up

* given an array of integers, find the difference between the max and min
  * access all elements but don't need index -> use range for

```
// sort the vector
sort(begin(v), end(v));
return v.end() - v.begin();

// get max and min
return max_element(begin(v), end(v)) - min_element(begin(v), end(v));

// get min and max at the same time
auto [a, b] = minmax_element(begin(v), end(v));
return *b - *a;

// const version (doesn't mutate)
auto [a, b] = minmax_element(cbegin(v), cend(v));
return *b - *a;
```
# algorithms

* headers
  * \<algorithm\>
  * \<numeric\>
  * \<memory\>

## iota
```
vector<int> v(10);

// forward iterators
iota(v.begin(), v.end(), 1); // 1 2 3 4 5 6 7 8 9 10

// reverse iterators
iota(v.rbegin(), v.rend(), 1); // 10 9 8 7 6 5 4 3 2 1
```

## accumulate/reduce
```
vector<int> v{1, 2, 3, 4, 5};
auto x = accumulate (cbegin(v), cend(v), 0); // default is plus{}
auto x = accumulate (cbegin(v), cend(v), 0, plus{}); // same as above
auto x = accumulate (cbegin(v), cend(v), 0, multiplies{});
```

## reduce/fold:
* taking a structure and folding it down to a single value
* many STL are folds
* can be done in parallel- std::execution::seq, std::execution::par

```
// catamorphic algorithms
template <class Iterator, class T>
int count(Iterator r, Iterator l, const T& val) {
  return std::reduce(r, l, 0,
    [val](auto a, auto b) {
      return a + (b == val);
    });
}

template <class Iterator, class Predicate>
bool any_of(Iterator r, Iterator l, Predicate p)  {
  return std::reduce(r, l, false,
    [p](auto a, auto b) {
      return a || p(b);
    });
}

template <class Iterator, class Predicate>
bool all_of(Iterator r, Iterator l, Predicate p)  {
  return std::reduce(r, l, false,
    [p](auto a, auto b) {
      return a && p(b);
    });
}
```

## adjacent difference
* N coins, want to find 2 coins with the minimum difference
```
int min_value(vector<int>& c) {
  sort(begin(c), end(c))
  vector<int> d(v.size())
  adjacent_difference(cbegin(), cend(c), begin(d));
  // for adjacent difference, the first element doesn't count (b/t 0 and the first element)
  return * min_element(cbegin(d) + 1, cend(d));
}
```

## inner_product
* a binary function + a fold function
* find the max of a binary function

```
int solve (int h, vector<int> w, vector<int> l) {
  return max(0, inner_product(begin(w), end(w), begin(l), 0,
    [](auto a, auto b) { return max(a, b); }, // fold function
    [](auto a, auto b) { return a - (b / 4); }) - h); // binary function
}
```


