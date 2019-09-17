# scenario
* inherited legacy code
* it's valuable
* need to add feature or fix bug
* can't refactor without tests -> need refactoring to add tests

# golden master testing
1. input data
2. existing code
3. output data
4. save this as "golden master"

## tests in use
1. input data
2. updated code
3. output == golden master? -> pass, else fail

## pros
* good to start testing legacy systems
* good when goal is to keep behavior unchanged

# approval tests
* flexible golden master implementation
* works on c++11 and above, google tests
* useful to lock down behavior of existing code
  * combines with existing testing framework
  * not intended to replace unit tests

## how to use
* download header

```
#define APPROVALS_CATCH
#include "ApprovalTests.hpp"

Approvals::verify("output");
```

## running
1. first run: will always fail because it doesn't konw the expected output
2. approve the output
3. second run: pass!
* actual output -> <TestName>.received.txt 
* expected output -> <TestName>.approved.txt 

[documentation](https://www.messenger.com/t/theringocheung?mid=mid.%24cAAAAAB0iuRdzbZ_iX1tPFqZEygFS)






