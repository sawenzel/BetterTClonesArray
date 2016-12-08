#include "BetterTClonesArray.h"
#include "Point.h"
#include <iostream>

// this works already:
// (demonstrating type safety; automatic type deduction; C++11 ranged based iteration
void consumerhook(BetterTClonesArray<Point *> const &array) {
  array[0]->foo();

  Point *p = array[0];
  p->foo();
  for (auto &&p : array) {
    p->foo();
  }
}

// testing whether I can pass a typed array to an interface expecting a bare TClonesArray
void interface(TClonesArray *) {
  std::cerr << "side effect\n";
}

// testing whether I can pass a typed array to an interface expecting a bare TClonesArray
void constinterface(TClonesArray const *) {
  std::cerr << "side effect\n";
}

void callInterface(BetterTClonesArray<Point *> &array) {
  interface(array); // this works due to implicit casting
  interface(array.raw()); // this works due to the more explicit raw() function
}

// testing whether assignment works (imagine interface returning bare TClonesArray)
void testAssignment() {
  BetterTClonesArray<Point *> n = new TClonesArray("Point");
  callInterface(n);
}

int main() {
  TClonesArray *a = new TClonesArray("Point",100);
  // init actual objects with placement new
  for(int i=0;i<50;++i){
    new((*a)[i]) Point;
  }

  consumerhook(*a);

  testAssignment();

  return 0;
}


// Disatvantages of TClonesArray:
// * needs dictionary
