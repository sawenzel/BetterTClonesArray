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

void testConstruction(BetterTClonesArray<Point *> array) {
  auto p = array.ConstructedAt(30);
  p->foo();

  // Note that the following version no longer compiles
  // thanks to deleted operator[]
  // new (array[30]) Point();
}


int main() {
  TClonesArray *a = new TClonesArray("Point",100);
  // init actual objects with placement new
  for(int i=0;i<50;++i){
    if (i!=30) new((*a)[i]) Point;
  }

  std::cerr << " " << a->GetSize() << "\n";
  std::cerr << " " << a->GetEntries() << "\n";
  std::cerr << " " << a->GetLast() << "\n";
  std::cerr << " " << a->At(30) << "\n";
  std::cerr << " " << a->UncheckedAt(30) << "\n";
  std::cerr << " " << const_cast<TClonesArray const *>(a)->operator [](30) << "\n";
  std::cerr << " " << a->At(30) << "\n";
  std::cerr << " " << a->UncheckedAt(30) << "\n";


  consumerhook(*a);

  testAssignment();
  testConstruction(*a);
  std::cerr << " " << a->UncheckedAt(30) << "\n";

  return 0;
}


// Disatvantages of TClonesArray:
// * needs dictionary
