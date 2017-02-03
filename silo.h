// A Silo


#ifndef SILO_H
#define SILO_H

#include "headers.h"
#include "buildings.h"
#include "TextGenerator.h"

class Silo : public Building {
 public:

  // Constructors 

  Silo() {}

  Silo( vector pos ) : Building( pos, maxHealth) {
    roundsLeft = 15;
  }

  bool canShoot() {
    return (roundsLeft > 0);
  }

  void decrMissiles() {
    roundsLeft--;
  }

  void incrMissiles(int num = 1) {
    roundsLeft += num;
  }

  vector position() {
    return pos;
  }

  // Returns true if the given explosion circle intersects with the silo's semicircle.
  bool isHit(vector explosionPos, float explosionR) {
    return (explosionPos.y + explosionR) >= pos.y // Only look at top half of silo 'circle'
      && (pos - explosionPos).length() < (radius + explosionR); // Check if silo 'circle' and other circle intersects
  }

  // Draw the silo

  void draw() {

    glColor3f( 1, (float)health / maxHealth, (float)health / maxHealth );

    glBegin( GL_POLYGON );
    for (float theta=0; theta<M_PI+0.01; theta += M_PI/32)
      glVertex2f( pos.x + radius * cos(theta), pos.y + radius * sin(theta) );
    glEnd();

    char buffer[5];
    sprintf(buffer, "%d", roundsLeft);
    TextGenerator::PrintString(buffer, pos.x - 0.008, pos.y + 0.013, GLUT_BITMAP_TIMES_ROMAN_10, vector(0.0, 0.0, 0.0));
  }

 private:
  const static int maxHealth = 15;
  const static float radius = 0.04;
  int roundsLeft;
};
  

#endif
