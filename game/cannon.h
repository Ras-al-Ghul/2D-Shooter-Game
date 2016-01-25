#include <utility>
#include <GL/glut.h>

#include "misc.h"

using namespace std;

class Cannon
{
    public:
        Cannon();
        void drawCannon();
        float get_rotAngle();
        void set_rotAngle(float newval);
        float get_rotUnit();
        void set_canBaseY(float newval);
        float get_canBaseY();
        void drawSnout();
        void drawPiston();
        float get_canYUnit();
        float get_springK();
        float get_springX();
        void set_springX(float newval);
        float get_springUnit();
        bool hitTaken;
    protected:
    private:
        float canRadius;
        float canWidth;
        float pisLength;
        float canYUnit;
        float springK;
        float springX;
        bool released;
        float rotAngle;
        float rotUnit;
        float arrX[4];
        float arrY[4];
        float canBaseY;
        float springUnit;
};

Cannon::Cannon(){
    rotAngle = 0;
    rotUnit = 5*PI/180;
    springX = 0;
    canBaseY = 0;
    canYUnit = 10;
    springK = 4;
    springUnit = 5;
    hitTaken = false;
}

void Cannon::drawCannon(){
    glColor3f(0.156863,0.156863,0.156863);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(37.25, 60);
    glVertex2f(0, 60);

    glVertex2f(0, 190);
    glVertex2f(37.25, 190);
    glVertex2f(0, 250);
    glEnd();

    glColor3f(BLACK);
    glBegin(GL_QUADS);
    glVertex2f(25, 65);
    glVertex2f(25, 100);
    glVertex2f(0, 100);
    glVertex2f(0, 65);

    glVertex2f(0, 77.5);
    glVertex2f(42.5, 77.5);
    glVertex2f(42.5, 172.5);
    glVertex2f(0, 172.5);

    glVertex2f(0, 150);
    glVertex2f(25, 150);
    glVertex2f(25, 185);
    glVertex2f(0, 185);
    glEnd();
}

void Cannon::drawSnout(){
    glColor3f(0.156863,0.156863,0.156863);
    glBegin(GL_QUADS);
    glVertex2f(35, 102.5);
    glVertex2f(140, 110);
    glVertex2f(140, 140);
    glVertex2f(35, 147.5);
    glEnd();
}

float Cannon::get_rotAngle(){
    return rotAngle;
}

void Cannon::set_rotAngle(float newval){
    rotAngle = newval;
}

float Cannon::get_rotUnit(){
    return rotUnit;
}

void Cannon::set_canBaseY(float newval){
    canBaseY = newval;
}

float Cannon::get_canBaseY(){
    return canBaseY;
}

float Cannon::get_canYUnit(){
    return canYUnit;
}

void Cannon::drawPiston(){
    glColor3f(WHITE);
    glBegin(GL_LINES);
    glVertex2f( 35, 125 );
    glVertex2f( 120 - springX , 125 );
    glVertex2f( 120 - springX , 110 );
    glVertex2f( 120 - springX , 140 );
    glEnd();
}

float Cannon::get_springK(){
    return springK;
}

float Cannon::get_springX(){
    return springX;
}

void Cannon::set_springX(float newval){
    springX = newval;
}

float Cannon::get_springUnit(){
    return springUnit;
}