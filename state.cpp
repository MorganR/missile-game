// state.cpp

#include <sstream>

#include "headers.h"

#include "state.h"

#include "TextGenerator.h"


// Draw each of the objects in the world


void State::draw() 

{
  int i;
  
  if (currentTime < 1)
    TextGenerator::PrintString("GOOD LUCK", 0.37, 0.38, GLUT_BITMAP_TIMES_ROMAN_24, vector(1.0, 1.0, 1.0));

  if(isGameOver)
  {
    TextGenerator::PrintString("GAME OVER", 0.37, 0.38, GLUT_BITMAP_TIMES_ROMAN_24, vector(1.0, 0.0, 0.0));
    std::stringstream sstream;
    sstream << "You survived for: " << currentTime << " seconds";
    TextGenerator::PrintString(sstream.str().c_str(), 0.39, 0.28, GLUT_BITMAP_TIMES_ROMAN_10, vector(1.0, 0.0, 0.0));
    return;
  }

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

  if (isGameOver)
    return;

  // Update the time

  currentTime += deltaT;
  timeSinceMissileUpdate += deltaT;

  // Generate some new missiles.  The rate of missle generation
  // should increase with time.

  if (randIn01() < getMissileProbability_()) {	// New missile with time varying probability
    float xVel = 0.02 + -0.04 * randIn01();
    missilesIn.add( Missile( vector( randIn01(), worldTop, 0), // source
			     vector( xVel , -0.1, 0 ),   // velocity
			     0,                      // destination y
			     incomingMissileColour ) );    // colour
  }

  if (timeSinceMissileUpdate > 7)
  {
    for (i = 0; i < silos.size(); i++)
    {
      silos[i].incrMissiles(5);
    }
    timeSinceMissileUpdate = 0;
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

  // Holds new explosions from missile destruction
  // Used to avoid looping over new explosions within the following loop
  seq<Circle> newExplosions;

  for (i = 0; i < explosions.size(); i++)
  {
    int j;
    // Look for terminating explosions
    if (explosions[i].radius() >= explosions[i].maxRadius()) {
      // CHANGE THIS: CHECK FOR DESTROYED CITY OR SILO
      for (j = 0; j < silos.size(); j++)
      {
        if (silos[j].isHit(explosions[i].position(), explosions[i].maxRadius()))
        {
          silos[j].damage();
          if (silos[j].isDestroyed())
          {
            silos.remove(j--);
          }
        }
      }

      for (j = 0; j < cities.size(); j++)
      {
        if (cities[j].isHit(explosions[i].position(), explosions[i].maxRadius()))
        {
          cities[j].damage();
          if (cities[j].isDestroyed())
          {
            cities.remove(j--);
          }
        }
      }

      explosions.remove(i);
      i--;
      continue;
    }


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

  // Move new explosions into the main explosions sequence
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

  isGameOver = (silos.size() == 0 && cities.size() == 0);
}


// Fire a missile to the given position

void State::fireMissile( int siloIndex, float x, float y )

{
  const float speed = 0.3;
    
  if (silos.size() == 0)
  {
    return;
  }

  siloIndex = std::min(siloIndex, silos.size() - 1);

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
