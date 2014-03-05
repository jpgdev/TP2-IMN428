/*
 IMN 428, Infographie 3D

 TP2: Introduction a la modelisation et a l'illumination locale.

 Nom1:
 Nom2:
 Nom3:
 Nom4:
*/


#define GLUT_DISABLE_ATEXIT_HACK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include "Glut/GLUT.h"
#endif

#include "structures.h"


/******************************\
 ***** Variables globales *****
\******************************/

MouseEvent	gLastMouseEvt;			/* Dernier evenement de la souris */
CamInfo		gCam;	                /* Parametres de la camera */

int 		objectResolution = 10;  /* Determine le nombre de polygones par objet */

Material	gMaterials[2];	        /* 0 = plancher, 1 = objet */

Light		gLights[2]; 			/* Les lumieres (LIGHT0 et LIGHT1) */
Light*		gCurrentLight = NULL;	/* Pointeur sur la lumiere selectionnee */

/* Variables globales contenant l'index des deux fenetres */
int windowModelerIndex;
int windowViewerIndex;

/* Variables globales concernant les points de la silhouette */
int nbPointsOnSilhouette;
point2D silhouettePointArray[NB_MAX_POINTS];
int indexSeclectedPoint=-1;

int CurrentAction = ActionRevolution;   /* None, Sweep, Revolution ou teapot */

bool drawNormals=false;

/**********************\
 ***** Prototypes *****
\**********************/
/* Pour dessiner les differents objets */
void drawPlane( int n );
void drawObject();

void initGL();
void initLighting();
void setLighting( const Light& light );

void initMaterials();
void setMaterial( const Material& mat );
void setCamera();

/* Callbacks */
void keyboard( unsigned char key, int x, int y );
void menuSelectionModeler(int value);
void menuSelectionViewer(int value);
void reshapeModeler(int width, int height);
void reshapeViewer(int width, int height);
void mouseButtonModeler( int button, int state, int x, int y );
void mouseButtonViewer( int button, int state, int x, int y );
void mouseMoveModeler(int x, int y);
void mouseMoveViewer(int x, int y);
void displayViewerWindow( void);
void displayModelerWindow( void);


/**************************\
 ********** Code **********
\**************************/


/*
	Converti les degres en radians
*/
double deg2rad( double deg )
{
	return	0.01745329251994329547437168059786927187815 * deg;
}

double rad2deg( double rad )
{
	return	57.2957795130823228646477218717336654663085 * rad;
}


/*
	Initialise des parametres OpenGL
*/
void initGL()
{
	/*
	** Initialisation
	*/

	initMaterials();
	initLighting();

	glFrontFace( GL_CCW );
	glEnable( GL_NORMALIZE );

	gCam.theta= 0.0;
	gCam.phi = 89.0;
	gCam.r = 500.0;
	gCam.fovy = 45.0;
	gCam.ratio = 1.0;
	gCam.znear = 1.0;
	gCam.zfar = 3000.0;
}

/*
	Cette fonction initialise les variables pour l'eclairage sous OpenGL
*/
void initLighting()
{
	glEnable( GL_LIGHTING );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

	/*Valeurs par defaut*/
	gLights[0].lightID = GL_LIGHT0;
	gLights[0].ambient[0] = 1.0; gLights[0].ambient[1] = 1.0; gLights[0].ambient[2] = 1.0; gLights[0].ambient[3] = 1.0;
	gLights[0].diffuse[0] = 1.0; gLights[0].diffuse[1] = 1.0; gLights[0].diffuse[2] = 1.0; gLights[0].diffuse[3] = 1.0;
	gLights[0].specular[0] = 1.0; gLights[0].specular[1] = 1.0; gLights[0].specular[2] = 1.0; gLights[0].specular[3] = 1.0;
	gLights[0].position[0] = 70; gLights[0].position[1] = -70; gLights[0].position[2] = 70; gLights[0].position[3] = 1.0;

	gLights[0].Kc = 0.0;
	gLights[0].Kl = 0.05;
	gLights[0].Kq = 0.0;
	gLights[0].on = true;

	gLights[1].lightID = GL_LIGHT1;
	gLights[1].ambient[0] = 0.0; gLights[1].ambient[1] = 0.0; gLights[1].ambient[2] = 1.0; gLights[1].ambient[3] = 1.0;
	gLights[1].diffuse[0] = 0.0; gLights[1].diffuse[1] = 0.0; gLights[1].diffuse[2] = 1.0; gLights[1].diffuse[3] = 1.0;
	gLights[1].specular[0] = 0.0; gLights[1].specular[1] = 0.0; gLights[1].specular[2] = 1.0; gLights[1].specular[3] = 1.0;
	gLights[1].position[0] = -10; gLights[1].position[1] = -90; gLights[1].position[2] = -10; gLights[1].position[3] = 1.0;

	gLights[1].Kc = 1.0;
	gLights[1].Kl = 0.0;
	gLights[1].Kq = 0.0;
	gLights[1].on = true;

}

/*
	 Cette fonction initialise les variables de materiel
*/
void initMaterials()
{
	gMaterials[0].ambient[0] = 0.33; gMaterials[0].ambient[1] = 0.22; gMaterials[0].ambient[2] = 0.03; gMaterials[0].ambient[3] = 1.0;
	gMaterials[0].diffuse[0] = 0.7; gMaterials[0].diffuse[1] = 0.5; gMaterials[0].diffuse[2] = 0.1; gMaterials[0].diffuse[3] = 1.0;
	gMaterials[0].specular[0] = 1.0; gMaterials[0].specular[1] = 1.0; gMaterials[0].specular[2] = 1.0; gMaterials[0].specular[3] = 1.0;
	gMaterials[0].shininess = 27.0;

	gMaterials[1].ambient[0] = 0.5; gMaterials[1].ambient[1] = 0.0; gMaterials[1].ambient[2] = 0.0; gMaterials[1].ambient[3] = 1.0;
	gMaterials[1].diffuse[0] = 1.0; gMaterials[1].diffuse[1] = 0.0; gMaterials[1].diffuse[2] = 0.2; gMaterials[1].diffuse[3] = 1.0;
	gMaterials[1].specular[0] = 0.5; gMaterials[1].specular[1] = 0.5; gMaterials[1].specular[2] = 0.5; gMaterials[1].specular[3] = 1.0;
	gMaterials[1].shininess = 32.0;
}

/*
	Fonction servant a afficher le plan
*/
void drawPlane( int n , bool displayNormals)
{

	/*
	** AJOUTER CODE ICI
	**
	** Cette fonction affiche un plan entre -100 et 100 sur les axes X et Z.
	** Le plan est positionne a Y = -100.
	** Le plan est constitue de n*n quads (polygones a 4 sommets)
	**
	** Utilisez gMaterials[0].diffuse comme couleur (glColor4fv)
	**
	*/

	//TODO: DISPLAY NORMALS

	glColor4fv(gMaterials[0].diffuse);

	int size = 200/n;

	glBegin(GL_QUADS);
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
			{
				//TOP LEFT
				glNormal3f(1,1,1);
				glVertex4f(-100 + (size * i),		-100,	100 - (size * j),		1);

				//TOP RIGHT
				glVertex4f(-100 + (size * (i+1)),	-100,	100 - (size * j),		1);

				//BOTTOM RIGHT
				glVertex4f(-100 + (size * (i+1)),	-100,	100 - (size * (j+1)),	1);

				//BOTTOM LEFT
				glVertex4f(-100 + (size * i),		-100,	100 - (size * (j+1)),	1);
			}
		}
	glEnd();
}


/*
	Fonction qui affiche silhouette sous la forme d'une
	'sweeping surface' dans le viewer 3D

*/
void drawSweepObject(int resolution, bool displayNormals)
{

	/*
	** AJOUTER CODE ICI
	**
	** Afficher la surface de translation de -200 a +200 le long de l'axe des Z
	** a l'aide des "nbPointsOnSilhouette" contenus dans le tableau "silhouettePointArray"
	**
	** Utilisez gMaterials[1].diffuse comme couleur (glColor4fv)
	**
	*/
}

/*
	Fonction qui affiche silhouette sous la forme d'une
	surface de revolution dans le viewer 3D
*/
void drawRevolutionObject(int resolution, bool displayNormals)
{
	/*
	** AJOUTER CODE ICI
	**
	** Afficher la surface de revolution a l'aide des "nbPointsOnSilhouette"
	** contenus dans le tableau "silhouettePointArray"
	**
	** utilisez gMaterials[1].diffuse comme couleur (glColor)
	**
	*/
}

/*
	Fonction appelee lorsqu'une entree du menu est selectionnee.
*/
void drawObject()
{
	glPushMatrix();

	/*
	** AJOUTER CODE ICI
	**
	** Ajouter le code afin d'afficher le teapot
	**
	** utilisez gMaterials[1].diffuse comme couleur (glColor)
	**
	*/

	

	switch( CurrentAction )
	{
		case ActionSweep:
			drawSweepObject(objectResolution, drawNormals);
			break;
		case ActionRevolution:
			drawRevolutionObject(objectResolution, drawNormals);
			break;
		case ActionTeapot:
			glFrontFace( GL_CW );
			glColor3fv(gMaterials[1].diffuse);
			glutSolidTeapot(20);
			glFrontFace( GL_CCW );
			break;

	}
	glPopMatrix();
}

/*
	Afficher les deux lumieres
*/
void drawLights()
{
	/*
	** AJOUTER CODE ICI
	**
	** Dessine chaque lumiere en utilisant sa composante diffuse
	** comme couleur.  Utilisez la commande "glutSolideSphere" pour afficher
	** les lumieres.
	**
	** Vous devez desactiver l'eclairage et le remettre dans son
	** etat inital ensuite
	*/
	glDisable(GL_LIGHTING);

	glColor3fv(gLights[0].diffuse);
	glPushMatrix();
		glTranslatef(gLights[0].position[0], gLights[0].position[1], gLights[0].position[2]);
		glutSolidSphere(25,50,50);
	glPopMatrix();


	glColor3fv(gLights[1].diffuse);
	glPushMatrix();
		glTranslatef(gLights[1].position[0], gLights[1].position[1], gLights[1].position[2]);
		glutSolidSphere(10,40,40);
	glPopMatrix();

	glEnable(GL_LIGHTING);

}

/*
	Fonction qui affiche les objets 3D : plan et/ou (sweep/revolution objet, teapot)
	Appelee chaque fois que la fenetre du viewer doit etre rafraiche.
*/
void displayViewerWindow()
{
	/*
	** AJOUTER CODE ICI
	**
	** Cette fonction est appelee a chaque fois que le contexte OpenGL est a rafraichir
	**
	** -Effacer l'ancien contenu (glClear)
	** -Activer la pile GL_PROJECTION et preciser les parametres de projection (gluPerspective + gCam)
	** -Activer la pile GL_MODELVIEW et positionner la camera (setCamera)
	** -Afficher les lumieres (setLighting + drawLights)
	** -Afficher le sol  (avec setMaterial + drawPlane())
	** -Afficher l'objet (avec setMaterial +drawObject())
	**
	*/

	/* Effacer l'ancien contenu */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* Configurer une camera (projection perspective) */
	glMatrixMode(GL_PROJECTION);
	gluPerspective(gCam.fovy, gCam.ratio, gCam.znear, gCam.zfar);

	/* Positionner la camera */
	glMatrixMode(GL_MODELVIEW);
	setCamera();

	/* Afficher les lumieres */
	setLighting(gLights[0]);
	setLighting(gLights[1]);
	drawLights();

	/* Afficher le plan avec le gMaterials[0] */
	setMaterial(gMaterials[0]);
	drawPlane(objectResolution, drawNormals);
	
	/* Afficher l'objet avec le gMaterials[1] */
	setMaterial(gMaterials[0]);
	drawObject();

	glutSwapBuffers();

}


/*
	Fonction qui affiche la silhouette dans la fenetre 'modeler'
	Appelee chaque fois que la fenetre du modeler doit etre rafraiche.
*/
void displayModelerWindow(void)
{
	int i;
	/* Configurer le "viewport" OpenGL. */
	glViewport(0, 0, 500, 500);
	glDisable(GL_LIGHTING);

	/* Configurer une camera (projection perspective) */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-500/2.0,500/2.0,-500/2.0,500/2.0);

	/* Selectionner la pile de transformations attachee au modele (MODELVIEW) */
	glMatrixMode(GL_MODELVIEW);

	/* Effacer l'ecran */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
		
	//Dessine les points verts
	glPointSize(6);
	glColor3f(0,1,0);
	glBegin(GL_POINTS);
		for(int i = 0; i < nbPointsOnSilhouette ; i++)
		{
			glVertex2f(silhouettePointArray[i].x, silhouettePointArray[i].y);
		}
	glEnd();

	//Dessine la courbe rouge
	glLineWidth(5);
	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
		
		for(int i = 0; i < nbPointsOnSilhouette ; i++)
		{
			glVertex2f(silhouettePointArray[i].x, silhouettePointArray[i].y);
		}
	glEnd();

	//Dessine la l'axe central blanc
	glLineWidth(5);
	glColor3f(1,1,1);
	glBegin(GL_LINES);		
		glVertex2f(0, 250);
		glVertex2f(0, -250);
	glEnd();


	glutSwapBuffers();
}


/*
	Specifie les parametres OpenGL de la lumiere courante
*/
void setLighting( const Light& light )
{
	/*
	** AJOUTER CODE ICI
	**
	** Cette fonction doit fixer les parametres de lumiere
	** sous OpenGL (glLightf[v])
	*/

	glLightfv(light.lightID, GL_AMBIENT, light.ambient);
	glLightfv(light.lightID, GL_DIFFUSE, light.diffuse);
	glLightfv(light.lightID, GL_SPECULAR, light.specular);
	glLightfv(light.lightID, GL_POSITION, light.position);
	glLightf(light.lightID, GL_CONSTANT_ATTENUATION, light.Kc);
	glLightf(light.lightID, GL_LINEAR_ATTENUATION, light.Kl);
	glLightf(light.lightID, GL_QUADRATIC_ATTENUATION, light.Kq);
	
	//Enable or disable the light using the "on" property
	if(light.on) 
		glEnable(light.lightID);
	else 
		glDisable(light.lightID);

}

/*
	Specifie les parametres OpenGL du materiaux courant
*/
void setMaterial( const Material& mat )
{
	/*
	** AJOUTER CODE ICI
	**
	** Cette fonction change les composantes du materiau courant
	** (glMaterialf[v])
	**
	*/

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
}

/*
	Fonction de rappel appelee lorsqu'une touche du clavier est enfoncee
*/
void keyboard( unsigned char key, int /* x */, int /* y */ )
{
	/*
	** Appelee lorsque l'on appuie sur une touche du clavier
	*/
	GLint polyMode[2];
	GLint params;
	switch( key )
	{
		/*
		 * Controle des lumieres
		 */
		case '1':
		case '2':
			gCurrentLight = &(gLights[key - '1']);
		break;

		case '3':
			gCurrentLight = NULL;
		break;

		case 8: /* back space*/
			nbPointsOnSilhouette--;
			if(nbPointsOnSilhouette<0)nbPointsOnSilhouette=0;
		break;
		/*
		 * Controle de la polygonisation des objets
		 */
		case 'A':
			++objectResolution;
			break;
		case 'a':
			--objectResolution;
			if( objectResolution <= 0 )
				objectResolution = 1;
			break;

		case 'b':
			gCam.ratio -= 0.05;
			if(gCam.ratio<0.05)
				gCam.ratio=0.05;
			printf("%f\n",gCam.ratio);
		break;

		case 'B':
			gCam.ratio += 0.05;
			if(gCam.ratio>5)
				gCam.ratio=5;
			printf("%f\n",gCam.ratio);
		break;

		case 'c':
			gCam.r -= 10;
			if(gCam.r<1)
				gCam.r=1.0;
		break;

		case 'C':
			gCam.r += 10;
			if(gCam.r>5000)
				gCam.r=5000.0;
		break;

		case 'e':
			gCam.fovy -= 1.0;
			if (gCam.fovy<1) gCam.fovy=1.0;
		break;

		case 'E':
			gCam.fovy += 1.0;
			if (gCam.fovy>170) gCam.fovy=170.0;
		break;


		case 'l': /* allumer/eteindre la lumiere courante */
			if( gCurrentLight )
				gCurrentLight->on = !gCurrentLight->on;
		break;

		case 'L': /* Active ou desactive le mode d'eclairage */
			/*
			** AJOUTER CODE ICI
			**
			** Activer/deactiver l'illumination
			**
			*/
			if(glIsEnabled(GL_LIGHTING)) 
				glDisable(GL_LIGHTING);
			else 
				glEnable(GL_LIGHTING);
		break;

		case 'n': /* afficher les normales */
		case 'N':
			drawNormals=!drawNormals;
		break;


		/*
		 * Pour quitter
		 */
		case 27:		/* ESC */
		case 'q':
			exit( 0 );
			break;

		case 's':			
			int valShade;
			glGetIntegerv(GL_SHADE_MODEL, &valShade);
			if(valShade == GL_SMOOTH)
				glShadeModel(GL_FLAT);
			else
				glShadeModel(GL_SMOOTH);
		break;


		case 'w':
			int valWF;
			glGetIntegerv(GL_POLYGON_MODE, &valWF);
			if(valWF == GL_FILL)
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			else 
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		break;

		//Facteurs speculaire
		case 'z':
			gMaterials[0].shininess -= 1.0;
			if(gMaterials[0].shininess<1.0)
				gMaterials[0].shininess =1.0;
			break;
		case 'Z':
			gMaterials[0].shininess += 1.0;
			if(gMaterials[0].shininess>128.0)
				gMaterials[0].shininess =128.0;
			break;
		case 'x':
			gMaterials[1].shininess -= 1.0;
			if(gMaterials[1].shininess<1.0)
				gMaterials[1].shininess =1.0;
			break;
		case 'X':
			gMaterials[1].shininess += 1.0;
			if(gMaterials[1].shininess>128.0)
				gMaterials[1].shininess =128.0;
			break;


		default:
			printf( "undefined key [%d]\n", (int)key );
			break;
	}

	glutPostWindowRedisplay(windowViewerIndex);
	glutPostWindowRedisplay(windowModelerIndex);
}

/*
	Positionne et oriente la camera en fonction
	du contenu de la variable globale gCam.
	Note : la camera regarde toujours l'origine (0,0,0)
*/
void setCamera()
{


	float z = gCam.r*cos(deg2rad(gCam.theta))*cos(deg2rad(gCam.phi));
	float x = gCam.r*sin(deg2rad(gCam.theta))*cos(deg2rad(gCam.phi));
	float y = gCam.r*sin(deg2rad(gCam.phi));

	gluLookAt(x, y, z, /* Position de la camera */
		0.0, 0.0, 0.0, /* Le point que la camera regarde*/
		0.0, 1.0, 0.0);/* le 'up-vector' */

}

/*
	Fonction de rappel:  fonction appelee lorsqu'une entree du menu du modeler
	a ete selectionnee.
*/
void menuSelectionModeler(int value)
{
	if( value == Reset )
	{
		nbPointsOnSilhouette=0;
		glutPostRedisplay();
	}
	else if ( value == Quit )
		exit(0);
	else
		CurrentAction = value;

	glutPostWindowRedisplay(windowViewerIndex);
}


/*
	Fonction de rappel: un bouton de la souris a ete
	enfonce dans la fenetre 'Modeler'
*/
void mouseButtonModeler( int button, int state, int x, int y )
{
	int i;
	float dist;

	gLastMouseEvt.button = button;
	gLastMouseEvt.stateButton = state;

	if(button == GLUT_MIDDLE_BUTTON)
	{
		indexSeclectedPoint=-1;
		if(state == GLUT_DOWN)
		{
			for(i=0;i<nbPointsOnSilhouette;i++)
			{
				dist=sqrt(pow(x-silhouettePointArray[i].x-500/2.0,2.0)+pow(-y-silhouettePointArray[i].y+500/2.0,2.0));
				if(dist<15.0)
				{
					indexSeclectedPoint=i;
					i=nbPointsOnSilhouette;
				}
			}
		}
		gLastMouseEvt.x = x;
		gLastMouseEvt.y = y;

	}
	else if(button == GLUT_LEFT_BUTTON)
	{
		/* Enregistrer le point de controle (x,y) */
		if(nbPointsOnSilhouette<NB_MAX_POINTS && state==GLUT_DOWN)
		{
			silhouettePointArray[nbPointsOnSilhouette].x=(int)(x-500/2.0);
			silhouettePointArray[nbPointsOnSilhouette].y=(int)(500-y-500/2.0);
			nbPointsOnSilhouette++;
		}
	}


	glutPostRedisplay();
	glutPostWindowRedisplay(windowViewerIndex);
}

/*
   Fonction de rappel: La souris vient d'etre bougee dans le fenetre 'modeler' avec
	un de ses boutons enfonce. La position de la souris est  (x,y)
*/
void mouseMoveModeler(int x, int y)
{
	/* Bouton gauche : rotation,
	   Bouton droit : translation */
	if( gLastMouseEvt.button == GLUT_MIDDLE_BUTTON && indexSeclectedPoint>=0 )
	{
		silhouettePointArray[indexSeclectedPoint].x += x-gLastMouseEvt.x;
		silhouettePointArray[indexSeclectedPoint].y -= y-gLastMouseEvt.y;
	}

	gLastMouseEvt.y=y;
	gLastMouseEvt.x=x;
	glutPostRedisplay();
	glutPostWindowRedisplay(windowViewerIndex);
}


/*
	Fonction de rappel: Fonction appelee lorsqu'on clique dans la fenetre viewer
*/
void mouseButtonViewer( int button, int state, int x, int y )
{
	/*
	** Appele lorsque l'on appuie ou relache un bouton de la souris
	**
	** button = GLUT_LEFT_BUTTON ou GLUT_MIDDLE_BUTTON ou GLUT_RIGHT_BUTTON
	** state  = GLUT_DOWN ou GLUT_UP
	*/

	if( state == GLUT_DOWN )
	{
		gLastMouseEvt.button = button;
		gLastMouseEvt.x = x;
		gLastMouseEvt.y = y;
	}
	else
	if( state == GLUT_UP )
	{
		gLastMouseEvt.button = -1;
		gLastMouseEvt.x = -1;
		gLastMouseEvt.y = -1;
	}

}

/*
	Fonction de rappel: Fonction appelee lorsqu'on bouge la souris dans la fenetre viewer
*/
void mouseMoveViewer( int x, int y )
{
	/*
	** Appelee lorsque la souris se deplace avec un de ses boutons enfonces
	** (gLastMouseEvt contient l'ancien evenement)
	*/
	int	dx = x - gLastMouseEvt.x;
	int	dy = -y + gLastMouseEvt.y;
	float m[16];
	gLastMouseEvt.x = x;
	gLastMouseEvt.y = y;

	switch( gLastMouseEvt.button )
	{
		case GLUT_LEFT_BUTTON:
		{
			/* Rotation */
			gCam.theta -= dx;
			gCam.phi   -= dy;
			if( gCam.phi >  89 ) gCam.phi =  89;
			if( gCam.phi < -89 ) gCam.phi = -89;
			break;
		}
		case GLUT_MIDDLE_BUTTON:
		{
			/* Deplacer la lumiere courante */
			if( gCurrentLight )
			{
					glMatrixMode( GL_MODELVIEW );

					glPushMatrix();
					glLoadIdentity();
					setCamera();
					glGetFloatv(GL_MODELVIEW_MATRIX,m);
					glPopMatrix();

					gCurrentLight->position[0] += (m[0]*dx + m[1]*dy + m[2]*0 + m[3]*1) *gCam.r/900;
					gCurrentLight->position[1] += (m[4]*dx + m[5]*dy + m[6]*0 + m[7]*1) *gCam.r/900;
					gCurrentLight->position[2] += (m[8]*dx + m[9]*dy + m[10]*0 + m[11]*1) *gCam.r/900;
				}
			break;
		}
		case GLUT_RIGHT_BUTTON:
		{
			/* Zoom in/out */
			gCam.r += dx - dy;
			if( gCam.r < 1 )	gCam.r = 1;
			break;
		}
		default:
			return;
	}


	glutPostRedisplay();
}



int main( int argc, char** argv )
{
	int MenuIdModeler;
	int MenuIdViewer;

	glutInit(&argc, argv);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize( 500, 500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	windowModelerIndex=glutCreateWindow("Modeler");

	/* Configuration de la couleur de fond (noir) */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	/* Creation du menu du modeler */
	MenuIdModeler = glutCreateMenu(menuSelectionModeler);
	glutSetMenu( MenuIdModeler );
	glutAddMenuEntry( "None", ActionNone );
	glutAddMenuEntry( "Sweep", ActionSweep );
	glutAddMenuEntry( "Revolution", ActionRevolution );
	glutAddMenuEntry( "Teapot", ActionTeapot );
	glutAddMenuEntry( "Reset", Reset );
	glutAddMenuEntry( "Quitter", Quit );
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Fonctions de rappel (callback) de glut pour la fenetre 'modeler' */
	glutMouseFunc(mouseButtonModeler);      /* Fonctions appelee lorsqu'un bouton de la souris est enfonce */
	glutMotionFunc(mouseMoveModeler);       /* Fonctions appelee lorsque la souris bouge et qu'un de ses boutons est enfonce */
	glutDisplayFunc(displayModelerWindow);  /* Fonctions appelee lorsque la scene doit etre affichee. */
	glutKeyboardFunc(keyboard);


	/* Double buffer, couleur, avec Z-Buffer */
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	/* On initialise la fenetre d'une taille raisonnable */
	glutInitWindowPosition(510, 0);
	glutInitWindowSize( 500, 500 );


	/* On cree veritablement la fenetre */
	windowViewerIndex=glutCreateWindow( "IMN428 -- TP2" );
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);

	/* On enregistre les fonctions CALLBACK */
	glutDisplayFunc( displayViewerWindow );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouseButtonViewer );
	glutMotionFunc( mouseMoveViewer );

	initGL();

	/* On part le programme jusqu'a ce qu'on quitte */
	glutMainLoop();

	return 0;
}
