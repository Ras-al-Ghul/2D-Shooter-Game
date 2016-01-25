#include <utility>
#include <cstdlib>
#include <iostream>

#include "cannon.h"

class Projectile{
	public:
		static bool released;
		float ballVelX,ballVelY;
		Projectile();
		void drawUnreleased();
		void initShooting();
		void drawReleased(vector<Projectile*> projectiles);
		void checkCollisions(vector<Projectile*> projectiles);
		void checkStopping();
		float get_ballX();
		float get_ballY();
		float get_radius();
		void set_ballCoor(float val1,float val2);
		bool IsAlive();
		float get_mass();
		pair<float,float> getWorldCoordinates(float tempBallX,float tempBallY,float angle, float CanBaseY);
		pair<float,float> getBallFrameCoordinates(float val1,float val2);
	protected:
	private:
		float radius;
		float accX, accY;
		float mass;
		float cenX , cenY;
		float cenVX , cenVY;
		float ballX,ballY,ballVel;
		float initRotAngle;
		float initCanBaseY;
		bool isAlive;
		float ex,ey;
		bool inBetween;
		float envX,envY;
};

using namespace std;

#define MOVEUPBY 3.5
#define MOVEDOWNBY 3.5

class Target{
	public:
		Target();
		pair<float, float> get_bottomleft();
		pair<float, float> get_bottomright();
		pair<float, float> get_topleft();
		pair<float, float> get_topright();
		pair<float, float> get_currenttopleft();
		pair<float, float> get_currenttopright();
		void drawCircles();
		void drawSquare();
		void drawDiamond();
		void drawTriangle();
		void drawPillar();
		void moveUp();
		void moveDown();
		void move();
		void draw(vector<Projectile*> projectiles);
		void makeCircle();
		static float endpoint;
	protected:
	private:
		float height;
		float width;
		bool up;
		bool collapsed;
		pair<float, float> bottomleft;
		pair<float, float> bottomright;
		pair<float, float> topleft;
		pair<float, float> topright;
		pair<float, float> currenttopleft;
		pair<float, float> currenttopright;
		pair<float, float> circleCoor;
		float circleRadius;
		float circleVelX,circleVelY;
		float circleAcclX,circleAcclY;
		float velocity;
		float color[3];//Stores RGB values
		char shape;//Square,Diamond,Circle,Triangle
		bool collided;
		int triangleToggle;
		int circleToggle1;
		int circleToggle2;
		int circleToggle3;
		int circleToggle4;
		int squareToggle;
		int diamondToggle;
		bool diamondDir;
		int diamondRotateToggle;
		bool inAir;
		int scoreNum;
};

float EuclidDist(float x1,float y1,float x2,float y2){
	return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}

float Target::endpoint = 350;

Target::Target(){
	triangleToggle = 0;
	circleToggle1 = 0;
	circleToggle2 = 45;
	circleToggle3 = 90;
	circleToggle4 = 135;
	squareToggle = 0;
	diamondToggle = 0;
	diamondRotateToggle = 0;
	diamondDir = true;

	inAir = false;

	scoreNum = 0;

	height = (rand()%550) + 250;
	width = (rand()%20) + 50;

	collided = false;
	circleVelX = 0;
	circleVelY = 0;
	
	circleAcclX = 0;
	if(!water)
		circleAcclY = -10;
	else
		circleAcclY = -10+5;

	int choice = rand()%4;
	switch(choice){
		case 0:
			shape = 'S';
			break;
		case 1:
			shape = 'D';
			break;
		case 2:
			shape = 'C';
			break;
		case 3:
			shape = 'T';
			break;
	}

	up = true;
	collapsed = false;

	velocity = (rand() % 30) + 30;

	choice = rand()%6;
	switch(choice){
		case 0:
			color[0] = 1.0; color[1] = 1.0; color[2] = 1.0;
			break;
		case 1:
			color[0] = 0.913725; color[1] = 0.588235; color[2] = 0.478431;
			break;
		case 2:
			color[0] = 0.8; color[1] = 0.1; color[2] = 0.2;
			break;
		case 3:
			color[0] = 0.5; color[1] = 0.0; color[2] = 0.8;
			break;
		case 4:
			color[0] = 0.0; color[1] = 0.75; color[2] = 0.0;
			break;
		case 5:
			color[0] = 0.466667; color[1] = 0.533333; color[2] = 0.6;
			break;
	}

	float tempx, tempy;
	while(1){
		tempx = (rand()%30) + endpoint + 100;
		if(tempx + width < 1600)
			break;
	}

	endpoint = tempx + width;
	tempy = 0;

	bottomleft = make_pair(tempx, tempy);
	bottomright = make_pair(tempx+width, tempy);
	topleft = make_pair(tempx, tempy+height);
	topright = make_pair(tempx+width, tempy+height);

	currenttopleft = bottomleft;
	currenttopright = bottomright;

	circleCoor.first = (currenttopleft.first + currenttopright.first) / 2;
	circleCoor.second = circleCoor.first - currenttopleft.first + currenttopleft.second;
	circleRadius = circleCoor.first - currenttopleft.first;
}

pair<float, float> Target::get_bottomleft(){
	return bottomleft;
}

pair<float, float> Target::get_bottomright(){
	return bottomright;
}

pair<float, float> Target::get_topleft(){
	return topleft;
}

pair<float, float> Target::get_topright(){
	return topright;
}

pair<float, float> Target::get_currenttopleft(){
	return currenttopleft;
}

pair<float, float> Target::get_currenttopright(){
	return currenttopright;
}

void Target::drawCircles(){
	glColor3f(1,0,0);
	drawCircle(circleCoor.first, circleCoor.second, circleRadius);
	glColor3f(SKY_BLUE);
	
	glPushMatrix();
	circleToggle1 = (circleToggle1 + 15) % 360;
	glTranslatef(circleCoor.first,circleCoor.second,0);
	glRotatef(circleToggle1, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(-circleRadius,0);
	glVertex2f(circleRadius,0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	circleToggle2 = (circleToggle2 + 15) % 360;
	glTranslatef(circleCoor.first,circleCoor.second,0);
	glRotatef(circleToggle2, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(-circleRadius,0);
	glVertex2f(circleRadius,0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	circleToggle3 = (circleToggle3 + 15) % 360;
	glTranslatef(circleCoor.first,circleCoor.second,0);
	glRotatef(circleToggle3, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(-circleRadius,0);
	glVertex2f(circleRadius,0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	circleToggle4 = (circleToggle4 + 15) % 360;
	glTranslatef(circleCoor.first,circleCoor.second,0);
	glRotatef(circleToggle4, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(-circleRadius,0);
	glVertex2f(circleRadius,0);
	glEnd();
	if(scoreNum != 0){
		glColor3f(RED);
		char sc[100];
		sprintf(sc,"%d",scoreNum);
	    glRasterPos3d(0,0,0.0);
	    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );
	}
	glPopMatrix();
}

void Target::drawDiamond(){
	if(diamondDir == true){
		diamondToggle = diamondToggle + 1;
		if(diamondToggle == 5)
			diamondDir = false;
	}
	else{
		diamondToggle = diamondToggle - 1;
		if(diamondToggle == 0)
			diamondDir = true;
	}
	glColor3f(1, 1, 0);
	glPushMatrix();
	diamondRotateToggle = (diamondRotateToggle+1) % 360;
	glTranslatef(circleCoor.first,circleCoor.second,0);
	glRotatef(diamondRotateToggle, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(-circleRadius-diamondToggle,-circleRadius-diamondToggle);
	glVertex2f(+circleRadius+diamondToggle,-circleRadius-diamondToggle);
	glVertex2f(0,circleRadius+diamondToggle);
	glEnd();
	glTranslatef(0,-17 ,0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-circleRadius-diamondToggle,+circleRadius+diamondToggle);
	glVertex2f(0,-circleRadius-diamondToggle);
	glVertex2f(circleRadius+diamondToggle,circleRadius+diamondToggle);
	glEnd();
	if(scoreNum != 0){
		glColor3f(RED);
		char sc[100];
		sprintf(sc,"%d",scoreNum);
	    glRasterPos3d(0,0,0.0);
	    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );
	}
	glPopMatrix();
}

void Target::drawSquare(){
	glColor3f(1,0.843137,0);
	glPushMatrix();
	int rands = (rand() % 20) + 5;
	if(rand() % 2 == 0)
		squareToggle = (squareToggle - rands) % 360;
	else
		squareToggle = (squareToggle + rands) % 360;
	glTranslatef(circleCoor.first,circleCoor.second,0);
	glRotatef(squareToggle, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex2f(-circleRadius,-circleRadius);
	glVertex2f(+circleRadius,-circleRadius);
	glVertex2f(+circleRadius,+circleRadius);
	glVertex2f(-circleRadius,+circleRadius);
	glEnd();
	if(scoreNum != 0){
		glColor3f(RED);
		char sc[100];
		sprintf(sc,"%d",scoreNum);
	    glRasterPos3d(0,0,0.0);
	    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );
	}
	glPopMatrix();
}

void Target::drawTriangle(){
	glColor3f(1,0.270588,0);
	glPushMatrix();
	int rands = (rand() % 20) + 5;
	triangleToggle = (triangleToggle + rands) % 360;
	glTranslatef(circleCoor.first,circleCoor.second,0);
	glRotatef(triangleToggle, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(-circleRadius,-circleRadius);
	glVertex2f(+circleRadius,-circleRadius);
	glVertex2f(0,circleRadius);
	glEnd();
	if(scoreNum != 0){
		glColor3f(RED);
		char sc[100];
		sprintf(sc,"%d",scoreNum);
	    glRasterPos3d(0,0,0.0);
	    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );
	}
	glPopMatrix();
}

void Target::drawPillar(){
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f( bottomleft.first, bottomleft.second);
	glVertex2f( bottomright.first, bottomright.second);
	glVertex2f( currenttopright.first, currenttopright.second);
	glVertex2f( currenttopleft.first, currenttopleft.second);
	glEnd();
}

void collision2D(char mode,double alpha, double R,
                 double m1, double m2, double r1, double r2,
                 double& x1, double& y1, double& x2, double& y2,
                 double& vx1, double& vy1, double& vx2, double& vy2,
                 int& error ){

    double  r12,m21,d,gammav,gammaxy,dgamma,dr,dc,sqs,t,dvx2,a,x21,y21,vx21,vy21,pi2,vx_cm,vy_cm;

    pi2=2*acos(-1.0E0);
    error=0;
    r12=r1+r2;
    m21=m2/m1;
    x21=x2-x1;
    y21=y2-y1;
    vx21=vx2-vx1;
    vy21=vy2-vy1;

    vx_cm = (m1*vx1+m2*vx2)/(m1+m2) ;
    vy_cm = (m1*vy1+m2*vy2)/(m1+m2) ;

    if ( vx21==0 && vy21==0 ) {
    	error=1; 
    	return;
    }

    gammav=atan2(-vy21,-vx21);

    if (mode != 'f') {
    	d=sqrt(x21*x21 +y21*y21);
     
        if (d<r12) {
        	error=2; 
        	return;
        }
        gammaxy=atan2(y21,x21);
        dgamma=gammaxy-gammav;
        if (dgamma>pi2) {
        	dgamma=dgamma-pi2;
        }
        else if (dgamma<-pi2) {
        	dgamma=dgamma+pi2;
        }
       	dr=d*sin(dgamma)/r12;
       
        if (  (fabs(dgamma)>pi2/4 && fabs(dgamma)<0.75*pi2) || fabs(dr)>1 ){
       		error=1; 
       		return;
        }

        alpha=asin(dr);

       	dc=d*cos(dgamma);
       	if (dc>0) {
       		sqs=1.0;
       	} 
       	else {
       		sqs=-1.0;
       	}
       	t=(dc-sqs*r12*sqrt(1-dr*dr))/sqrt(vx21*vx21+ vy21*vy21);

       	x1=x1+vx1*t;
       	y1=y1+vy1*t;
       	x2=x2+vx2*t;
       	y2=y2+vy2*t;
   	}

    a=tan( gammav +alpha);

    dvx2=-2*(vx21 +a*vy21) /((1+a*a)*(1+m21));
       
    vx2=vx2+dvx2;
    vy2=vy2+a*dvx2;
    vx1=vx1-m21*dvx2;
    vy1=vy1-a*m21*dvx2;
	   
    vx1=(vx1-vx_cm)*R + vx_cm;
    vy1=(vy1-vy_cm)*R + vy_cm;
    vx2=(vx2-vx_cm)*R + vx_cm;
    vy2=(vy2-vy_cm)*R + vy_cm;
      
    return;
}

void Target::draw(vector<Projectile*> projectiles){
	drawPillar();
	for(unsigned int i=0;i<projectiles.size();i++){
		if(projectiles[i]->IsAlive() && EuclidDist(circleCoor.first,circleCoor.second,projectiles[i]->get_ballX(),projectiles[i]->get_ballY()) <= circleRadius+projectiles[i]->get_radius()+15){
			double velx1 = this->circleVelX;
			double vely1 = this->circleVelY;
			double velx2 = projectiles[i]->ballVelX;
			double vely2 = projectiles[i]->ballVelY;

			double x1 = this->circleCoor.first;
			double y1 = this->circleCoor.second;
			double x2 = projectiles[i]->get_ballX();
			double y2 = projectiles[i]->get_ballY();

			int error = 1;
			collision2D('a',0,1,projectiles[i]->get_mass(),projectiles[i]->get_mass(),circleRadius,projectiles[i]->get_radius(),x1,y1,x2,y2,velx1,vely1,velx2,vely2,error);
			
			if(error == 0){
				smash.play();
				collided = true;
				gametime += 5;
				switch(shape){
					case 'C':
						scoreNum = 5;
						p1.setScore(p1.getScore() + scoreNum);
						break;
					case 'D':
						scoreNum = 20;
						p1.setScore(p1.getScore() + scoreNum);
						break;
					case 'S':
						scoreNum = 15;
						p1.setScore(p1.getScore() + scoreNum);
						break;
					case 'T':
						scoreNum = 10;
						p1.setScore(p1.getScore() + scoreNum);
						break;
				}
				numoftargets --;
				if(numoftargets == 0){
					gameover = true;
				}
			}

			this->circleVelX = velx1;
			this->circleVelY = vely1;
			projectiles[i]->ballVelX = velx2;
			projectiles[i]->ballVelY = vely2;

			this->circleCoor.first = x1;
			this->circleCoor.second = y1;
			projectiles[i]->set_ballCoor(x2, y2);
		}
	}
	switch(shape){
		case 'C':
			makeCircle();
			drawCircles();
			break;
		case 'D':
			makeCircle();
			drawDiamond();
			break;
		case 'S':
			makeCircle();
			drawSquare();
			break;
		case 'T':
			makeCircle();
			drawTriangle();
			break;
	}
}

void Target::makeCircle(){
	if(collided == false && inAir == false){
		circleAcclY = 0;
		circleCoor.first = (currenttopleft.first + currenttopright.first) / 2;
		circleCoor.second = circleCoor.first - currenttopleft.first + currenttopleft.second + (rand() % 200) + 250;
		circleRadius = circleCoor.first - currenttopleft.first;
		circleVelX = 0;
		circleVelY = 0;
		inAir = true;
	}
	else
		circleAcclY = -3;

	circleVelX = circleVelX + (circleAcclX) * ut;
	circleCoor.first += circleVelX*ut;
	circleVelY = circleVelY + (circleAcclY) * ut;
	circleCoor.second += circleVelY*ut;
	if(circleCoor.second <= currenttopleft.second)
		inAir = false;
}

void Target::moveUp(){
	currenttopleft.second += velocity*ut;
	currenttopright.second += velocity*ut;
}

void Target::moveDown(){
	currenttopleft.second -= velocity*ut;
	currenttopright.second -= velocity*ut;
}

void Target::move(){
	if(up == true){
		if( currenttopleft.second < topleft.second )
			moveUp();
		else
			up = false;
	}
	else{
		if( currenttopleft.second > 0 )
			moveDown();
		else
			up = true;
	}
}