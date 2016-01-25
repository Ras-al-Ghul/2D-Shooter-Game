#include <utility>

class Beast{
	public:
		bool missileFired;
		vector<pair<pair<float,float>,pair<float,bool> > >missileCoor;
		vector<float> velocities;
		bool laughs;
		Beast();
		void renderBeast();
		void giveTextureBeast();
		void loadTex();
		void drawMissile();
		void moveMissile();
		void detectCollision();
		void drawExplosion();
		float explosionradius;
	protected:
	private:
		float beastX,beastY;
		float beastRadius;
		GLuint beastTexture;
		float missileX,missileY;
		float missileVelX;
		bool toggle;
};

Beast::Beast(){
	laughs = false;
	beastRadius = 50;
	missileFired = false;
	explosionradius = 47;
	toggle = true;
}

void Beast::drawExplosion(){
	glColor3f(RED);
	drawCircle(missileX,missileY,explosionradius);
	if( explosionradius <= 50 && toggle == true ){
		explosionradius+=2;
	}
	else{
		toggle = false;
	}
	if( explosionradius >= 40 && toggle == false ){
		explosionradius-=2;
	}
	else{
		toggle = true;
	}

}

void Beast::giveTextureBeast(){
	glColor3f(WHITE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, beastTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float temp = beastRadius;
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-temp,-temp,0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(temp,-temp,0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(temp,temp,0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-temp,temp, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Beast::loadTex(){
	gluOrtho2D(0.0,1600,0.0,900);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(!water){
    	Image* image = loadBMP("../images/monster.bmp");
    	beastTexture = loadTexture(image);
    	delete image;
	}
	else{
		Image* image = loadBMP("../images/shark.bmp");
        beastTexture = loadTexture(image);
        delete image;
	}
}

void Beast::drawMissile(){
	for(unsigned int i=0; i<missileCoor.size();i++){
		if(missileCoor[i].second.second){
			glPushMatrix();
			glTranslatef(missileCoor[i].first.first,missileCoor[i].first.second,0);
			glColor3f(BLACK);
			glBegin(GL_QUADS);
			glVertex2f(-80,-15);
			glVertex2f(0,-15);
			glVertex2f(0,15);
			glVertex2f(-80,15);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex2f(-20,0);
			glVertex2f(10,-30);
			glVertex2f(10,30);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex2f(-80,-15);
			glVertex2f(-80,15);
			glVertex2f(-90,0);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex2f(-50,15);
			glVertex2f(-35,15);
			glVertex2f(-10,30);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex2f(-50,-15);
			glVertex2f(-35,-15);
			glVertex2f(-10,-30);
			glEnd();
			glPopMatrix();
		}
	}
}

void Beast::moveMissile(){
	for(unsigned int i=0; i<missileCoor.size();i++){
		if(missileCoor[i].second.second){	
			velocities[i] = velocities[i] + (missileCoor[i].second.first) * ut;
			missileCoor[i].first.first = missileCoor[i].first.first - velocities[i]*ut;
			if(missileCoor[i].first.first <= 0)
				missileCoor[i].second.second = false;
		}
	}
}

void updateTime(){
	explosiontime = elapsedtime;
}

void Beast::detectCollision(){
	for(unsigned int i=0; i<missileCoor.size();i++){
		if(missileCoor[i].second.second){
			if((missileCoor[i].first.first - 80) <= 40 && missileCoor[i].first.second >= c1.get_canBaseY()+125-30 && missileCoor[i].first.second <= c1.get_canBaseY()+125+30){
				missileCoor[i].second.second = false;
				p1.setScore(p1.getScore() - 10);
				c1.hitTaken = true;
				smash.play();
				p1.lives--;
				if(p1.lives < 0){
					gameover = true;
					p1.lives = 0;
				}
				missileX = missileCoor[i].first.first;
				missileY = missileCoor[i].first.second;
				updateTime();
			}
		}
	}
}

void Beast::renderBeast(){
    glPushMatrix();
    beastX = (rand() % 400) + 1150;
    beastY = c1.get_canBaseY() + 125;
    glTranslatef(beastX,beastY,0);
    giveTextureBeast();
    glPopMatrix();
    if(!missileFired){
    	missileFired = true;
    	if(p1.level == 1){
    		missileCoor.push_back(make_pair(make_pair(beastX,beastY),make_pair(15,true)));
    		velocities.push_back(0);
    	}
    	else if(p1.level == 2){
    		missileCoor.push_back(make_pair(make_pair(beastX,beastY),make_pair(20,true)));
    		velocities.push_back(0);	
    	}
    	else{
    		missileCoor.push_back(make_pair(make_pair(beastX,beastY),make_pair(25,true)));
    		velocities.push_back(0);
    		missileCoor.push_back(make_pair(make_pair(beastX,beastY+50),make_pair(25,true)));
    		velocities.push_back(0);
    	}
    }
}