
# Testing legacy C++ code
## Goal: explain techniques used to safely add features to legacy code and testing it

[Documentation](https://github.com/approvals/ApprovalTests.cpp/blob/master/doc/README.md)

Legacy code is code without unit tests/profitable code that we feel afraid to change/any code that you want to change, but are afraid to change it

Problem: Want to refactor code so that you can test it, but the code has no tests so refactoring it is dangerous.

**Locking down current behavior** 
Golden Master Testing:
1. Feed some input into system
2. Save the output as the "golden master"

Allows you to capture current behavior of legacy code, then when you change the system you can run the golden master tests. 
Issues:
1. capturing output can be difficult
2. easy to overwrite golden master 

ApprovalTests.cpp is the Github name of the repo. 

How to use it:
```
//main.cpp
#define APPROVALS_CATCH
#include "ApprovalTests.hpp"
```
Catch2 test with ApprovalsTest framework
```
#include "Catch.hpp"
#include "ApprovalTests.hpp"

// Approvals test - test static value, for demo purposes
// You'd want this to run your real code in approval testing
TEST_CASE("TestFixedInput") {
  Approvals::verify("Some string");
}
```
**Note that your test has to run your source code**
Approval tests use 'approved' instead of 'expected' and 'received' instead of 'actual'. 
The first time you run an approval test, it'll fail because you haven't told it what output has been approved yet. The framework will write the approved tests to a .txt file for each test.
Now on the second run, the output has been approved so the test passes. 
Think of approval tests as an **addition** to test framework. It's an easy way to test larger, more complex things. Useful for locking down behavior of existing code. (Maybe we can make these for different protocols?)

As you write tests, the directory will become cluttered with these approved files. If you want this to be suppressed, you can put a subdirectory in the source code and it will save all approved outputs to this subdirectory.

## Application to legacy code
```
TEST_CASE("New test of legacy feature") {
  // Standard pattern:
  // Wrap your legacy feature to test in a function call
  // that returns some state that can be written to a text file for verification:
  const LegacyThing result = doLegacyOperation();
  Approvals::verify(result);
}
```
**If you rename your tests, then you'll have to rename your .approved files**
- Consistent over operating systems: newlines and carriage return characters work as expected
## Quick to write tests
The idea is that you can check approvals on a bunch of combinations by putting inputs in a container and having the .approved file have many input-output pairs
```
TEST_CASE("verifyAllWithHeaderBeginEndAndLambda") {
  std::list<int> numbers{1,2,3};
}
```
## Quick to get good coverage
Can extend this to multiple combinations as well, so if the legacy system takes in two inputs then you can have the .approved file run on all combinations of the two inputs.
```
TEST_CASE("verifyAllCombinationsWithLambda") {
std::vector<std::string> strings{"hello", "world"};
  std::vector<int> numbers{1,2,3};
  CombinationApprova
}
```
## Customizable
ApprovalTests allows you to customize the underlying pieces. In particular, the **reporter** which runs on test failure can be overwritten to run whatever you want. This allows you to change the reporter for just one test. 
## Convention over Configuration
- By default you can usual behaviors, people can specify unusual behaviors (such as a custom reporter) 
## Challenges
- What if the approved file has things that change such as being a log file with datetimes? One approach is stripping the datetimes from the .approved files

## Approving images (Clare's use case)
**review her slides b/c her example is really good**

