# benefits of inference
* makes generic programming like normal programming
* reduces boilerplace (don't have to specify template parameters)
* much better signal-to-noise
* improve maintainability
  * reducing template boilerplate
  * makes code less brittle/tightly-coupled
* *template vs normal is one of the most tightly coupled decisions in programming*

# two different models
1. runtime dispatch (inheritance)
2. compile-time dispatch (templates)

* support for both paradigms is an important part of why C++ is so successful
  * OO for simplicity
  * templates for performance
* downside: it's hard to switch between OO and templates

