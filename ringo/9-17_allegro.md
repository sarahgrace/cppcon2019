# 'Allegro' means both fast and happy. Coincidence? - Andrei Alexandrescu
If you integrate conditionals in arithmetic, then there's no jump, just an arithmetic which is faster:
```
// good
auto right = first + 1+ (size & 1); 
// bad, involves branch prediction
if (size & 1) {
  right++;
}
```
Sometimes doing more work is better
In the end, the idea that won the most is:
heapifying before insertion sort is faster

- Anti-pattern: Don't mix hot and cold code together (code that is hit often and code that is not hit often)
- break early out of functions and loop so you don't do extra work
