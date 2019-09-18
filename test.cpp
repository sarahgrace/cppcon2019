#include <memory>
#include <iostream>
#include <string>

struct Bar {
  std::string s{"abcd"};
  std::ostream& operator<<(std::ostream& out, const Bar& bar) {
    return out << bar.s << std::endl;
  }
};

template <typename T>
void foo(T&& a) {
  std::cout << std::forward<T>(a) << std::endl;
  std::cout << std::forward<T>(a) << std::endl;
}

int main() {
  foo(Bar());
}
