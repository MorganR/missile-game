// Missile defence game


#include "headers.h"

// Window and world dimensions


#define WINDOW_X_POS   100
#define WINDOW_Y_POS    50

#define WINDOW_WIDTH   600
#define WINDOW_HEIGHT  400

#define WORLD_LEFT   -0.02	// The world lives in a [0,1] x [0,1] box
#define WORLD_RIGHT   1.02	// with a small (0.02) border all around.
#define WORLD_BOTTOM -0.02

float worldTop;			// calculated in main()
float windowWidth; // updated on reshape
float windowHeight; // updated on reshape


// ----------------------------------------------------------------
//
// The "state" contains the world and manages all actions.
//
// ----------------------------------------------------------------


#include "state.h"

State *state;

// ----------------------------------------------------------------
//
// Below are all the OpenGL event handlers.
//
// ----------------------------------------------------------------


void display()

{
  glClear( GL_COLOR_BUFFER_BIT );

  state->draw();

  glFlush();
  glutSwapBuffers();
}

void convertMouseCoordsToWorldCoords(int x, int y, float& wx, float& wy)
{
  // Calculate the world coordinates of mouse (x,y)
  // x, y - The position of the mouse in pixel coordinates with (0,0) at the top left corner of the game window
  // wx, wy - The position of the mouse in world coordinates, going [0,1] in each direction with (0,0) in the bottom left corner

  wx = (float)x / windowWidth * (WORLD_RIGHT - WORLD_LEFT) + WORLD_LEFT;
  wy = (1.0 - (float)y / windowHeight) * (worldTop - WORLD_BOTTOM) + WORLD_BOTTOM;
}

void fireMissile(int silo, int mouseX, int mouseY)
{
    float wx, wy;
    convertMouseCoordsToWorldCoords(mouseX, mouseY, wx, wy);

    state->fireMissile(silo, wx, wy);
}

void mouseClick( int button, int buttonState, int x, int y )
{
  if (buttonState == GLUT_DOWN) {

    // Shoot from silo 0, 1, or 2

    switch (button) {

    case GLUT_LEFT_BUTTON:
      fireMissile( 0, x, y );
      break;

    case GLUT_MIDDLE_BUTTON:
      fireMissile( 1, x, y );
      break;

    case GLUT_RIGHT_BUTTON:
      fireMissile( 2, x, y );
      break;
    }
  }

  glutPostRedisplay();
}

void keyPress( unsigned char c, int x, int y )

{
  switch (c) {
    case 'a':
      fireMissile(0, x, y);
      break;
    case 's':
      fireMissile(1, x, y);
      break;
    case 'd':
      fireMissile(2, x, y);
      break;
    case 27:
      glutLeaveMainLoop();
      exit(0);
  }

  glutPostRedisplay();
}

void reshape(int width, int height)
{
  glViewport(0,0,width,height);

  windowWidth = width;
  windowHeight = height;
}


void idleAction()

{
  static bool firstTimeThrough = true;
  static unsigned int previousTime;
  unsigned int currentTime;

#ifdef LINUX
  // Get the current time (measured in milliseconds since the Unix Epoch)
  struct timeb t;
  ftime( &t );
  currentTime = t.time * 1000 + t.millitm;
#else
  // Just guess at the current time interval (30/1000 seconds)
  currentTime = previousTime + 30;
#endif

  // Update the state.  Provide the number of seconds 
  // since the last update (if using Unix).

  if (!firstTimeThrough)
    state->updateState( (currentTime - previousTime) / 1000.0 );
  else
    firstTimeThrough = false;

  previousTime = currentTime;
  
#ifdef LINUX
  usleep( 30000 );		// sleep for 0.030 seconds
#endif

  glutPostRedisplay();
}


// ----------------------------------------------------------------
//
// The main program
//
// ----------------------------------------------------------------


int main( int argc, char **argv )

{
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInit( &argc, argv );

  glutInitWindowPosition( WINDOW_X_POS, WINDOW_Y_POS );
  glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
  glutCreateWindow( "Missile Defence" );

  glutReshapeFunc( reshape );
  glutDisplayFunc( display );
  glutMouseFunc( mouseClick );
  glutKeyboardFunc( keyPress );
  glutIdleFunc( idleAction );

  windowWidth = WINDOW_WIDTH;
  windowHeight = WINDOW_HEIGHT;
  worldTop = (WORLD_RIGHT - WORLD_LEFT) / (float) WINDOW_WIDTH * WINDOW_HEIGHT + WORLD_BOTTOM; 

  gluOrtho2D( WORLD_LEFT, WORLD_RIGHT, WORLD_BOTTOM, worldTop );

  std::cout << "top = " << worldTop  << std::endl;

  state = new State( worldTop );

  glutMainLoop();
  return 0;
}
