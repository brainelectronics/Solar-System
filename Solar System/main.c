//
//  main.c
//  Solar System
//
//  Created by Jonas Scharpf on 12.06.14.
//  Copyright (c) 2014 Jonas Scharpf. All rights reserved.
//
/*
 Sonnensystem animiert
 Erde dreht sich in 365 Tagen um die Sonne
 Mond dreht sich in 28 Tagen um die Erde
 
 Kurztasten
 a      Koordinatenachsen anzeigen
 v      Beschriftungen anzeigen
 m      Perspective wechseln
 r      automatische Rotation
 s      Erde drehen um 1 Tag
 S      Erde entgegengesetzt drehen
 g		Longituden Einheiten erhöhen
 G		Longituden Einheiten verringern
 h		Latituden Einheiten erhöhen
 H		Latituden Einheiten verringern
 +      fov erhöhen
 -      fov verringern
 Rechts Gegen den Uhrzeiger auf Y alles drehen
 Links  Mit dem Uhrzeiger Y alles drehen
 Runter Same
 Hoch   Same
 ESC    Programm beenden
 */



#include "includes.h"

//  Math definitions
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
#define PI 3.1415926535898

//  Globals
float dim=3.0; // dimension of orthogonal box   //double
char *windowName = "Sonnensystem V2";
int windowWidth=450;    //fenster breite
int windowHeight=450;   //fenster höhe

//  Toggles
int toggleAxes = 0;   // toggle axes on and off
int toggleValues = 1; // toggle values on and off
int toggleMode = 1; //  projection mode
int toggleRotation = 0;

//  Display view
int th = -30;  // azimuth of view angle
int ph = 45;  // elevation of view angle
int fov = 55;   //field of view for perspective
int asp = 1;    //aspect ratio

//  Objects
int objSlices = 10; // slices lines of longitude
int objStacks = 10; // stacks lines of latitude
GLdouble size = 1;  //size of teapod

// Spheres
int earthAroundSun = 0;
int moonAroundEarth = 0;

void project()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (toggleMode)
    {
        gluPerspective(fov,asp,dim/4,4*dim);    //perspective
    }
    else
    {
        glOrtho(-dim*asp,+dim*asp, -dim,+dim, -dim,+dim);   //orthogonal projection
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//Koordinaten Achsen
void drawAxes()
{
    if (toggleAxes) //Achsen mit 'a' schalten
    {
        double len = 2.0;   //Length of axes
        
        glColor3f(1.0,1.0,1.0); //Farbe, blau
        glBegin(GL_LINES);
        glVertex3d(0,0,0);
        glVertex3d(len,0,0);
        glVertex3d(0,0,0);
        glVertex3d(0,len,0);
        glVertex3d(0,0,0);
        glVertex3d(0,0,len);
        glEnd();
        
        //  Label axes
        glColor3f(1.0,1.0,1.0); //Farbe, weiß
        glRasterPos3d(len,0.0,0);
        print("X");
        glRasterPos3d(0,len,0);
        print("Y");
        glRasterPos3d(0,0,len);
        print("Z");
    }
}

//Beschriftungen
void drawValues()
{
    if (toggleValues)   //Beschriftungen mit 'v' schalten
    {
        glColor3f(1.0,1.0,1.0); //Farbe, weiß
        printAt(5,5,"View Angle (th, ph, dim, fov) =(%d, %d, %1.1f, %d)", th,ph,dim,fov);
        printAt(5, 25, "Day, Moon Day: %d, %d", earthAroundSun, (moonAroundEarth/13));
        printAt(5, 45, "Lines (lon, lat) =(%d, %d)", objSlices, objStacks);
        printAt(5,65,"Projection mode =(%s)", toggleMode?"Perspective":"Orthogonal");
    }
}

void drawSystem()
{
    glTranslated(0,0,0);
    glRotated(0,0,0,0);
    glScaled(1,1,1);
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);   //yellow
    glutSolidSphere(1.0, objSlices, objStacks);   //Sun, radius, slices, stacks
    glRotatef ((GLfloat) earthAroundSun, 0.0, 1.0, 0.0);  //winkel, x, y, z
    
    glColor3f(0.0, 0.0, 1.0);   //blue
    glTranslatef (2.5, 0.0, 0.0);   //x, y, z   //2
    //glRotatef(23.44, 1.0, 0.0, 0.0);  //Neigung um Rotationsachse
    glRotatef ((GLfloat) moonAroundEarth, 0.0, 1.0, 0.0);   //winkel, x, y, z
    glutSolidSphere(0.1, objSlices, objStacks);    //earth, radius, slices, stacks
    
    glColor3f(1.0, 1.0, 1.0);   //white
    glTranslatef(0.128, 0.0, 0.0);    //Moon, radius, slices, stacks //0.2
    //glRotatef((GLfloat) moonDay, 0.0, 1.0, 0.0);    //winkel, x, y, z
    glutSolidSphere(0.025, objSlices, objStacks);
    glPopMatrix();
    
}

void setEye()
{
    if (toggleMode)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        // camera/eye position, aim of camera lens, up-vector
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
    //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //  Clear the image
    glEnable(GL_DEPTH_TEST);
    
    glLoadIdentity();   //  Reset previous transforms
    setEye();
    
    //Draw
    drawAxes();
    drawValues();
    drawSystem();   //zeichnet das Sonnensystem
    
    //Flush and swap
    glFlush();
    glutSwapBuffers();
}

void spinDisplay()
{
    if (toggleRotation)
    {
        earthAroundSun = earthAroundSun + 1.0;
        if (earthAroundSun > 360.0)
        {
            earthAroundSun = 0.0;
        }
        
        moonAroundEarth = moonAroundEarth + 13.0;
        if (moonAroundEarth > 360.0)
        {
            moonAroundEarth = 0.0;
        }
        /*
         earthAroundSun = (earthAroundSun+1)%360;
         moonAroundEarth = (moonAroundEarth+13)%360;
         */
        glutPostRedisplay();
    }
}


void reshape(int width,int height)
{
    asp = (height>0) ? (double)width/height : 1;    //double w2h
    glViewport(0,0, width,height);
    project();
}


void windowKey(unsigned char key,int x,int y)
{
    if (key == 27) exit(0); //Exit on ESC
    else if (key == 'a' || key == 'A') toggleAxes = 1-toggleAxes;
    else if (key == 'v' || key == 'V') toggleValues = 1-toggleValues;
    else if (key == 'm' || key == 'M') toggleMode = 1-toggleMode;
    else if (key == 'r' || key == 'R') toggleRotation = 1-toggleRotation;
    else if (key == 'g') objSlices += 1;    //longitude Einheiten erhöhen
    else if (key == 'G')
    {
        objSlices -= 1;    //longitude Einheiten verringern
        if (objSlices <= 10) //weniger uninteressant
        {
            objSlices = 10;
        }
    }
    
    else if (key == 'h') objStacks += 1;    //latitude Einheiten erhöhen
    else if (key == 'H')
    {
        objStacks -= 1;    //latitude Einheiten verringern
        if (objStacks <= 10) //weniger uninteressant
        {
            objStacks = 10;
        }
    }
    else if (key == 's')
    {
        earthAroundSun = (earthAroundSun+1)%360;    //ca. 1.01°
        moonAroundEarth = (moonAroundEarth+13)%360; //ca. 12,85°
    }
    else if (key == 'S')
    {
        earthAroundSun = (earthAroundSun-1)%360;    //ca. 1.01°
        moonAroundEarth = (moonAroundEarth-13)%360; //ca. 12,85°
    }
    else if (key == '-' && key>1) fov--;    // Change field of view angle
    else if (key == '+' && key<179) fov++;
    
    else if (key == 'D') dim += 0.1;    // Change field of view angle
    else if (key == 'd' && dim>1) dim -= 0.1;
    
    project();
    glutPostRedisplay();
}


void windowSpecial(int key,int x,int y)
{
    //  Right arrow key - increase azimuth by 5 degrees
    if (key == GLUT_KEY_RIGHT) th += 5;
    //  Left arrow key - decrease azimuth by 5 degrees
    else if (key == GLUT_KEY_LEFT) th -= 5;
    
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP) ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN) ph -= 5;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    
    project();
    
    glutPostRedisplay();
}


void windowMenu(int value)
{
    windowKey((unsigned char)value, 0, 0);
}


int main(int argc,char* argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DOUBLE);
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(1000, 0);
    glutCreateWindow(windowName);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutMouseFunc(mouse);
    glutKeyboardFunc(windowKey);
    glutSpecialFunc(windowSpecial);
    
    glutCreateMenu(windowMenu);
    glutAddMenuEntry("Toggle Axes [a]",'a');
    glutAddMenuEntry("Toggle Values [v]",'v');
    glutAddMenuEntry("Toggle Mode [m]",'m');
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutIdleFunc(spinDisplay);
    glutMainLoop();
    return 0;
}