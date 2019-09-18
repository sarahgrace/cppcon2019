
# From Functions to Concepts: Refactoring Impact of Higher-level design features by Titus Winters
## Historical Context
Code evolved from:
- pile of code (gotos, labels)
- structured programming (functions w/ well-defined signatures, loops)
- OOP
- Functions, classes, templates, concepts, ???

## Refactoring
### Fowler Refactorings
- Change Function declaration
- Extract function
- inline function
- move function
- replace error code with exception
- replace magic literal

These are all single step refactorings (or atomic refactoring)

Single step refactoring fails when the changes are of the type scales-with-usage. This means a change to something like std::vector will result in many callsites having to be updated, maybe more than version control can take as once.

### Multi-step refactoring
#### Rename a type
```
namespace oldns {
class StringPiece {
  // ...
};
}
```
Say we want to replace this type with abs::string_view

namespace lookup happens for arguments to functions too. So if you replace the argument, then the build might break because the namespace the old argument was in was needed to build. This is called ADL.

This
```
// for types
using NewType = OldType;

// for concepts
template <typename T>
concept MyShape = Shape<T>

// for templates
```

Pro-tip: Find a way to express pre/post syntax/semantics in a way that they co-exist. Rename away if needed. Convert individual calls as appropriate. Clean-up if needed. 

#### Change default precision for absl::StrCat
```
std::string result = absl::StrCat(SomeDouble());
// to refactor: step 1

std::string result = absl::StrCat(LegacyPrecision(SomeDouble()));
inline double LegacyPrecision(double d) { return d; }

// then: step 2
inline string_view LegacyPrecision(double d);

// Change the formatting precision of StrCat
```

## Changing Functions

Small atoms:
- Weaken preconditions
- Strengthen postconditions
Large atoms:
- Strengthen preconditions
- Weaken postconditions
### Changing return types
```
int foo (); // change to int64_t
void bar(int);
void f() {
  bar(foo());
}
```
Can you change long to int? Not without the renaming trick
```
long foo();
int f() {
  printf("%ld\n", foo()); // UB, or format string checking
  long val = std::min(foo(), SomeLong()); // deduction failure
}
```
## Changing types
Very difficult to do this. Avoid introducing vocabulary types. Vocabulary types are more expressive, but also harder to change. **What is a vocabulary type?**
## Changing a virtual interface
Also difficult
## Changing function templates
Same as functions unless there are specializations
## Changing concepts
Single step changes are possible but multi-step changes are not. Intensive multi-step refactoring involving renaming the concept to concept2 is probably the best option. 

Higher level expressive structures increase refactoring cost since the invariants have been spread globally into the codebase.
