#pragma once

#include "ofMain.h"
#include "ofxGui.h"
class particle
{
private:
	double direction;
	ofPoint position[2];
	ofPoint center;
	ofVec3f vec;
	ofVec3f rotateAxis;
	double radius;
	double slope;
	vector<ofVec3f> friends;
	int friendNum;

public:
	
	particle(ofPoint point);
	~particle(){};
	void calSecondPosition(ofPoint fontPosition);
	/*void setup(double radius,double theta,double Speed);
	void firstsetup(double radius, double Speed);*/
	void update(double time);
	void draw();
	void swap();
	void addFriend(ofVec3f position);
	void resetFriend();
	void addFriendNum();
	ofVec3f getPosition();
};
class ofApp : public ofBaseApp{
	public:
	void setup();
	void update();
	void draw();

	void getWordPoints(string input);
 
	ofTrueTypeFont font;
	vector < particle > particles;
	vector < ofPoint > points;
	double dTime[2] = {0,0};

	string wordList[26] 	= {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
	double SPEED			= 1.5;
	double DISTANCE			= 50;
	int FONT_SIZE			= 400;
	int SAMPLE_RESOLUTION 	= 70;
	int POINT_SET			= 5;
};
