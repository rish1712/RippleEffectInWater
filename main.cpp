#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>


#define BOUNDS	1


//size of the water
#define scene 350
// attenuationRate
#define attenuationRate  32


/*!amplitudes attached to each point 3-d array to store amplitudes at initial an final point
*/
float amplitude[2][scene][scene];

/*!x-y rotation and zoom
*/
int rot_x, rot_y, rot_z;
/*!height, width	of window
*/
int h, w;
/*! used for window fullscreen toggling
*/
int old_x, old_y, move_z;
int i = 0;

/*! light source position
*/
GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };
// normal array
GLfloat norm[3];

/*!'t' for initial state and 'f' for final state
*/
int s = 0, f = 1;

/*!calculating the effect of amplitudes by four neighboring points
*/
void calcAmplitude()
 {
        int x, y;
		float n;
		for(y = 1; y < scene-1; y++)
        {
            for(x = 1; x < scene-1; x++)
            {   /*!
                    using the fact from the paper that the value of a=1/2 and b=-1
                */
                n = ( amplitude[s][x-1][y] +amplitude[s][x+1][y] +amplitude[s][x][y-1] +amplitude[s][x][y+1])/2;
                n -= amplitude[f][x][y];
				n = n - (n / attenuationRate );
                amplitude[f][x][y] = n;
            }
		}
        /*!
         calculating the effect of amplitudes of the border points on these neighboring points that is the points which correspond to the edge y=0 of
         the water surface.
		*/
		y = 0;
		for(x = 1; x < scene-1; x++)
		{
            n = ( amplitude[s][x-1][y] +amplitude[s][x+1][y] +amplitude[s][x][y+1]) /2;
            n -= amplitude[f][x][y];
            n = n - (n / attenuationRate );
            amplitude[f][x][y] = n;
		}

        /*!
         calculating the effect of amplitudes of the border points on these neighboring points that is the points which correspond to the edge x=0 of
         the water surface.
		*/

		x = 0;
		for(y = 1; y < scene-1; y++)
        {
            n = ( amplitude[s][x+1][y] +amplitude[s][x][y-1] +amplitude[s][x][y+1])/2;
            n -= amplitude[f][x][y];
            n = n - (n / attenuationRate );
            amplitude[f][x][y] = n;
		}
        /*!
         calculating the effect of amplitudes of the border points on these neighboring points that is the points which correspond to the edge x=scene-1 of
         the water surface.
		*/
		x = scene-1;
		for(y = 1; y < scene-1; y++)
        {
            n = ( amplitude[s][x-1][y] +amplitude[s][x][y-1] + amplitude[s][x][y+1])/2;
            n -= amplitude[f][x][y];
            n = n - (n / attenuationRate );
            amplitude[f][x][y] = n;
		}
        /*!
         calculating the effect of amplitudes of the border points on these neighboring points that is the points which correspond to the edge y=scene-1 of
         the water surface.
		*/
		y = scene-1;
		for(x = 1; x < scene-1; x++)
        {
            n = ( amplitude[s][x-1][y] +amplitude[s][x+1][y] +amplitude[s][x][y-1])/2;
            n -= amplitude[f][x][y];
            n = n - (n / attenuationRate );
            amplitude[f][x][y] = n;
		}


}

/*!
   To find the calculateNormals for the lines made by three given points The arguments of this function are the 3 points
   which consists of the triangle.
   The return type of this function is void
*/
void calculateNormals(float x1, float y1,float	z1,//point 1
            float x2,float	y2,float	z2,//point 2
            float x3,float	y3,float	z3)//point 3
{
		/*!
        calculate [1 - 2]x[2 - 3] -- the surface calculateNormals for a triangle
        */
        float v1[3], v2[3];
        float dist;

        /*!
        point1 - point2
        */
        v1[0] = x1 - x2;
        v1[1] = y1 - y2;
        v1[2] = z1 - z2;

        /*!
         point2 - point3
        */
        v2[0] = x2 - x3;
        v2[1] = y2 - y3;
        v2[2] = z2 - z3;

        /*!
        now the cross product to calculate the direction ratios
        */
        norm[0] = v1[1]*v2[2] - v1[2]*v2[1];
        norm[1] = v1[2]*v2[0] - v1[0]*v2[2];
        norm[2] = v1[0]*v2[1] - v1[1]*v2[0];

        /*!
         magnitude of normal vector
        */
        dist = sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);

        /*!
           to get a unit vector along the normal
        */
        norm[0] /= dist;
        norm[1] /= dist;
        norm[2] /= dist;

}

/*!
   This function is used to display the ripple effect. This function type is void.
   The return type of this function is also void
 */
void display(void)
{
        int  j, temp;
		float w = scene/2;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*!
         specifying the light and the lighting position. It has 3 arguments. 1st argument specifies the light
         i.e number of lights. The second one specifies the light source parameter for light. The third one
         specifies the position of the light source.
        */
        glLightfv (GL_LIGHT0, GL_POSITION, position);

		glPushMatrix();
		glTranslatef (-1.0, 1.0, -10);
		/*!
            disable server-side GL capabilities
		*/
		glDisable (GL_LIGHTING);
		glColor3f (0.0, 1.0, 1.0);
		/*!
		 indication of light source by a small cube of edge 0.1.
		*/
		glutSolidCube (0.1);
		/*!
            enabling server-side GL capabilities
		*/
		glEnable (GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
        glTranslatef(0, 0, rot_z-200);
        glRotatef(rot_x, 0, 1, 0);

        glRotatef(rot_y-60, 1, 0, 0);

		calcAmplitude();
        glColor3f(0, 1, 1);
		/*!
		 To show ripple effect we are drawing triangles to represent waves
		*/
		glBegin(GL_TRIANGLES);
		for(i = 0; i < scene-1; i++)
        {
            for(j = 0; j < scene-1; j++)
            {
                calculateNormals(j-w, i-w, amplitude[s][j][i],//point1
                        j+1-w, i-w, amplitude[s][j+1][i],//point2
                        j+1-w, i+1-w, amplitude[s][j+1][i+1]);//point3
                /*!
                   This function is used to define the normal vector of the geometry faces so that lighting calculations can be done.
                */
                glNormal3fv(norm);
                glVertex3f(j-scene/2, i-scene/2, amplitude[s][j][i]);
                glVertex3f(j+1-scene/2, i-scene/2, amplitude[s][j+1][i]);
                glVertex3f(j+1-scene/2, i+1-scene/2, amplitude[s][j+1][i+1]);

                calculateNormals(j-w, i+1-w, amplitude[s][j][i+1],//point1
                        j-w, i-w, amplitude[s][j][i],//point2
                        j+1-w, i+1-w, amplitude[s][j+1][i+1]);//point3

                /*!
                   This function is used to define the normal vector of the geometry faces so that lighting calculations can be done.
                */
                glNormal3fv(norm);
                glVertex3f(j-scene/2, i+1-scene/2, amplitude[s][j][i+1]);
                glVertex3f(j-scene/2, i-scene/2, amplitude[s][j][i]);
                glVertex3f(j+1-scene/2, i+1-scene/2, amplitude[s][j+1][i+1]);
            }
		}
		glEnd();

        /*!
        swapping initial and final amplitudes as initial are store in 0th index of amplitudes array and
        final is stored in index 1
		*/
		temp = s;
		s = f;
		 f = temp;

        glPopMatrix();
        glutSwapBuffers();
}

int num	= 0;
/*!
 delay between random ripples
*/
int delay = 50;

/*!
   This function is used to make random raindrops and corresponding ripples by using rand function in C library
   The arguments of this function is void.
   The return type of this function is also void
*/
void idle(void)
{

	if(!(++num %delay))
    {
		amplitude[f][rand()%scene][rand()%scene] = -rand()%300+10;
		delay = rand()%60 + 100;
	}
	glutPostRedisplay();
}

/*!
  This function is used to exit the execution.
  The arguments of this function is void.
  The return type of this function is also void
*/
void bail(int code)
{
	exit(code);
}

/*!
 Used for rotating using left click.This function has 4 arguments. The first one is to decide whether left button or right button is clicked
    one to check whether there is release or press of the mouse pointer. The other 2 arguments are used to bood the x and y coordinates of the point clicked on the screen.
    the return type of this function is void
*/
void mouse(int button, int state, int x, int y)
{

	switch(button)
    {
		case 0:

        old_x = x - rot_x;
        old_y = y - rot_y;
        break;

		case 2:

        old_y = y - rot_z;
        move_z = (move_z ? 0 : 1);
	}


	glutPostRedisplay();

}

/*!
  This function is used when mouse cursor is moved from one point to the another. This function is use for zooming in and out using right click and drag gesture. Its arguments are are the x and y coordinates of the desired point
   the return type of this function is void
*/
void motion(int x, int y)
{

	if(!move_z)
    {
		rot_x = x - old_x;
		rot_y = y - old_y;
	}
	else
	{
		rot_z = y - old_y;
	}

	glutPostRedisplay();
}

/*!
 To register keyboard presses. We can toggle between window and full screen using 'w' and 'f' respectively.
 We can add amplitudes to the middle point of the scene with 'spacebar' key
 This function return type is void
*/
void keyboard(unsigned char key, int x, int y)
{
	static int old_x = 50;
	static int old_y = 50;
	static int old_width = 512;
	static int old_height = 512;

	switch (key)
	{
    case 27:
    bail(0);
    break;

    case 'w':
    glutPositionWindow(old_x, old_y);
    glutReshapeWindow(old_width, old_height);
    break;

    case 'f':
    if (glutGet(GLUT_WINDOW_WIDTH) < glutGet(GLUT_SCREEN_WIDTH))
    {
        old_x = glutGet(GLUT_WINDOW_X);
        old_y = glutGet(GLUT_WINDOW_Y);
        old_width = glutGet(GLUT_WINDOW_WIDTH);
        old_height = glutGet(GLUT_WINDOW_HEIGHT);
        glutFullScreen();
    }
    break;

    case ' ':
    amplitude[f][scene/2][scene/2] = -100;
    break;

	}
}


void init(void)
{
    int i, j;
    /*!
     This function is  used to specify diffuse light value
    */
    GLfloat mat_diffuse[] = { 0.0, 0.467, 0.845, 1.0 };
	/*!
	 This function is  used to specify specular light value
	*/
	GLfloat mat_specular[] = { 0.0, 0.467, 0.745, 1.0 };
	/*!
	This function is  used to specify shininess
	*/
	GLfloat mat_shininess[] = { 100.0 };
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
    glClearColor (0.0, 0.0, 0.0, 1.0);
	/*!
	 This function is  used to give the diffuse light value to the objects
	*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	/*!
	 This function is  used to give the specular light value to the objects
	*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	/*!
	 This function is  used to give the shininess to the objects
	*/
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    /*!
     This function is  used to enable lighting
	*/
	glEnable(GL_LIGHTING);
	/*!
	 type of light
	*/
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glShadeModel (GL_SMOOTH);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);



    for( i = 0; i < scene; i++)
    {
        for( j = 0; j < scene; j++)
        {
            amplitude[0][j][i] = 0;
            amplitude[1][j][i] = 0;
        }
    }
}

/*!
 This function is  used to reshape the window. It takes width and height as the arguments and sets the size of
 Window
 The return type of this function is void
*/
void reshape(int width, int height)
{
	w = width;
	h = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
}

void init();


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    /*!glutInitDisplayMode sets the display mode as GLUT_Double or GLUT_DEPTH orGLUT_RGB
	*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	/*!glutInitDisplayMode sets the Window Position
	*/
	glutInitWindowPosition(50, 50);
	/*!glutInitWindowSize sets the windows size according to the arguments given
	*/
	glutInitWindowSize(512, 512);
	/*! glutCreateWindow gives the title to the window which is given as string as arguments
    */
	glutCreateWindow("Ripple Creation ");
	/*! determines the normal plane for the windows that need to be displayed
    */
	glutDisplayFunc(display);
	/*!register callback when a window is maximised and minimised
    */
	glutReshapeFunc(reshape);
     /*!register callbacks for keyboard events that occur when you press a key
    */
	glutKeyboardFunc(keyboard);
	 /*!register callbacks for mouse events that occur when you press a mouse button
    */
	glutMouseFunc(mouse);
	 /*!register callbacks for mouse events that occur when we move the cursor
    */
	glutMotionFunc(motion);
    /*!
        The glEnable functions enable various OpenGL graphics capabilities
    */
	glEnable (GL_DEPTH_TEST);

	init();
    /*!
    glutIdleFunc sets the global idle callback to be func so a GLUT program can perform background processing tasks or continuous animation when window system events are not being received.
    */
	glutIdleFunc(idle);
	/*!glutMainLoop() is a function that loops within itself, processing events and triggering your callback functions when necessary
    */
	glutMainLoop();
	return 0;
}
