// Missile class


#ifndef MISSILE_H
#define MISSILE_H


#include "vector.h"

class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vector p, vector v, float y, vector c ) {
    pos0 = p;
    pos1 = p;
    velocity = v;
    colour = c;
    destY = y;
    maxLength = 0.2;
  }

  // Draw the missile and its trail

  void draw() {
    glBegin( GL_LINES );
    glColor4f( 0.0, 0.0, 0.0, 0.0 );
    glVertex2f( pos0.x, pos0.y );
    glColor4f( colour.x, colour.y, colour.z , 1.0);
    glVertex2f( pos1.x, pos1.y );
    glEnd();
  }

  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;
    adjustTail();
  }

  // Adjust the tail length so the tail is <= maxTail;

  void adjustTail()
  {
    vector p1To0 = pos0 - pos1;
    float mag = p1To0.length();
    if (mag > maxLength)
    {
      pos0 = (p1To0 / mag) * maxLength + pos1;
    }
  }

  // Return the current position 

  vector position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() {
    return velocity.y > 0 ? pos1.y >= destY : pos1.y <= destY;
  }

 private:

  vector pos0;			// start position
  vector pos1;			// current position
  vector velocity;		// velocity
  vector colour;		// colour of missile trail
  float  destY;			// y position at destination
  float maxLength; // Maximum visible tail length
};


#endif
