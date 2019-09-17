# evolution of code
1. pile of code
2. structured code
3. OOP
4. design patterns
5. ...

# design abstractions
* functions
* classes
* templates
* concepts

# fowler refactorings
* change function definition
* extract function
* inline function
* move function
* replace error code with exception
* replace magic literal

# single step refactoring
* exeucte change to a codebase in a single step
  * fowler refactorings
  * change behavior
  * add default paramenter
  * global find/raplece for an APIj
* is this what you're trying to achieve, or how you're trying to achieve it?
* when this fails
  * need a more complex strategy when making a scales-with-usage change
  * across organization/repo boundaries
  * involve more files can you can commit at once
  * making a change to a library wtih compatibility-over-time-guarantees
    * how long should it be backwards compatible?

no single step refactoring for:
1. scale
2. visibility

# multi-step refactoring
execute a large/distributed change in a series of steps such that:
1. the ecosystem continues to build and function cleanly at every step
2. each step is small enough to be handled as part of a normal developer workflow
* basic atoms of refactoring
  * add an overload
  * add an alias
  * change a caller from one function to another
  * change an instantion from one type to another
  * remove an unused API
* larger atoms (molecules?) of refactoring
  * renaming a type, works if usage allows the change in a single step
  * change a function signature

# types of changes
renaming a type

```
// types
using NewType = OldType;

// templated types
template <typename T> using MyVector = vector<T>;

// functions
static int auto& NewName = oldns::OldName;
```
* find a way to allow the pre/post syntax/semantics in a way that they co-exist

## functions
* ex: change default precision for absl::StrCat
  * default convert double to 32 character string
  * this is expensive!

```
// starting point
std::string result = absl::StrCat(SomeDouble());

// 1. add LegacyPrecision
std::string result = absl::StrCat(LegacyPrecision(SomeDouble()));

// 2. change LegacyPrecision to return string_view
inline string_view LegacyPrecision(double d);
std::string result = absl::StrCat(LegacyPrecision(SomeDouble()));
```

* small atoms
  * weakoning preconditions
  * strengthen postconditions
* large atoms
  * strengthen postconditions
  * weaken postconditions

## function signatures
* issues
  * string formatting
  * type deduction (ex: max(a, b), changing a or b alone will break)
* widening return type (int -> long)
  * rename foo to old_foo
  * delete foo
  * add new foo with new return type
* narrowing return type (long -> int)

## vocabulary types
* changing usage of "compatible" types
  * ex: map -> unordered map
  * vocabulary types form dependecy networks (everything that object is passed to)
