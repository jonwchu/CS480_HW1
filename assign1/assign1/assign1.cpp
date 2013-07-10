// assign1.cpp : Defines the entry point for the console application.
//

/*
  Name: Jonathan Chu
  CSCI 480 Computer Graphics
  Assignment 1: Height Fields
  C++ starter code
*/

#include "stdafx.h"
#include <pic.h>
#include <windows.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>

int g_iMenuId;

int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;


CONTROLSTATE g_ControlState = ROTATE;


//Used for different types of coloring
typedef enum { SINGLE_COLOR, FILL, WIREFRAME, DOTS } SHADESTATE;

SHADESTATE g_ShadeState = SINGLE_COLOR;

/* state of the world */
float g_vLandRotate[3] = {0.0, 0.0, 0.0};
float g_vLandTranslate[3] = {0.0, 0.0, 0.0};
float g_vLandScale[3] = {1.0, 1.0, 1.0};

float heightScale = 75.0f;

/* see <your pic directory>/pic.h for type Pic */
Pic * g_pHeightData;

//Magic Number... Can only acomodate pictures that are 256x256 pixels
const int dimension = 257;

//Magic numbers used because of this
float vertexGrid[dimension][dimension];

void loadVertexBuffer(int, int);
void colorMesh(int, int);

/* Write a screenshot to the specified filename */
void saveScreenshot (char *filename)
{
  int i, j;
  Pic *in = NULL;

  if (filename == NULL)
    return;

  /* Allocate a picture buffer */
  in = pic_alloc(640, 480, 3, NULL);

  printf("File to save to: %s\n", filename);

  for (i=479; i>=0; i--) {
    glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE,
                 &in->pix[i*in->nx*in->bpp]);
  }

  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);
}

void myinit()
{
  /* setup gl view here */
  glClearColor(0.7, 0.77, 1.0, 0.0); //Set background to something not black
  glShadeModel(GL_SMOOTH);
}

void display()
{
  /* draw 1x1 cube about origin */
  /* replace this code with your height field implementation */
  /* you may also want to precede it with your 
rotation/translation/scaling */
	

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//Initial rotation for prettier view
	glRotatef(-45,1,0,0);	

	//Translate back to see it in view
	glTranslatef(0,225,-250);

	glScalef(g_vLandScale[0],g_vLandScale[1],g_vLandScale[2]);

	glRotatef(g_vLandRotate[0],1.0,0.0,0.0);
	glRotatef(g_vLandRotate[1],0.0,1.0,0.0);
	glRotatef(g_vLandRotate[2],0.0,0.0,1.0);

	glTranslatef(g_vLandTranslate[0],g_vLandTranslate[1],g_vLandTranslate[2]);
	

	int width = g_pHeightData->nx;
	int height = g_pHeightData->ny; //Assume both are the same

	//Creates the vertex buffer to create the mesh
	loadVertexBuffer(width, height);

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
	//Depending on the state, will change how vertices are drawn
		if(g_ShadeState == DOTS)
		{
			glBegin(GL_POINTS);
		}
		else
		{
			if(g_ShadeState == WIREFRAME)
			{
				glBegin(GL_LINES);
			}
			else if((g_ShadeState == FILL) || (g_ShadeState == SINGLE_COLOR))
			{
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				glBegin(GL_TRIANGLES);
			}
		}

	for(int z = 0; z < height-1; z++)
	{
		for(int x = 0; x < width-1; x++)
		{
				if(g_ShadeState == DOTS)
				{
					glColor3f(1.0,1.0,0.0);

					//Draw Points of Pixel
					glVertex3f((float)(x-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x][z] * heightScale));
					glVertex3f((float)(x+1-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x+1][z] * heightScale));
					glVertex3f((float)(x-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x][z+1] * heightScale));
					glVertex3f((float)(x+1-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x+1][z+1] * heightScale));

				}
				else
				{
					if(g_ShadeState == WIREFRAME)
					{
						glColor3f(0.0,0.0,1.0);

						//Draw Line 1 1-2
						glVertex3f((float)(x-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x][z] * heightScale));
						glVertex3f((float)(x+1-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x+1][z] * heightScale));

						//Draw Line 2 2-3
						glVertex3f((float)(x+1-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x+1][z] * heightScale));
						glVertex3f((float)(x-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x][z+1] * heightScale));

						//Draw Line 3 3-1
						glVertex3f((float)(x-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x][z+1] * heightScale));
						glVertex3f((float)(x-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x][z] * heightScale));

						//Draw Line 4 2-4
						glVertex3f((float)(x+1-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x+1][z] * heightScale));
						glVertex3f((float)(x+1-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x+1][z+1] * heightScale));

						//Draw Line 5 4-3
						glVertex3f((float)(x+1-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x+1][z+1] * heightScale));
						glVertex3f((float)(x-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x][z+1] * heightScale));						
					}
					else if((g_ShadeState == FILL) || (g_ShadeState == SINGLE_COLOR))
					{
						//Draw Triangle 1 of Pixel
						colorMesh(x,z);
						glVertex3f((float)(x-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x][z] * heightScale));

						colorMesh(x+1,z);
						glVertex3f((float)(x+1-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x+1][z] * heightScale));

						colorMesh(x,z+1);
						glVertex3f((float)(x-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x][z+1] * heightScale));

						//Draw Triangle 2 of Pixel

						colorMesh(x+1,z);
						glVertex3f((float)(x+1-(width/2)),(float)(z-(height/2)),(float)(vertexGrid[x+1][z] * heightScale));

						colorMesh(x+1,z+1);
						glVertex3f((float)(x+1-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x+1][z+1] * heightScale));

						colorMesh(x,z+1);
						glVertex3f((float)(x-(width/2)),(float)(z+1-(height/2)),(float)(vertexGrid[x][z+1] * heightScale));
					}
				}
			
		}
	}
	glEnd();

	glutSwapBuffers();
}

void colorMesh(int x, int z)
{
	//Colro the vertices depending on state and height in terms of z
	if(g_ShadeState == SINGLE_COLOR)
	{
		glColor3f(1.0 * vertexGrid[x][z], 0.0, 0.0);
	}
	else
	{
		if(vertexGrid[x][z] < 0.20f)
		{
			glColor3f(0.0, 0.0, 1.0 * vertexGrid[x][z]);
		}
		else
		{
			if(vertexGrid[x][z] < 0.40f)
			{
				glColor3f(0.0, 1.0 * vertexGrid[x][z], 0.0);
			}
			else
			{
				if(vertexGrid[x][z] < 0.60f)
				{
					glColor3f(1.0 * vertexGrid[x][z], 1.0 * vertexGrid[x][z], 0.0);
				}
				else
				{
					if(vertexGrid[x][z] < 0.80f)
					{
						glColor3f(1.0 * vertexGrid[x][z], 0.5 * vertexGrid[x][z], 0.0);
					}
					else
					{
						glColor3f(1.0 * vertexGrid[x][z], 0.0, 0.0);
					}
				}
			}
		}
	}
}

void loadVertexBuffer(int width, int height)
{
	
	//Loop through image to determine averages for vertices
	for(int i = 0; i < dimension; i++)
	{
		for(int j = 0; j < dimension; j++)
		{
			int partA = 0;
			int partB = 0;
			int partC = 0;
			int partD = 0;

			float count = 1.0f;

			if((i > 0) && (i < (dimension - 1)) && (j > 0) && (j < (dimension - 1))) //inside values Case 1
			{
				/*
				partA = g_pHeightData->pix[((i - 1) * width) + (j - 1)];
				partB = g_pHeightData->pix[((i - 1) * width) + (j)];
				partC = g_pHeightData->pix[(i * width) + (j - 1)];
				partD = g_pHeightData->pix[(i * width) + (j)];
				*/

				partA = PIC_PIXEL(g_pHeightData,j-1,i-1,0);
				partB = PIC_PIXEL(g_pHeightData,j,i-1,0);
				partC = PIC_PIXEL(g_pHeightData,j-1,i,0);
				partD = PIC_PIXEL(g_pHeightData,j,i,0);

				count = 4.0f;
			}
			else
			{
				if((i == 0) && (j > 0) && (j < (dimension - 1))) //Top Middle values Case 2
				{
					/*
					partA = g_pHeightData->pix[(i * width) + (j - 1)]; //i technically evaluates to 0 
					partB = g_pHeightData->pix[(i * width) + (j)];
					*/
					partA = PIC_PIXEL(g_pHeightData,j-1,i,0);
					partB = PIC_PIXEL(g_pHeightData,j,i,0);

					count = 2.0f;
				}
				else
				{
					if((i == (dimension - 1)) && (j > 0) && (j < (dimension - 1))) //Bottom Middle values Case 3
					{
						/*
						partA = g_pHeightData->pix[((i - 1) * width) + (j - 1)]; //i technically evaluates to 0 
						partB = g_pHeightData->pix[((i - 1) * width) + (j)];
						*/
						partA = PIC_PIXEL(g_pHeightData,j-1,i-1,0);
						partB = PIC_PIXEL(g_pHeightData,j,i-1,0);

						count = 2.0f;
					}
					else
					{
						if((i > 0) && (i < (dimension - 1)) && (j == 0)) //Left Middle values Case 4
						{
							/*
							partA = g_pHeightData->pix[((i - 1) * width) + (j)]; //j evaluates to 0
							partB = g_pHeightData->pix[(i * width) + (j)];
							*/
							partA = PIC_PIXEL(g_pHeightData,j,i-1,0);
							partB = PIC_PIXEL(g_pHeightData,j,i,0);

							count = 2.0f;
						}
						else
						{
							if((i > 0) && (i < (dimension - 1)) && (j == (dimension - 1))) //Right Middle values Case 5
							{
								/*
								partA = g_pHeightData->pix[((i - 1) * width) + (j - 1)]; //j evaluates to 0
								partB = g_pHeightData->pix[(i * width) + (j - 1)];
								*/

								partA = PIC_PIXEL(g_pHeightData,j-1,i-1,0);
								partB = PIC_PIXEL(g_pHeightData,j-1,i,0);

								count = 2.0f;
							}
							else
							{
								if((i == 0) && (j == 0)) //Top Left value Case 6
								{
									/*
									partA = g_pHeightData->pix[(i * width) + j];
									*/

									partA = PIC_PIXEL(g_pHeightData,j,i,0);

									count = 1.0f;
								}
								else
								{
									if((i == 0) && (j == (dimension - 1))) //Top Right value Case 7
									{
										/*
										partA = g_pHeightData->pix[(i * width) + (j - 1)];
										*/

										partA = PIC_PIXEL(g_pHeightData,j-1,i,0);

										count = 1.0f;
									}
									else
									{
										if((i == (dimension - 1)) && (j == 0)) //Bottom Left value Case 8
										{
											/*
											partA = g_pHeightData->pix[((i - 1) * width) + j];
											*/

											partA = PIC_PIXEL(g_pHeightData,j,i-1,0);

											count = 1.0f;
										}
										else
										{
											if((i == (dimension - 1)) && (j == (dimension - 1))) //Bottom Right value Case 9
											{
												/*
												partA = g_pHeightData->pix[((i - 1) * width) + (j - 1)];
												*/

												partA = PIC_PIXEL(g_pHeightData,j-1,i-1,0);

												count = 1.0f;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			vertexGrid[i][j] = ((partA + partB + partC + partD) / (count * 255.0f));
		}
	}
}

void menufunc(int value)
{
  switch (value)
  {
    case 0:
      exit(0);
      break;
  }
}

void doIdle()
{
  /* do some stuff... */

  /* make the screen update */
  glutPostRedisplay();
}

/* converts mouse drags into information about 
rotation/translation/scaling */
void mousedrag(int x, int y)
{
  int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
  
  switch (g_ControlState)
  {
    case TRANSLATE:  
      if (g_iLeftMouseButton)
      {
        g_vLandTranslate[0] += vMouseDelta[0]*0.01;
        g_vLandTranslate[1] -= vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandTranslate[2] += vMouseDelta[1]*0.01;
      }
      break;
    case ROTATE:
      if (g_iLeftMouseButton)
      {
        g_vLandRotate[0] += vMouseDelta[1];
        g_vLandRotate[1] += vMouseDelta[0];
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandRotate[2] += vMouseDelta[1];
      }
      break;
    case SCALE:
      if (g_iLeftMouseButton)
      {
        g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
        g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
      }
      break;
  }
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mouseidle(int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mousebutton(int button, int state, int x, int y)
{

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      g_iLeftMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
      break;
  }
 
  switch(glutGetModifiers())
  {
    case GLUT_ACTIVE_CTRL:
      g_ControlState = TRANSLATE;
      break;
    case GLUT_ACTIVE_SHIFT:
      g_ControlState = SCALE;
      break;
    default:
      g_ControlState = ROTATE;
      break;
  }

  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void keyboard(unsigned char key, int x, int y)
{
	//FGets keys to change state
	if(key == 'h' || key == 'H')
	{
		g_ShadeState = SINGLE_COLOR;
	}
	if(key == 'j' || key == 'J')
	{
		g_ShadeState = FILL;
	}
	if(key == 'k' || key == 'K')
	{
		g_ShadeState = WIREFRAME;
	}
	if(key == 'l' || key == 'L')
	{
		g_ShadeState = DOTS;
	}
	//Changes height multiplier to change how much the height is affected
	if(key == 'o' || key == 'O')
	{
		if(heightScale < 150.0f)
		{
			heightScale += 5.0f;
		}
	}
	if(key == 'p' || key == 'P')
	{
		if(heightScale > 0.0f)
		{
			heightScale -= 5.0f;
		}
	}
}

/* reshape callback (From Powerpoint) */
void myReshape(int w, int h)
{
    GLfloat aspect = (GLfloat) w / (GLfloat) h;
    /* scale viewport with window */
    glViewport(0, 0, w, h);
    /* prevent distortion */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //if (w <= h) /* aspect <= 1 */
    //glOrtho(-150.0, 150.0, -150.0, 150.0, -1000.0, 10000.0);
    //else /* aspect > 1 */
    //glOrtho(-150.0 * aspect, 150.0 * aspect, -175.0, 275.0, -1000.0, 1000.0);
    gluPerspective(55.0, aspect, 0.01, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int _tmain(int argc, _TCHAR* argv[])
{
	// I've set the argv[1] to spiral.jpg.
	// To change it, on the "Solution Explorer",
	// right click "assign1", choose "Properties",
	// go to "Configuration Properties", click "Debugging",
	// then type your texture name for the "Command Arguments"
	if (argc<2)
	{  
		printf ("usage: %s heightfield.jpg\n", argv[0]);
		exit(1);
	}

	g_pHeightData = jpeg_read((char*)argv[1], NULL);
	if (!g_pHeightData)
	{
	    printf ("error reading %s.\n", argv[1]);
	    exit(1);
	}

	glutInit(&argc,(char**)argv);
  
	/*
		create a window here..should be double buffered and use depth testing
		
	    the code past here will segfault if you don't have a window set up....
	    replace the exit once you add those calls.
	*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(640, 480);
	//May need to set window position
	glutCreateWindow("JChu Height Field");

	glutReshapeFunc(myReshape);

	//exit(0);

	/* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);
  
	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
  
	/* replace with any animate code */
	glutIdleFunc(doIdle);

	/* callback for mouse drags */
	glutMotionFunc(mousedrag);
	/* callback for idle mouse movement */
	glutPassiveMotionFunc(mouseidle);
	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);
	/* callback for keyboard changes */
	glutKeyboardFunc(keyboard);

	/* do initialization */
	myinit();

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}