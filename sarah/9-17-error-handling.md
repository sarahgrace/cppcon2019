# failures vs. bugs
* bug
  * caused by programmer
  * correct response -> change code
* failure
  * caused by software
  * correct reponse -> execute different code

# success dependency
* if we want to exit when something fails, that implied a dependency between the operations
  * `if A fails, cancel B`
* not cancelling the dependent operations
  * this is a bug
  * OK if following operations are not dependent
* force safety in checking success
  * [[nodiscard]] annotation: caller must read the returned value

# handling resources
* something we need to share with others, possibly ourselves (later)
* object lifetime  (RAII)
  * constructor acquires the resource
  * destructor releases the resource
* dependencies with resources
  * release depends on acquisition
  * usage depends on acquisition
  * release does NOT depend on usage
  * subsequent operations do not depend on resource release

# basic failure safety

## cancellation cascade
* unwides scope
* destroys objects in scopes
* no undefined behavior, memory leaks, etc.

## try catch blocks
* do not use the bad object in the catch block (we don't know what happened to it)
* if object is const -> this is ok because it's guaranteed that it's in a good state
* technically only applies to mutating operations

## scope guards
* use with caution!
* destructors can only touch members of the object
* scope guards can touch anything that was in scope -> there are a lot of opportunities to mess up

## when a member function fails
* object can be safely destroyed
* object invariants are preserved
  * valid but unspecified state
  * any operations without preconditions can be safely invoked

# using the model 
* should I throw an exception?
  * if caller has dependent operations, yes
* should I throw from the destructor?
  * do you understand all the "gotchas"?
* what if nobody catches my exception and the program terminates?
  * if all subsequent instructions depend on yours -> the whole program needs to be cancelled
  * there is a bug in the caller that fails to stop the failure cascade
    * this should be fixed
* what if my caller is not "exception safe"?
  * not observing cancellation cascade -> more serious problem
  * uses different failure handling technique -> maybe catch and translate to status code
* what if throwing an exception incurs an unacceptable cost?
  * they have certain performance trade offs, may not work in your domain
