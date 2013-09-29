#include "Utils.h"
#include "Ball.h"

using namespace std;

// constants
#define WINDOW_W 800
#define WINDOW_H 480
#define CAM_FOV  45.0f
#define CAM_NEAR 0.0f
#define CAM_FAR  1000.0f

// global vars
namespace {
  int mouseX, mouseY;
  int mouseButtons = 0;

  // camera
  float rotateX = 30.0;
  float rotateY = 0.0;
  float translateZ = -100.0;

  uint timer = 0.0f, timerElapsed = 0.0f;
  uint frameCount = 0, timeBase = 0;  // fps calc

  vector<Ball> balls;
}

// global methods
void initGL();
void initScene();
void resize(int width, int height);
void update();
void draw();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

int main( int argc, char** argv) {
  // init glut
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WINDOW_W, WINDOW_H);
  glutCreateWindow("GL Window");

  // register callbacks  
  glutDisplayFunc(update);
  glutIdleFunc(update);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  initGL();
  initScene();

  // start main loop
  glutMainLoop();

  return 1;
}

void initGL() {  
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

  // depth buffer
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    
  // enable GL features and settings
  glShadeModel(GL_SMOOTH);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);

  glPointSize(3.0f);
}

void resize(int width, int height) {  
  glViewport(0, 0, width, height);

  // reset projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // calculate aspect ratio
  gluPerspective(CAM_FOV, (GLfloat)width/(GLfloat)height, CAM_NEAR, CAM_FAR);

  // reset modelview matrix
  glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void getFPS() {
  ++frameCount;
  uint currTime = glutGet(GLUT_ELAPSED_TIME);

  uint elapsed = currTime - timeBase;
  if (elapsed > 1000) {
    float fps = frameCount*1000.0f/elapsed;
    uint millisecs = elapsed / frameCount;
    timeBase = currTime;
    frameCount = 0;

    char buffer[32];
    sprintf(buffer, "%.2f : %i", fps, millisecs);
    glutSetWindowTitle(buffer);
  }
}

void update() {
  getFPS(); 

  // compute time
  timerElapsed = glutGet(GLUT_ELAPSED_TIME) - timer;
  timer += timerElapsed;

  // update objects
  for (int i=0; i<balls.size(); ++i)
    //balls[i].testUpdate();
    balls[i].simStep(timerElapsed / 1000.0f);

  draw();
}

void draw() {
  // clear color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // reset view matrix
  glLoadIdentity();

  // transform view matrix; HACK camera
  glTranslatef(0.0, 0.0, translateZ);
  glRotatef(rotateX, 1.0, 0.0, 0.0);
  glRotatef(rotateY, 0.0, 1.0, 0.0);

  // draw objects
  for (int i=0; i<balls.size(); ++i)
    balls[i].draw();

  // draw bounds
  glBegin( GL_POINTS);
  glVertex3f(B_H_WIDTH, 0.0f, B_H_HEIGHT);
  glVertex3f(-B_H_WIDTH, 0.0f, B_H_HEIGHT);
  glVertex3f(-B_H_WIDTH, 0.0f, -B_H_HEIGHT);
  glVertex3f(B_H_WIDTH, 0.0f, -B_H_HEIGHT);
  glEnd();

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  switch(key) {
  case(27) : exit(0);
  }
}

void mouse(int button, int state, int x, int y) {
  printf("click %x\n", button);

  // set/clear bits
  if (state == GLUT_DOWN) {
    mouseButtons |= 0x1 << button;
  } else if (state == GLUT_UP) {
    mouseButtons &= ~(0x1 << button);
  }

  mouseX = x;
  mouseY = y;
}

void motion(int x, int y) {
  float dx, dy;
  dx = (float)(x - mouseX);
  dy = (float)(y - mouseY);

  if (mouseButtons & 0x1) {
    rotateX += dy * 0.2f;
    rotateY += dx * 0.2f;
  }
  else if (mouseButtons & 0x4) {
    translateZ += dy * 0.01f;
  }

  mouseX = x;
  mouseY = y;
}

void initScene() {
  balls.push_back(Ball(0.0f, 0.0f, 0.0f));
  balls[0].v.x = 3.0f;
  balls[0].v.z = 3.0f;
  //balls[0].w.z = -50.0f;
}