/*
 *  Simple Shader Example
 *
 *  'p' to switch projections
 *  's' to start/stop light
 *  'o' to switch objects
 *  'a' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 *  PgUp/PgDn zooms in/out
 */
#include "CSCIx239.h"
#include "Object.h"
#include "Cube.h"
#include "Sphere.h"
#include "WaveOBJ.h"
#include "object.c"
#include<vector>
using namespace std;

int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int move=1;       //  Move light
int proj=0;       //  Projection type
int obj=0;        //  Object
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
vector<Object*> objs;  //  Array of objects
int shader[] = {0,0,0}; //  Shader program
char* text[] = {"No Shader","Simple Shader","Basic Shader"};

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (proj)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Select shader (0 => no shader)
   glUseProgram(shader[mode]);

   //  Export time to uniform variable
   if (mode)
   {
      float time = 0.001*glutGet(GLUT_ELAPSED_TIME);
      int id = glGetUniformLocation(shader[mode],"time");
      if (id>=0) glUniform1f(id,time);
   }

   //  Draw the model, teapot or cube
   glColor3f(1,1,0);
    objs[obj]->display();
//Cube(1,0,0 , 0.3,0.3,0.3);

   //  No shader for what follows
   glUseProgram(0);

   //  Draw axes - no lighting from here on
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s %s",
     th,ph,dim,proj?"Perpective":"Orthogonal",text[mode]);

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int xch,int ych)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int xch,int ych)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
      proj = 1-proj;
   //  Toggle objects
   else if (ch == 'o' || ch == 'O')
      obj = (obj+1)%4;
   //  Cycle modes
   else if (ch == 'm' || ch == 'M')
      mode = (mode+1)%3;
   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(proj?fov:0,asp,dim);
}


/*
 *  GLUT calls this routine every 50ms
 */
void timer(int k)
{
   glutPostRedisplay();
   glutTimerFunc(50,timer,0);
}

/*
 *  Set object states
 */
void Set(float zh)
{
    //  Change radius of green sphere
    ((Sphere*)objs[1])->radius(0.3+0.1*Sin(zh));
    //  Small brown bunny
    ((WaveOBJ*)objs[2])->color(0.6,0.4,0.2);
    ((WaveOBJ*)objs[2])->scale(2.0);
    // armadillo
    ((WaveOBJ*)objs[3])->color(0.5f, 0.0f, 1.0f);
    ((WaveOBJ*)objs[3])->scale(0.8);
}

/*
 *  Initialize object states
 */
void Init()
{
   objs.push_back(new Cube(1,0,0 , 0.3,0.3,0.3));
   objs.push_back(new Sphere(1,1,0 , 0.3 , 1,0,0));
   objs.push_back(new WaveOBJ("bunny.obj"));
   objs.push_back(new WaveOBJ("armadillo.obj"));
   Set(0.0);
}



/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   //  Translate time into 90 degree/sec angle
   Set(fmod(90*t,360.0));
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    //const char *verstr = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    //printf("%s", verstr);

   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Simple Shader");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   timer(1);
   //  Create Shader Programs
   shader[0] = CreateShaderProg("color.vert", "color.frag");
   shader[1] = CreateShaderProg("stripes.vert","stripes.frag");
   shader[2] = CreateShaderProg("basic.vert","basic.frag");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   Init();
   glutMainLoop();
   //printf("%s\n", glGetString(GL_VERSION));
   return 0;
}

