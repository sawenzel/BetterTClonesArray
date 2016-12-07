#include "Point.h"
#include <iostream>

ClassImp(Point);

void Point::foo() const {
  std::cerr << "Point foo() called id " << mId << "\n";
}

size_t Point::gInstanceCount=0;
