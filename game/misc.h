#include "imageloader.h"

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define RAD2DEG(rad) (rad * 180 / PI)
#define ROTATEX(x,y,deg) ((x * cos(DEG2RAD(deg)))-(y*sin(DEG2RAD(deg))))
#define ROTATEY(x,y,deg) ((x * sin(DEG2RAD(deg)))+(y*cos(DEG2RAD(deg))))
#define WIDTH 1300
#define HEIGHT 700

#define WHITE 1.0,1.0,1.0
#define BLACK 0.0,0.0,0.0
#define RED 1.0,0.0,0.0
#define PURPLE 0.5f,0.0f,0.8f
#define GREEN 0.0f,0.75f,0.0f
#define SKY_BLUE 0.7f,0.9f,1.0f


float zoom;
GLuint _textureId,beastTexture; 
float ut = 0.1;

float x1plus = 0,x2plus = 0;

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
			0,                            //0 for now
			GL_RGB,                       //Format OpenGL uses for image
			image->width, image->height,  //Width and height
			0,                            //The border of the image
			GL_RGB, //GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
			//as unsigned numbers
			image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void giveTexture(){
	glColor3f(WHITE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f,0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1600.0f,0.0f,0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1600.0f, 900.0f,0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f,900.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void myOrtho(float X,float Y){
        float x1,x2,y1,y2;
        float width = 1600.0 - zoom;
        float height = (width*9.0)/16.0;
        x1 = X - width/2.0;
        x2 = X + width/2.0;
        y1 = Y - height/2.0;
        y2 = Y + height/2.0;
        float fx1 , fx2, fy1,fy2;
        if(x1<0){
            fx1 = 0.0;
            fx2 = width;
        }
        else if(x2>1600.0){
            fx1 = 1600.0-width;
            fx2 = 1600.0;
        }
        else{
            fx1 = X - (width/2.0);
            fx2 = X + (width/2.0);
        }
        if(y1<0){
            fy1 = 0.0;
            fy2 = height;
        }
        else if(y2>900.0){
            fy1 = 900.0-height;
            fy2 = 900.0;
        }
        else{
            fy1 = Y - (height/2.0);
            fy2 = Y + (height/2.0);
        }
        gluOrtho2D(fx1 + x1plus,fx2 + x2plus,fy1,fy2);
}

void drawCircle(float X , float Y, float r){
	glPushMatrix();
	{
		glTranslatef(X,Y,0);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f( 0 , 0 , 0.0);
		for(int j=0;j<380;j+=5){
			glVertex2f( r*cos((j*PI)/180.0), r*sin((j*PI)/180.0) );
		}
		glEnd();
	}
	glPopMatrix();
}