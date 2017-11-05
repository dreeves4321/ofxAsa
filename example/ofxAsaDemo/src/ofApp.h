#pragma once

#include "ofMain.h"
#include "ofxAsaApp.h"

class ofxAsaConfig;
class ofxAsaAnimator;

class ofApp : public ofxAsaApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
protected:
	void setupFonts();  // inherited
	void setupScenes(); // inherited
	
	void makeContent();
	shared_ptr<ofxAsaObject> makeLabelBox(string _label, ofFloatColor _color);
	
protected:
	shared_ptr<ofxAsaConfig>   mConfig;
	shared_ptr<ofxAsaAnimator>	mAnimator;
	
};
