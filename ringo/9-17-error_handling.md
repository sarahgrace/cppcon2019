# Error handling

## Failures vs. Bugs
### Bug
- caused by programmer
- correct response is to change code
- detected by static analysis
- different tool for handling needed (type system, contracts)
Example
```
double b = any_val();
double d = x*x+y*y;
double dist = sqrt(b); // should be sqrt(d)
```
### Failure
- correct response is to execute different code
Contract of a failure is either open a socket OR report failure
```
int open_socket();
// contract
// either open a socket OR
// reports failure
```
Error handling is like success dependency, avoid calling operations that depend on previous operations if the previous operations fail

There are cases when we can stop the cancellation cascade. For example, in Multiway, the overall catch in MultiwayWorker does this. 

apply [[nodiscard]] to functions that MUST return an error code can be useful because if you forget to return an error code from a function, then the compiler will warn you. 

### Handling resources
Resource dependencies. Use RAII to guarantee resource destruction even when exceptions are thrown:
```
Status get_data_from_server() {
  Socket socket{};
  if (failed) return failure();
  
  connect();
  if (failed) return failure();

  obtain_data();
  if (failed) return failure();
  
  return success();
} // socket is closed when this function returns b/c of RAII
```
A function's contract is not to manage a resource, callers of function don't care what resources that the function manages.

**We don't throw when something fails, we throw when we want to cancel an operation.**

## Logging
We want to cancel logging when the dependent
Logging should not crash your program, a good logging framework should just return a failure code 

## Failure safety
```
class Person {
  string first_name;
  string last_name;
  Person& Person::operator=(Person const& p) {
    first_name = p.first_name;
    last_name = p.last_name;
    return *this;
  }
};

void process(Person &p);
// precondition: p is existing Person

void fun() {
  Person p = next_person();
  process(p);
  p = next_person(); // this will throw
  process(p); // process will never see the invalid person
}
```
Another example where you **do** want to catch:
the good/badness of rq cannot change after the try catch because rq is const.
```
while (server_is_up) {
  const Request rq = queue.pop();
  try {
    process(rq);
  }
  catch (exception const& e) {
    report_failure(rq, e);
  }
}
```
Scope guards should be used with caution since they can touch anything in scope
## Basic failure safety
- When a function fails: 
  - no resources are leaked (due to RAII)
- When member function fails:
  - leave object in a state where it can be safely destroyed
  - object's invariants are preserved (valid but unspecified state). 
  - Any operation without preconditions can be safely invoked

## Using the model
Q: Should I catch the exception?
A: Do subsequent operations depend on the one that failed? If so, don't catch the exception

Q: Should I throw an exception?
A: Do callers need to cancelled? If yes, then throw

Q: Should I throw from destructor?
A: Using a destructor for something other than releasing resources? Do you understand the gotchas? You generally don't want to do this.

Q: What if nobody catches my exception and the program terminates?
A: Either all subsequent instructions depend on yours, in that case all of the program needs to be cancelled 
OR there is a bug in the caller that fails to stop the cascade

Q: What if my caller is not exception-safe?
A: What do you mean by exception-safe?
Not observing the cancellation cascade? Then this is the caller's problem
Caller is using a different failure-handling technique? Then indeed, need to adapt.

Q: What if throwing an exception incurs unacceptable cost?
A: Sometimes it is an unjustified belief, sometime it is really the case: 
  Exceptions have performance trade-offs, they may not work in your domain
