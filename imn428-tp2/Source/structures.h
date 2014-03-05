#ifndef STRUCTURE_H__
#define STRUCTURE_H__

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include "Glut/GLUT.h"
#endif

#define NB_MAX_POINTS 1000


/**********************\
 *******  ENUM  *******
\**********************/

enum
{
    ActionNone,
    ActionSweep,
    ActionRevolution,
    ActionTeapot,
    Reset,
    Quit
};


/**********************\
 ***** Structures *****
\**********************/

typedef struct
{
	int	button;  /* left, middle, right */
	int stateButton; /* pressed or not */
	int	x;
	int	y;
} MouseEvent;

typedef struct
{
    int x;
    int y;
} point2D;

typedef struct
{
	GLfloat	theta;	/* angle de rotation sur le plane X/Z */
	GLfloat	phi;	/* angle d'elevation du plan X/Z */
	GLfloat	r;		/* distance d'eloignement */
	GLfloat fovy;   /* angle d'ouverture */
	GLfloat ratio;  /* ration hauteur/largeur de la fenetre courante */
	GLfloat znear;   /* near clipping plane */
	GLfloat zfar;    /* far clipping plane */
} CamInfo;

typedef struct
{
	GLfloat	ambient[4]; 	//Couleur ambiante
	GLfloat	diffuse[4];		//Couleur diffuse
	GLfloat	specular[4];	//Couleur speculaire
	GLfloat shininess;		//L'exposant 'n' du cos
} Material;

typedef struct
{

	GLenum	lightID;		//GL_LIGHT0, GL_LIGHT1, etc.
	GLfloat	ambient[4]; 	//Couleur pour la composante ambiante
	GLfloat	diffuse[4];		//Couleur pour la composante diffuse
	GLfloat	specular[4];	//Couleur pour la composante speculaire
	GLfloat	position[4];	//Position

	GLfloat Kc;	//Facteur d'attenuation constante
	GLfloat Kl;	//Facteur d'attenuation linaire
	GLfloat Kq;	//Facteur d'attenuation quadratique

	bool	on; // allumée ou fermée

}Light;


#endif
