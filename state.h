// The state 


#ifndef STATE_H
#define STATE_H



#include "seq.h"
#include "vector.h"
#include "missile.h"
#include "silo.h"
#include "city.h"
#include "circle.h"


class State {

 public:

  State( float topCoord ) {
    worldTop = topCoord;
    setupWorld();
  }

  void setupWorld();
  void draw();
  void fireMissile( int siloIndex, float x, float y );
  void updateState( float deltaT );

 private:

  float currentTime;		// current game time

  float timeOfIncomingFlight;	// time taken for incoming missile to reach bottom

  float worldTop;		// world y coordinate at top of window

  // Gets the current probability of generating a new incoming missile.
  // The probability increases with time.
  float getMissileProbability_();

  seq<Silo> silos;
  seq<City> cities;
  seq<Missile> missilesIn;
  seq<Missile> missilesOut;
  seq<Circle> explosions;
};


#endif
