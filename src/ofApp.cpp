#include "ofApp.h"
particle::particle(ofPoint point)
{
	position[0]=point;
	rotateAxis={pow(-1,(int)ofRandom(0,10)),0,pow(-1,(int)ofRandom(0,10))};
}
void particle::update(double time)
{
	radius=position[0].distance(center);
	vec=(position[0]-center)/radius;

	position[1]=vec.rotateRad(time,rotateAxis);
	position[1]*=(radius*cos(time));
	position[1]+=center;
}
void particle::draw()
{
	//pointの描画
	ofSetLineWidth(1);
	ofSetColor(255,255,255,(friendNum+1)*10);
	ofDrawCircle(position[1],3);

	//近いpoint間をつなぐ
	ofSetLineWidth(1);
	ofSetColor(255,255,255,(friendNum+1)*5);
	for(int i=0;i<friends.size();i++)
	{
		ofDrawLine(position[1],friends[i]);
	}
}
void particle::calSecondPosition(ofPoint fontPosition)
{
	center=fontPosition;
}
void particle::swap()
{
	position[0]=position[1];
}
void particle::resetFriend()
{
	friendNum=0;
	friends.clear();
}
void particle::addFriend(ofVec3f position)
{
	friends.push_back(position);
}
void particle::addFriendNum()
{
	friendNum++;
}
ofVec3f particle::getPosition()
{
	return position[1];
}
//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(39);
    ofSetFrameRate(60);
	ofSetWindowTitle("Rise up letter");
 
	for(int i=0;i<(SAMPLE_RESOLUTION - 1)*POINT_SET;i++)
	{
		particle Particle(ofPoint(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight())));
		particles.push_back(Particle);
	}

	font.loadFont("fonts/Kazesawa-Bold.ttf", this->FONT_SIZE, true, true, true);
	getWordPoints((string)wordList[(int)ofRandom(0,25)]);
}
//--------------------------------------------------------------
void ofApp::update()
{
	//各ポイントの位置の更新
	for (int i = 0; i < (SAMPLE_RESOLUTION -1)*POINT_SET; i++) 
	{
		particles[i].update(dTime[0]);
		particles[i].resetFriend();
	}

	//近隣のpointはつなげる
	for (int i = 0; i < (SAMPLE_RESOLUTION -1)*POINT_SET-1; i++)
	{
		for (int j = i+1; j < (SAMPLE_RESOLUTION -1)*POINT_SET; j++)
		{
			if(particles[i].getPosition().distance(particles[j].getPosition())<DISTANCE)
			{
				particles[i].addFriend(particles[j].getPosition());	
				particles[i].addFriendNum();
				particles[j].addFriendNum();
			}
		}
	}

	//3.14ごとに文字の変更
	dTime[0]=(ofGetElapsedTimef()-dTime[1])*SPEED;
	if(dTime[0]>=3.14)
	{
		dTime[1]=ofGetElapsedTimef();
		for (int i = 0; i < (SAMPLE_RESOLUTION -1)*POINT_SET; i++) 
		{
			particles[i].swap();
		}
		getWordPoints((string)wordList[(int)ofRandom(0,25)]);
	}
}
//--------------------------------------------------------------
/// 各ポイントの描画処理
void ofApp::draw()
{
	for (int i = 0; i < (SAMPLE_RESOLUTION -1)*POINT_SET; i++) 
	{
		particles[i].draw();
	}	
}

///新しい文字の取得
void ofApp::getWordPoints(string input)
{
	points.clear();
	ofPoint word_size(this->font.stringWidth(input), this->font.stringHeight(input));
	vector<ofPath> word_path = this->font.getStringAsPoints(input, true, false);
	for (int word_index = 0; word_index < word_path.size(); word_index++) 
	{
		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) 
		{
			outline[outline_index] = outline[outline_index].getResampledByCount(SAMPLE_RESOLUTION );
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {
 
				ofPoint point(vertices[vertices_index].x, vertices[vertices_index].y, vertices[vertices_index].z);
				point += ofPoint(word_size.x * -0.5, word_size.y * 0.5);
				point += ofPoint(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
				points.push_back(point);
			}
		}
	}

	//新しい移動先の計算
	for (int i = 0; i < (SAMPLE_RESOLUTION -1)*POINT_SET; i++) 
	{
		particles[i].calSecondPosition(points[i%(points.size()-1)]);
	}
}

