#include <string>
#include <time.h>
#include <ctime>
#include <cstdlib>
#include <cstdio>

using namespace std;

class Player{
    public:
		int highscore;
		int lives;
		int shots;
		int level;
        string highscorerName;
        Player();
        void setPlayerName(string newname);
        int getScore();
        void setScore(int newscore);
        string getName();
        void readHighScore();
        void writeHighScore();
        virtual ~Player();
    protected:
    private:
        int score;
        string name;
};

Player::Player(){
	level = 1;
	lives = 3;
    score = 0;
    name = "Default";
    highscore = 0;
    highscorerName = "Be the first!";
}

void Player::setPlayerName(string newname){
    score = 0;
    name = newname;
    readHighScore();
}

Player::~Player(){

}

int Player::getScore(){
    return score;
}

void Player::setScore(int newscore){
    score = newscore;
}

string Player::getName(){
    return name;
}

void Player::readHighScore(){
	FILE* fd = fopen("highscore.txt", "r");
	if(fd!=NULL){
		char tempstr[101];
		fscanf(fd, "%100s %d", tempstr, &highscore);
		highscorerName = tempstr;
		fclose(fd);
	}
}

void Player::writeHighScore(){
	FILE* fd = fopen("highscore.txt", "w");
	if(fd!=NULL){
		fprintf(fd, "%s %d", name.c_str(), score);
		fclose(fd);
		readHighScore();
	}
}