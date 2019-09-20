# definitions
* cardinality: # of times an object can be associated with another object
* modality: minimum # of times
* predicate: relationship which may or may not be satisfied
* constraint: a relationship which must be satisfied
* implication: a implies b (a -> b)

## notation
* forms a bipartite graph
* entities: rectangle
* relationships: circle
* implication: directional edges

# relationships
* object copied/moved -> relationship is maintained/severed w.r.t. destination object
* object destructed -> relationship is severed

## witness relationship
a relationship represented by an object
* as an object, it's copyable and equality comparable
* if copied/moved -> relationship maintained/severed/invalidated (lost it's meaning)
  * might choose not to implement copy/move for witnessed relationships
* if destructed -> relationship severed/invalidated

## whole-part relationship
* HAS-A relationship
  * connected
  * non-circular
  * lgcally disjoint
  * owning
* simplifies reasoning about an object

## safety
* an object instance without meaning is invalid
* an object in an invalid state must either be restored to a valid state or destroyed (partially formed)
* an operation which leaves an object in an invalid state is *unsafe*
  * `std::move` is unsafe, it leaves moved object in an invalid state

# visualizing relationships

## pattern matching
* concepts: compile time constraint to select an appropatiate operation
* contract: assert at runtime if an operation preconditions are met
* pattern matching: runtime contraint to select an apprprate operation

## tips
* use strong preconditions to move the issue to the caller
* meanings: setting a property: two objects setting the value on a shared object
  1. code is redundant
  2. different aspects of the same relationstiop, reperesented in disparate sections of code
  3. mutually exclusive
  4. implied "last in wins" relationship
  5. indidental algorithm -> property converges to the correct value
  
