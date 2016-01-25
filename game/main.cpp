#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <string.h>
#include <sys/time.h>
#include <vector>

#include <SFML/Audio.hpp>
sf::Sound cannon,smash,laugh;

struct timeval tv;
time_t current = 0,previous = 0, gametime = 70, starttime, curtime, elapsedtime, oldtime = 0, beasttime, explosiontime;
bool gameover = false;
bool levelup = false;
bool water = false;

#include "player.h"

Player p1;
int numoftargets;

#include "target.h"
#include "imageloader.h"

Cannon c1;

vector<Target*> targets;
Target* curTarget;

#include "projectile.h"
#include "beast.h"

Beast b1;

vector<Projectile*> projectiles;
Projectile* curProjectile;

using namespace std;

void initialize(){
    
    zoom = 0.0;
    
    glClearColor( SKY_BLUE , 1.f);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluOrtho2D(0.0,1600,0.0,900);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(!water){
        Image* image = loadBMP("../images/land.bmp");
        _textureId = loadTexture(image);
        delete image;
    }
    else{
        Image* image = loadBMP("../images/water.bmp");
        _textureId = loadTexture(image);
        delete image;    
    }

    b1.loadTex();

    curProjectile = new Projectile();

    int i=0;
    while(Target::endpoint < 1550){
        curTarget = new Target();
        targets.push_back(curTarget);
        i++;
    }
    numoftargets = i;
}

void displayScoreboard(){
    glColor3f(BLACK);

    glBegin(GL_QUADS);
    glVertex2f( 1330, 785 );
    glVertex2f( 1600, 785 );
    glVertex2f( 1600, 875 );
    glVertex2f( 1330, 875 );
    glEnd();

    glColor3f(1, 0.843137, 0);
    char sc[100];
    sprintf(sc,"Score:%d",p1.getScore());
    glRasterPos3d(1335,850 ,0.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );
    sprintf(sc,"HighScore:%d (%s)",p1.highscore,p1.highscorerName.c_str());
    glRasterPos3d(1335,820 ,0.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );

    gettimeofday(&tv,NULL);
    curtime = tv.tv_sec;
    elapsedtime = curtime - starttime;
    if(oldtime == 0 && !gameover){
        gametime = gametime - elapsedtime;
        oldtime = elapsedtime;
        beasttime = elapsedtime;
        explosiontime = elapsedtime;
    }
    else if(elapsedtime - oldtime >= 1 && !gameover){
        gametime = gametime - 1;
        oldtime = elapsedtime;
    }

    if(gameover){
        if(elapsedtime - oldtime >= 5){
            if(p1.level == 1){
                p1.setScore(p1.getScore()+gametime*p1.level);
                levelup = true;
            }
            else if(p1.level == 2){
                p1.setScore(p1.getScore()+gametime*p1.level);
                levelup = true;
            }
            else{
                p1.setScore(p1.getScore()+gametime*p1.level);
                if(p1.getScore() > p1.highscore)
                    p1.writeHighScore();
                cout<<"Well done..You were superb.. Your score is "<<p1.getScore()<<endl;
                exit(0);
            }
        }
    }

    if(gametime < 0){
        if(p1.getScore() > p1.highscore)
            p1.writeHighScore();
        cout<<"Haha!!You shamed yourself..Go home crying!!"<<endl;
        exit(0);
    }

    if(p1.level == 1){
        if(elapsedtime - beasttime >= 10){
            b1.renderBeast();
            if(b1.laughs == false){
                b1.laughs = true;
                laugh.play();
            }
            if(elapsedtime - beasttime >= 12){
                beasttime = elapsedtime;
                b1.missileFired = false;
                b1.laughs = false;
            }
        }
    }
    else if(p1.level == 2){
        if(elapsedtime - beasttime >= 7){
            b1.renderBeast();
            if(b1.laughs == false){
                b1.laughs = true;
                laugh.play();
            }
            if(elapsedtime - beasttime >= 10){
                beasttime = elapsedtime;
                b1.missileFired = false;
                b1.laughs = false;
            }
        }
    }
    else{
        if(elapsedtime - beasttime >= 5){
            b1.renderBeast();
            if(b1.laughs == false){
                b1.laughs = true;
                laugh.play();
            }
            if(elapsedtime - beasttime >= 7){
                beasttime = elapsedtime;
                b1.missileFired = false;
                b1.laughs = false;
            }
        }
    }

    b1.moveMissile();
    b1.drawMissile();
    b1.detectCollision();

    if(c1.hitTaken){
        if(elapsedtime - explosiontime <= 2){
            b1.drawExplosion();
        }
        else{
            c1.hitTaken = false;
            b1.explosionradius = 0;
        }
    }

    glColor3f(1, 0.843137, 0);
    sprintf(sc,"Time Remaining:%f",(float)gametime);
    glRasterPos3d(1335,790 ,0.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );    

    glColor3f(BLACK);
    glBegin(GL_QUADS);
    glVertex2f( 0, 875 );
    glVertex2f( 0, 785 );
    glVertex2f( 130, 785 );
    glVertex2f( 130, 875 );
    glEnd();

    glColor3f(1, 0.843137, 0);
    sprintf(sc,"%s",p1.getName().c_str());
    glRasterPos3d(5,850 ,0.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );

    glColor3f(1, 0.843137, 0);
    sprintf(sc,"Lives:%d",p1.lives);
    glRasterPos3d(5,820 ,0.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );

    glColor3f(1, 0.843137, 0);
    sprintf(sc,"Level:%d",p1.level);
    glRasterPos3d(5,790 ,0.0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );        

    glColor3f(1, 0, 0);
    if(p1.level == 3){
        glColor3f(1, 0.843137, 0);
        sprintf(sc,"Shots left:%d",p1.shots);
        glRasterPos3d(5,760 ,0.0);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc ); 
    }

}

void adjustCamera(){
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    myOrtho(100,45);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
}

void display(void){

    if(current == 0 && previous == 0){
        gettimeofday(&tv,NULL);
        current = tv.tv_sec;
        previous = current;
    }

    adjustCamera();
    glClear(GL_COLOR_BUFFER_BIT);
    giveTexture();
    
    displayScoreboard();

    glPushMatrix();
    
    glColor3f(BLACK);
    glTranslatef(0, c1.get_canBaseY(), 0);
    
    glPushMatrix();
    glTranslatef(40, 125, 0);
    glRotatef(c1.get_rotAngle()*180/PI, 0, 0, 1);
    glTranslatef(-40, -125, 0);
    c1.drawSnout();
    c1.drawPiston();
    
    glPopMatrix();
    c1.drawCannon();
    glPopMatrix();
    
    gettimeofday(&tv,NULL);
    current = tv.tv_sec;
    if(current - previous == 0.1){
        ut = current - previous;
        previous = current;
    }

    for(unsigned int i=0;i<targets.size();i++){
        targets[i]->draw(projectiles);
        targets[i]->move();
    }

    curProjectile->drawUnreleased();

    for(unsigned int i=0;i<projectiles.size();i++){
        if(projectiles[i]->IsAlive()){
            projectiles[i]->drawReleased(projectiles);
        }
    }

    glFlush();
    glutSwapBuffers();
}

void update (int value){
    if(levelup == true && p1.level == 1){
        targets.clear();
        projectiles.clear();
        b1.missileCoor.clear();
        Projectile::released = false;
        Target::endpoint = 350;
        initialize();
        gettimeofday(&tv,NULL);
        starttime = tv.tv_sec;
        oldtime = 0;
        levelup = false;
        p1.level = 2;
        p1.lives = 3;
        gametime = 55;
        gameover = false;
        glutDisplayFunc(display);
    }
    else if(levelup == true && p1.level == 2){
        p1.shots = 10;
        targets.clear();
        projectiles.clear();
        b1.missileCoor.clear();
        Projectile::released = false;
        Target::endpoint = 350;
        initialize();
        gettimeofday(&tv,NULL);
        starttime = tv.tv_sec;
        oldtime = 0;
        levelup = false;
        p1.level = 3;
        p1.lives = 3;
        gametime = 40;
        gameover = false;
        glutDisplayFunc(display);   
    }
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

void keyboard( unsigned char key, int mouseX , int mouseY){
    if( key == 27){
        exit(0);
    }
    if( key == 'a'){
        if( c1.get_rotAngle() < 30*PI/180 )
            c1.set_rotAngle( c1.get_rotAngle() + c1.get_rotUnit() );
    }
    if( key =='b'){
        if( c1.get_rotAngle() > -30*PI/180 )
            c1.set_rotAngle( c1.get_rotAngle() - c1.get_rotUnit() );
    }
    
    if(key =='p'){
        if( c1.get_canBaseY() < HEIGHT - c1.get_canYUnit() - 40 ){
            c1.set_canBaseY( c1.get_canBaseY() + c1.get_canYUnit() );
        }
    }
    if(key == 'l'){
        if( c1.get_canBaseY() > 0 ){
            c1.set_canBaseY( c1.get_canBaseY() - c1.get_canYUnit() );
        }
    }
    if( key == 'f'){
        if( c1.get_springX() < 80 ){
            c1.set_springX(c1.get_springX() + c1.get_springUnit());
        }
    }
    if( key == 's'){
        if( c1.get_springX() > 0 ){
            c1.set_springX(c1.get_springX() - c1.get_springUnit());
        }
    }
    if( key == ' '){
        curProjectile->initShooting();
        projectiles.push_back(curProjectile);
        cannon.play();
        curProjectile = new Projectile();
        if(p1.shots == 0 && p1.level == 3){
            gameover = true;
            return;
        }
        p1.shots--;
    }
}

void SpecialInput(int key, int x, int y){
    
    switch(key){
        case GLUT_KEY_UP:
                zoom += 200.0;
            break;
        case GLUT_KEY_DOWN:
                zoom -= 200.0;
            break;
        case GLUT_KEY_LEFT:
            x1plus += 3;
            x2plus += 3;
            break;
        case GLUT_KEY_RIGHT:
            x1plus -= 3;
            x2plus -= 3;
            break;
    }
    glutPostRedisplay();
}

char buttons;
int oldx = 0,oldy = 0;

void mousePressed(int x,int y){
    if(buttons == 'l'){
        if(oldy == 0){
            oldy = y;
        }
        else{
            if(oldy > y){
               if( c1.get_springX() < 80){
                    c1.set_springX(c1.get_springX() + 1);
                } 
            }
            else{
                if( c1.get_springX() > 0){
                    c1.set_springX(c1.get_springX() - 1);
                }
            }
        }
        float my = 900-y;
        float mx = x;
        float tempang = atan(my/mx);
        if(tempang > -(PI/3) && tempang < (PI/3))
            c1.set_rotAngle(tempang);
    }
    if(buttons == 'r'){
        if(oldx == 0){
            oldx = x;
        }
        else{
            if(oldx > x){
                x1plus += 3;
                x2plus += 3;
            }
            else{
                x1plus -= 3;
                x2plus -= 3;
            }
        }
    }
}
void mouseClick(int button,int state,int x, int y){
    if(button == GLUT_LEFT_BUTTON && state==GLUT_UP){
        oldy = 0;
        curProjectile->initShooting();
        cannon.play();
        projectiles.push_back(curProjectile);
        curProjectile = new Projectile();
        if(p1.shots == 0 && p1.level == 3){
            gameover = true;
            return;
        }
        p1.shots--;
    }
    if(button == GLUT_LEFT_BUTTON){
        buttons = 'l';
    }
    if(button == GLUT_RIGHT_BUTTON){
        oldx = 0;
        buttons = 'r';
    }
    if(button == 3){
        zoom += 200;
    }
    if(button == 4){
        zoom -= 200;
    }
}

int main(int argc, char *argv[]){
    
    srand(time(NULL));

    sf::SoundBuffer buffer1;
    if(!buffer1.loadFromFile("../audio/cannon.wav"))
        return -1;
    cannon.setBuffer(buffer1);

    sf::SoundBuffer buffer2;
    if(!buffer2.loadFromFile("../audio/smash.wav"))
        return -1;
    smash.setBuffer(buffer2);

    sf::SoundBuffer buffer3;
    if(!buffer3.loadFromFile("../audio/laugh.wav"))
        return -1;
    laugh.setBuffer(buffer3);

    gettimeofday(&tv,NULL);
    starttime = tv.tv_sec;

    string tempname;
    cout<<"Enter Player Name"<<endl;
    cin>>tempname;
    
    p1.setPlayerName(tempname);

    cout<<"Select setting 1 Land 2 Underwater ..?"<<endl;
    cin>>tempname;

    if(tempname == "2")
        water = true;
    
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( WIDTH , HEIGHT );
    glutInitWindowPosition(0,0);
    glutCreateWindow("GAME");

    initialize();

    glutDisplayFunc(display);
    glutTimerFunc(25, update , 0);

    glutSpecialFunc(SpecialInput);
    glutKeyboardFunc(keyboard);

    glutMouseFunc(mouseClick);
    glutMotionFunc(mousePressed);

    glutMainLoop();

    return 0;
}