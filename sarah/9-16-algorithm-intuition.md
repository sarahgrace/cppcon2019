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

## 1. iota
```
vector<int> v(10);

// forward iterators
iota(v.begin(), v.end(), 1); // 1 2 3 4 5 6 7 8 9 10

// reverse iterators
iota(v.rbegin(), v.rend(), 1); // 10 9 8 7 6 5 4 3 2 1
```

## 2. accumulate/reduce
```
vector<int> v{1, 2, 3, 4, 5};
auto x = accumulate (cbegin(v), cend(v), 0); // default is plus{}
auto x = accumulate (cbegin(v), cend(v), 0, plus{}); // same as above
auto x = accumulate (cbegin(v), cend(v), 0, multiplies{});
```

## 3. reduce/fold
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

## 4. adjacent difference
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

## 5. inner_product -> transform_reduce
* a binary function + a fold function

```
// find the max of a binary function
int solve(int h, vector<int> w, vector<int> l) {
  return max(0, inner_product(begin(w), end(w), begin(l), 0,
    [](auto a, auto b) { return max(a, b); }, // fold function
    [](auto a, auto b) { return a - (b / 4); }) - h); // binary function
}
```

* `transform_reduce(cbegin(v), cend(v), cbegin(u), 0, plus{}, multiplies{});`
  * start with 0
  * fold defaults to plus
  * transform defaults to multiply

```
// use transform_reduce on adjacent_difference problem
int min_value(vector<int>& c) {
  return transform_reduce(cbegin(c), --cend(c), ++cbegin(c), std::numeric_limits<int>::max,
    [](auto a, auto b) { return min(a, b); }, // reduce
    [](auto a, auto b) { return abs(a, b); }); // transform
}
```

## 6. partial_sum
* range sum, can be used with binary_search
* defaults to +, but can be overridden

```
vector v{1, 2, 3, 4, 5};
vector u(5, 0);
partial_sum(cbegin(v), cend(v), begin(u));
// 1 3 6 10 15
```
* ex: given n non-negative integers representing an elevation map, compute how much water will be trapped in the topology after it rains

```
template <class T>
using rev = reverse_iterator<T>;
int trap(vector<int>& v) {
  vector u(v.size(), 0);
  auto it = max_element(cbegin(v), cend(v));
  inclusive_scan(begin(v), next(it), begin(u), ufo::max()); // scan begin->max
  inclusive_scan(rbegin(v), rev(it), rbegin(u), ufo::max()); // scan max->end
  return transform_reduce(cbegin(u), cend(u), cbegin(v), 0,
    std::plus<>(),
    std::minus<>());
}
```

# algorithm intuition table

algo | indicies viewed | accumulator | reduce/transform | default
--- | --- | --- | --- | ---
accumulate/reduce | 1 | yes (init=specified) | reduce | plus
transform_reduce | 1 * | yes (init=specified) | reduce | plus, mult
inclusive_scan | 1 | yes (init=first element) | transform | plus
exclusive_scan | 1 | yes (init=specified) | transform | plus
adjacent_difference | 2 | no | transform | minus
transform_inclusive_scan | 2 | yes | transform | -
adjacent_reduce | 2 | yes | reduce | -
adjacent_find | 2 | no | reduce * (short circuits) | equal_to
transform | 1 * | no | transform | -
find | 1 | no | reduce * (short circuits) | -
iota | n/a | n/a | transform | -

```
auto generate_n(int n) {
  vector fib(n, 1);
  return adjacent_difference(cbegin(fib), cend(fib) - 1, begin(fib) + 1, std::plus{});
}
```
