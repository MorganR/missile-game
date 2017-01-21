// state.cpp


#include "headers.h"

#include "state.h"


// Draw each of the objects in the world


void State::draw() 

{
  int i;

  for (i=0; i<silos.size(); i++)
    silos[i].draw();

  for (i=0; i<cities.size(); i++)
    cities[i].draw();

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw();

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].draw();

  for (i=0; i<explosions.size(); i++)
    explosions[i].draw();
}


// Update the state of the world after some time interval, deltaT
//
// CHANGE ALL OF THIS FUNCTION


void State::updateState( float deltaT )

{
  int i;

  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missle generation
  // should increase with time.

  if (randIn01() < getMissileProbability_()) {	// New missile with probability 10%

    missilesIn.add( Missile( vector( randIn01(), worldTop, 0), // source
			     vector( -0.02, -0.1, 0 ),   // velocity
			     0,                      // destination y
			     incomingMissileColour ) );    // colour
  }

  // Look for terminating missiles

  for (i=0; i<missilesIn.size(); i++)
    if (missilesIn[i].hasReachedDestination()) {
      explosions.add(Circle(missilesIn[i].position(),
        0.2, // speed
        0.05, // max radius
        incomingMissileColour));
      missilesIn.remove(i);
      i--;
    }

  for (i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
      explosions.add(
        Circle(missilesOut[i].position(),
          0.2, // speed
          0.07, // max radius
          outgoingMissileColour));
      missilesOut.remove(i);
      i--;
    }

  seq<Circle> newExplosions;

  for (i = 0; i < explosions.size(); i++)
  {
    // Look for terminating explosions
    if (explosions[i].radius() >= explosions[i].maxRadius()) {
      // CHANGE THIS: CHECK FOR DESTROYED CITY OR SILO
      explosions.remove(i);
      i--;
      continue;
    }

    int j = 0;

    // Look for incoming missiles that hit an explosion and are
    // destroyed

    for (j = 0; j < missilesIn.size(); j++)
    {
      if (explosions[i].intersects(missilesIn[j].position()))
      {
        newExplosions.add(
          Circle(missilesIn[j].position(),
            0.2,
            0.06,
            incomingMissileColour));
        missilesIn.remove(j--);
      }
    }
  }

  for (i = 0; i < newExplosions.size(); i++)
  {
    explosions.add(newExplosions[i]);
  }

  // Update all the moving objects

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].move( deltaT );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (i=0; i<explosions.size(); i++)
    explosions[i].expand( deltaT );
}


// Fire a missile to the given position

void State::fireMissile( int siloIndex, float x, float y )

{
  const float speed = 0.3;
    
  if (silos[siloIndex].canShoot()) {

    silos[siloIndex].decrMissiles();

    vector siloPos = silos[siloIndex].position();
    vector direction = vector(x - siloPos.x, y - siloPos.y, 0).normalize();

    missilesOut.add( Missile( siloPos,   // source
			      speed * direction, // velocity
			      y,                             // destination y
			      outgoingMissileColour ) );           // colour
  }
}



// Create the initial state of the world


void State::setupWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;	// 6 seconds for incoming missile to arrive

  // Seed the random number generator

#ifdef _WIN32
  srand( 12345 );
#else
  struct timeb t;
  ftime( &t );
  srand( t.time );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vector( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vector( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vector( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vector( 0.2, 0, 0 ) ) );
  cities.add( City( vector( 0.3, 0, 0 ) ) );
  cities.add( City( vector( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vector( 0.6, 0, 0 ) ) );
  cities.add( City( vector( 0.7, 0, 0 ) ) );
  cities.add( City( vector( 0.8, 0, 0 ) ) );
}

float State::getMissileProbability_()
{
  return std::min(0.005 * currentTime, 0.10);
}
