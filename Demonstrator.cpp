#include "BetterTClonesArray.h"
#include "Point.h"

// this works already:
void consumerhook(BetterTClonesArray<Point *> const &array) {
  array[0]->foo();

  Point *p = array[0];
  p->foo();

  for(auto &&p : array) {
    p->foo();
  }
}



int main() {
  TClonesArray *a = new TClonesArray("Point",100);
  // init actual objects with placement new
  for(int i=0;i<50;++i){
    new((*a)[i]) Point;
  }

  consumerhook(*a);

  return 0;
}


// Disatvantages of TClonesArray:
// * needs dictionary

// TODO:
// can we make it work implicitely for non-const references?
