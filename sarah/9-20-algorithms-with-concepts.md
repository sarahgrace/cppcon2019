# algorithms
1. useful
2. generic
3. small enough to fit in 1 slide

## semiregular types
operations:
1. copy construction
2. assignment
3. destruction

## regular types
operations:
1. copy construction
2. assignment
3. eauqlity
4. destruction

# 1. min(x, y)

```
// does not modify object
// if equal, return first element
template <typename T, typename P> // T=object type, P=predicate
const T& min(const T& x, const T& y, P pred) {
  if (pred(y, x)) {
    return y;
  }
  return x;
}
```

## 2. unique_count, unique_copy, unique
```
// define some iterators
concept iterator = std::is_same<std::forward_iterator_tag, typename std::iterator_traits ...
concept forward_iterator = ...
concept bidirectional_iterator = ...

template <typename T, typename U = ValueType(T)>
concept writable = requires(T it, U x) { *it = x; };
```
