// Types of building


#ifndef BUILDING_H
#define BUILDING_H


#include "vector.h"


class Building {
 public:

  // Constructors

  Building() {}

  Building( vector p, int h ) {
    pos = p;
    health = h;
  }

  // Draw the building

  virtual void draw() = 0;

  void damage(int d = 1)
  {
    health = health - 1;
  }

  bool isDestroyed()
  {
    return health <= 0;
  }

 protected:

  vector pos;			// centre
  int health;
};



#endif
