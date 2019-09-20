# intro
* how do I write functions that accept lambdas as arguments?
* templating
  * must be defined in the header file
  * code bloat
  * slow to complie

```
// needs implementation in header file
template <class Func>
void for_each_book(Func f) {
  for (const Book& b : books) {
    f(b);
  }
}

// can define this anywhere now
void for_each_book(ConcreteFuncType f) { ...}
```

## type erasure
* "concretifies" templates
* we want:
  * type safe
  * can be defined out of line (don't pay for template bloat"
* *representation + behavior = data type*

```
// suppose func called with no arguments returns int
template <class Callanbble>
void foo (Callable& func) ( 

// pointer to the function
void *representation = &callable;
// cast return value to int
int (*behavior) (void*) = +[](void* r) {
  // cast r (void* ) to a Callable*
  return (*(Callable *) r)();
};
// behavior of the callable equals calling the callable
assert(behavior(representation) == func());
```

## type-erased objects

### type-erased object with reference semantics
```
struct TypeErasedNumber {
template<class Number> 
TypeErasedNumeRef(Number& n) :
  representation_((void*)&n),
  negate_([](void *r) -> int { return ~(*(Number*) r); }),
  not_([](void *r) -> bool { return !(*(Number*) r); }) {}

  void *representation_;
  int (*negate_) (void*);
  bool (*not_) (void*);
  int operator ~() const { return negate_(representation_); }
  int operator !() const { return not_(representation_); }
};
```

### type-erased object with value semantics
```
struct TE {
  void *rep_;
  void (*delete_) (void*);
  void* (*clone_) (void*);

  // constructor
  template<class Number>
  TE(Number n) {
    repr_(new Number(std::move(n))),
    delete_([](void* r) { delete (Number*) r; }),
    clone_([](void* r) { return new Number(*(Number*) r); }) {}

  // copy constructor
  TE(const TE& rhs) {
    repr_(rhs.clone_rhs.repr_)),
    clone_(rhs.clone_) {}

  // destructor
  ~TE() { delete_(repr_); }
};
```

### type-erased object with defined behaviors outside of the class definition

```
struct TE_behaviors {
  void (*doA_) (void*);
  void (*doB_) (void*);
  void (*doC_) (void*);
};
```

### type safe type-erased object

```
// base
struct TEBase {
virtual unique_ptr<TEBase> clone() const = 0;
virtual int negate() const = 0;
virtaul bool not_() const = 0;
virtual ~TEBase() = default;
};

// child
template <class Number>
struct TED : public TEBase {
  Number num_;
  explicit TED(Number n) : num_(std::move(n)) {}

  unique_ptr<TEBase> clone() const override {
    return std::make_unique<TED>(num_);
  }
  int negate() const override { return -num_; }
  bool not_() const override { return !num_; }
};

```

### `std::any_cast`

```
struct AnyBase {
  virtual unique_ptr<AnyBase> clone() const = 0;
  virtual void *addr() = 0;
  virtual ~AnyBase() = default;
};

template <class T>
struct AnyD : public AnyBase {
  T value_;
  explicit AnyD(const T& t) : value(t) {}
  
  unique_ptr<AnyBase> clone() const override {
    return std::make_unique<AnyD>(value_);
  }

  void *addr() override { return &value_; }
};

struct any {
  unique_ptr<AnyBase> p_ = nullptr;

  template<class T>
  explicit any(const T& t) : p_(std::make_unique<AnyD<T>>(t)) {}

  any(const any& rhs): p_(rhs.p_->clone()) {}
  any(any&&) noexcept = default;
  any& operator=+(any rhs) { ... }
  ~any() = default;
};

// implementation of any_cast
template <class U>
U any_cast(any& a) {
  AnyBase *b = a.p_.get();
  if (auto *d = dynamic_cast<AnyD<U>*>(b)) {
    return *(U*)a.p_->addr();
  }
  throw std::bad_any_cast();
}

// usage
any mya(42);
int i = any_cast<int>(mya);
```

# conclusion
* std::function and std::any are done with type erasure
* lets us pass arbitrary types across ABI boundaries
  * flexiility of templates with speed of separate compilation
* it's not too hard to write by yourself!
  1. list your affordances
  2. make a vtable (manually or using virtual methods)
  3. initialize each behavior
    1. in a constructor template with a bunch of lambdas
    2. with a derived class template
* copyability and destructability are just other affordances
