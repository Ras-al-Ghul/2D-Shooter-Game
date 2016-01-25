#include <GL/glut.h>
#include <utility>
#include <cmath>

bool Projectile::released = false;

Projectile::Projectile(){
	mass = 1;
	accX = 0;
	if(!water)
		accY = -10;
	else 
		accY = -7;
	radius = 15;
	cenX = radius + 120 - c1.get_springX();
	cenY = radius + 110;
	isAlive = true;
	ex = 0.85;
	ey = 0.9;
	inBetween = false;
}

float Projectile::get_mass(){
	return mass;
}

void Projectile::set_ballCoor(float val1,float val2){
	ballX = val1;
	ballY = val2;
}

float Projectile::get_radius(){
	return radius;
}

bool Projectile::IsAlive(){
	return isAlive;
}

void Projectile::drawUnreleased(){
	glColor3f(0.9,0,0);
	cenX = radius + 120 - c1.get_springX();
	cenY = radius + 110;
	pair<float,float> worldcoordinates = getWorldCoordinates(cenX,cenY,c1.get_rotAngle(),c1.get_canBaseY());
	drawCircle(worldcoordinates.first, worldcoordinates.second, radius);
}

void Projectile::initShooting(){	
    initRotAngle = c1.get_rotAngle();
    initCanBaseY = c1.get_canBaseY();

	ballVel = sqrt( (c1.get_springX() * c1.get_springX() * c1.get_springK()) / mass);
	
	pair<float,float> worldcoordinates = getWorldCoordinates(cenX,cenY,initRotAngle,initCanBaseY);

	cenVX = ballVel*cos(initRotAngle);
	cenVY = ballVel*sin(initRotAngle);

	ballX = worldcoordinates.first;
	ballY = worldcoordinates.second;
	ballVelY = cenVY;
	ballVelX = cenVX;

	c1.set_springX(0);
}

pair<float,float> Projectile::getWorldCoordinates(float tempBallX, float tempBallY, float angle, float CanBaseY){
	float mat1[3];
	mat1[0] = tempBallX;mat1[1] = tempBallY;mat1[2] = 1;
	float mat2[3][3],mat3[3][3],mat4[3][3],mat5[3][3];
	
	mat2[0][0] = 1    ;mat2[0][1] = 0    ;mat2[0][2] = 40     ;
	mat2[1][0] = 0    ;mat2[1][1] = 1    ;mat2[1][2] = 125    ;
	mat2[2][0] = 0    ;mat2[2][1] = 0    ;mat2[2][2] = 1      ;

	float tempinitrotangle = angle;

	mat3[0][0] = cos(tempinitrotangle);mat3[0][1] =-sin(tempinitrotangle)  ;mat3[0][2] = 0     ;
	mat3[1][0] = sin(tempinitrotangle)  ;mat3[1][1] = cos(tempinitrotangle);mat3[1][2] = 0     ;
	mat3[2][0] = 0    ;mat3[2][1] = 0    ;mat3[2][2] = 1     ;

	mat4[0][0] = 1    ;mat4[0][1] = 0    ;mat4[0][2] = -40    ;
	mat4[1][0] = 0    ;mat4[1][1] = 1    ;mat4[1][2] = -125   ;
	mat4[2][0] = 0    ;mat4[2][1] = 0    ;mat4[2][2] = 1      ;

	for(int i = 0; i < 3; i++){
      	for (int j = 0; j < 3; j++){
      		float sum = 0;
        	for (int k = 0; k < 3; k++){
          	sum = sum + mat2[i][k]*mat3[k][j];
        	}
        	mat5[i][j] = sum;
      	}
    }

    for(int i = 0; i < 3; i++){
      	for (int j = 0; j < 3; j++){
      		float sum = 0;
        	for (int k = 0; k < 3; k++){
          	sum = sum + mat5[i][k]*mat4[k][j];
        	}
        	mat2[i][j] = sum;
      	}
    }

    tempBallX = mat2[0][0]*mat1[0] + mat2[0][1]*mat1[1] + mat2[0][2]*mat1[2];
    tempBallY = mat2[1][0]*mat1[0] + mat2[1][1]*mat1[1] + mat2[1][2]*mat1[2];

	tempBallY += CanBaseY;
	return make_pair(tempBallX,tempBallY);
}

void Projectile::checkCollisions(vector<Projectile*> projectiles){
	for(unsigned int i = 0;i<projectiles.size();i++){
		if(projectiles[i]->IsAlive() && projectiles[i]->get_ballX()!=this->ballX && projectiles[i]->get_ballY()!=this->ballY){
			if(EuclidDist(this->ballX,this->ballY,projectiles[i]->get_ballX(),projectiles[i]->get_ballY()) <= (3*radius)){
				double velx1 = this->ballVelX;
				double vely1 = this->ballVelY;
				double velx2 = projectiles[i]->ballVelX;
				double vely2 = projectiles[i]->ballVelY;

				double x1 = this->ballX;
				double y1 = this->ballY;
				double x2 = projectiles[i]->ballX;
				double y2 = projectiles[i]->ballY;

				int error=0;
				collision2D('a',0,1,mass,mass,radius,radius,x1,y1,x2,y2,velx1,vely1,velx2,vely2,error);

				this->ballVelX = velx1;
				this->ballVelY = vely1;
				projectiles[i]->ballVelX = velx2;
				projectiles[i]->ballVelY = vely2;

				this->ballX = x1;
				this->ballY = y1;
				projectiles[i]->ballX = x2;
				projectiles[i]->ballY = y2;

			}			
		}
	}
	for(unsigned int j = 0;j<targets.size();j++){
		if(ballX >= targets[j]->get_bottomleft().first && ballX <= targets[j]->get_bottomright().first && ballY <= targets[j]->get_currenttopleft().second){
			inBetween = true;
		}
		if(inBetween == true){
			if(ballVelX > 0 && ballX >= targets[j]->get_bottomright().first){
				ballVelX = -(ex*ballVelX);
				inBetween = false;
				if(ballVelY > 0 && ballY >= targets[j]->get_currenttopleft().second){
					ballVelY = -(ey*ballVelY);
				}
				break;
			}
			if(ballVelX < 0 && ballX <= targets[j]->get_bottomleft().first){
				ballVelX = -(ex*ballVelX);
				inBetween = false;
				if(ballVelY > 0 && ballY >= targets[j]->get_currenttopleft().second){
					ballVelY = -(ey*ballVelY);
				}
				break;
			}
		}
		if(this->ballVelX > 0){
			if((abs(targets[j]->get_bottomleft().first - this->ballX) <= this->radius) && (this->ballY + radius <= targets[j]->get_currenttopleft().second)){
				this->ballVelX = -(ex*this->ballVelX);
				this->ballX = targets[j]->get_bottomleft().first - this->radius;
			}
		}
		if(this->ballVelX < 0){
			if(( abs(this->ballX - targets[j]->get_bottomright().first ) <= this->radius) && (this->ballY + radius <= targets[j]->get_currenttopright().second)){
				this->ballVelX = -(ex*this->ballVelX);
				this->ballX = targets[j]->get_bottomright().first + this->radius;
			}
		}
	}
}

pair<float,float> Projectile::getBallFrameCoordinates(float val1,float val2){
	float mat1[3];
	mat1[0] = val1;mat1[1] = val2 - initCanBaseY;mat1[2] = 1;
	float mat2[3][3],mat3[3][3],mat4[3][3],mat5[3];
	
	mat2[0][0] = 1    ;mat2[0][1] = 0    ;mat2[0][2] = -40     ;
	mat2[1][0] = 0    ;mat2[1][1] = 1    ;mat2[1][2] = -125    ;
	mat2[2][0] = 0    ;mat2[2][1] = 0    ;mat2[2][2] = 1      ;

	float tempinitrotangle = -(initRotAngle * PI/180);

	mat3[0][0] = cos(tempinitrotangle);mat3[0][1] =-sin(tempinitrotangle)  ;mat3[0][2] = 0     ;
	mat3[1][0] = sin(tempinitrotangle)  ;mat3[1][1] = cos(tempinitrotangle);mat3[1][2] = 0     ;
	mat3[2][0] = 0    ;mat3[2][1] = 0    ;mat3[2][2] = 1     ;

	mat4[0][0] = 1    ;mat4[0][1] = 0    ;mat4[0][2] = 40    ;
	mat4[1][0] = 0    ;mat4[1][1] = 1    ;mat4[1][2] = 125   ;
	mat4[2][0] = 0    ;mat4[2][1] = 0    ;mat4[2][2] = 1      ;

	mat5[0] = mat2[0][0]*mat1[0] + mat2[0][1]*mat1[1] + mat2[0][2]*mat1[2];
	mat5[1] = mat2[1][0]*mat1[0] + mat2[1][1]*mat1[1] + mat2[1][2]*mat1[2];
	mat5[2] = mat2[2][0]*mat1[0] + mat2[2][1]*mat1[1] + mat2[2][2]*mat1[2];

	mat1[0] = mat3[0][0]*mat5[0] + mat3[0][1]*mat5[1] + mat3[0][2]*mat5[2];
	mat1[1] = mat3[1][0]*mat5[0] + mat3[1][1]*mat5[1] + mat3[1][2]*mat5[2];
	mat1[2] = mat3[2][0]*mat5[0] + mat3[2][1]*mat5[1] + mat3[2][2]*mat5[2];

	mat5[0] = mat4[0][0]*mat1[0] + mat4[0][1]*mat1[1] + mat4[0][2]*mat1[2];
	mat5[1] = mat4[1][0]*mat1[0] + mat4[1][1]*mat1[1] + mat4[1][2]*mat1[2];
	mat5[2] = mat4[2][0]*mat1[0] + mat4[2][1]*mat1[1] + mat4[2][2]*mat1[2];

	return make_pair(mat5[0],mat5[1]);
}

void Projectile::checkStopping(){

	if(ballY <= 95){
		
		ballVelY = -(ey)*ballVelY;
		ballVelX = ex*ballVelX;
		
		ballY = 95;
	}

	if(ballY >= 900){
		ballVelY = -(ey)*ballVelY;
		ballVelX = ex*ballVelX;
		ballY = 885;
	}

	if(ballX <= 0){
		ballVelX = -(ex*ballVelX);
		ballVelY = ey*ballVelY;
		ballX = 15;
	}

	if(abs(ballVelX) < 1 && abs(ballVelY) < 1)
		isAlive = false;
}

void Projectile::drawReleased(vector<Projectile*> projectiles){
    if(!(ballVelX > 0))
    	envX = 0;
    else
    	envX = water == true ? 3 : 0.01;
    if(!(ballVelY > 0))
    	envY = 0;
    else
    	envY = water == true ? 3 : 0.01;
    ballVelX = ballVelX + (accX-envX) * ut;
	ballX = ballX + ballVelX*ut;
	ballVelY = ballVelY + (accY-envY) * ut;
	ballY = ballY + ballVelY*ut;

	checkCollisions(projectiles);
	checkStopping();

	glColor3f(0.9,0,0);
	drawCircle(ballX,ballY,radius);
}

float Projectile::get_ballX(){
	return ballX;
}

float Projectile::get_ballY(){
	return ballY;
}