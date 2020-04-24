#include <iostream>
#include "reflect.h"

class Base {
public:
  Base() {
    std::cout << "Base" << std::endl;
  }
};

re_class(Class1, Base) {
public:
  Class1() {
    std::cout << "Class1" << std::endl;
  }
};

re_class(Class2, Base) {
public:
  Class2() {
    std::cout << "Class2" << std::endl;
  }
};

int main(int argc, const char * argv[]) {
  reflect(Base)["Class1"]->create();
  return 0;
}
