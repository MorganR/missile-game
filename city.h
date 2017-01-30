// A city


#ifndef CITY_H
#define CITY_H


#include "buildings.h"


class City : public Building {
 public:

  // Constructors 

  City() {}

  City( vector pos ) : Building( pos, maxHealth ) {}

  // Draw the city

  void draw() {

    glColor3f( 1, (float)health/maxHealth, (float)health/maxHealth );

    glBegin( GL_POLYGON );
    glVertex2f( pos.x + halfWidth, pos.y );
    glVertex2f( pos.x + halfWidth, pos.y + height );
    glVertex2f( pos.x - halfWidth, pos.y + height );
    glVertex2f( pos.x - halfWidth, pos.y );
    glEnd();
  }

  // Check if the given explosion's circle intersects with the city rectangle
  bool isHit( vector explosionPos, float explosionR )
  {
    vector vectorBetween = (pos - explosionPos); // The vector between the center of the explosion and the center of the city.
    if (vectorBetween.length() < explosionR)
      return true;
    vector closestPoint = explosionPos + explosionR * vectorBetween.normalize();
    return closestPoint.x <= (pos.x + halfWidth)
      && closestPoint.x >= (pos.x - halfWidth)
      && closestPoint.y <= (pos.y + height)
      && closestPoint.y >= pos.y;
  }

  private: 
    const static int maxHealth = 5;
    const static float halfWidth = 0.04;
    const static float height = 0.03;
};
  


#endif
