#include <windows.h>
#include <iostream>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

using namespace std;

// constants
#define WINDOW_W 512
#define WINDOW_H 512
#define CAM_FOV  45.0f
#define CAM_NEAR 0.0f
#define CAM_FAR  1000.0f

// global vars
int mouseX, mouseY;
int mouseButtons = 0;
float rotateX = 0.0, rotateY = 0.0;
float translateZ = -5.0;

// global methods
void initGL();
void resize(int width, int height);
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

  // init GL
  initGL();

  // register callbacks  
  glutDisplayFunc(draw);
  glutIdleFunc(draw);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

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

void draw() {
  // clear color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // reset view matrix
  glLoadIdentity();

  // transform view matrix; HACK camera
  glTranslatef(0.0, 0.0, translateZ);
  glRotatef(rotateX, 1.0, 0.0, 0.0);
  glRotatef(rotateY, 0.0, 1.0, 0.0);

  // draw geometry
  GLUquadric* sphere;
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_LINE );
  glPushMatrix();
  glTranslatef(0.0f, 0.0f,0.0f);
  gluSphere(sphere, 0.30, 20, 20);
  glPopMatrix();

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