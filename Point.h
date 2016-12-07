#pragma once
#include "TObject.h"

// very simple class just for demonstrating purposes
struct Point : public TObject {
  Point() : x(-1), y(-1), z(-1), mId{gInstanceCount++} { }
  double x=-1,y=-1,z=-1;

  void foo() const;

  static size_t gInstanceCount;
  size_t mId;

  ClassDef(Point, 1);
};
