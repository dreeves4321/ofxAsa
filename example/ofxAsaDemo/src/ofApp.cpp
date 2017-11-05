#include "ofApp.h"

#include "ofxAsaConfig.hpp"
#include "ofxAsaTypeLibrary.hpp"
#include "ofxAsaType.hpp"
#include "ofxAsaAnimator.hpp"
#include "ofxAsaPrimitive.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
	mConfig = ofxAsaConfig::make();
	mAnimator = ofxAsaAnimator::make();
	
	mShowWelcomeSign = true;
	mWelcomeTime = 1.0;
	
	ofxAsaApp::setup();  // calls setupFonts and setupScenes
	
	makeContent();
	

}

void ofApp::setupScenes(){
	///// Demonstrates the modularity of scenes
	
	// make a bottom scene
	auto fullScene = ofxAsaScene::make("base scene", mWidth, mHeight, true);
	fullScene->setupDefaultCameraAndLights();
	addScene("base scene", fullScene);
	
	// make a second right side only
	auto leftScene = ofxAsaScene::make("left scene", 0.5*mWidth, mHeight, true);
	leftScene->setupDefaultCameraAndLights();
	addScene("left scene", leftScene);
	
	
	// make a composite scene
	auto compositeScene = ofxAsaScene::make("composite", mWidth, mHeight, false);
	compositeScene->addCompositingCamera();
	
	auto mainholder = fullScene->getFBOHolder();
	//ofVec2f dashSize = mDashboard->getSize();
	auto leftholder = leftScene->getFBOHolder();
	//float x = mWidth - dashSize[0];
	//float y = mHeight - dashSize[1];
	leftholder->setPosition(0, 0, 1);
	compositeScene->getRoot()->addChild(mainholder);
	compositeScene->getRoot()->addChild(leftholder);
	addScene("composite", compositeScene);
	 
}


void ofApp::update(){
	ofxAsaApp::update();
	mAnimator->update();
	//float t = ofGetElapsedTimef();
	//float dt = ofGetLastFrameTime();
}

void ofApp::draw(){
	ofxAsaApp::draw();
	
}


void ofApp::setupFonts(){
	auto library = ofxAsaTypeLibrary::sTypeLibrary;
	library->addFace("default", "verdana.ttf");
	
	///usage: addStyle(string _name, float _pointSize, float _lineSpacing, float _letterSpacing, bool _centered = false, float _columnWidth = 1024, int _maxLines = 20);
	
	library->addStyle("medium", 28, 1.0, 1.00);
	library->addStyle("big", 70 , 1.0, 1.00);
	
}


void ofApp::makeContent(){
	auto mainroot = getScene("base scene")->getRoot();
	auto leftroot = getScene("left scene")->getRoot();
	
	// Make label for main scene
	auto label = ofxAsaType::make("Main Scene label", "default", "big");
	mainroot->addChild(label);
	label->setAllColors(1.0,0,0,1.0);
	
	int midx = 0.5*mWidth;
	int midy = .5*mHeight;
	label->setPosition(midx - 100, midy, 0);
	
	
	
	
	
	// Make label for right side scene. Note how it cuts off in the middle of the scene, because the framebuffer is pasted in only on the right side
	auto llabel = ofxAsaType::make("Left Side Label: this is totally cool! Weeeeee!!!!", "default", "medium");
	//mainroot->addChild(llabel);
	leftroot->addChild(llabel);
	llabel->setAllColors(0.9,0.9,0,1.0);
		// animate it
	llabel->setPosition(10, midy, 0);
	
	
	auto func = [llabel, midx, midy](float t){
		llabel->setPosition(0.5*midx, midy*(1+cos(2.*3.14*t)),.1);
	};
	int index = mAnimator->animateLambda(llabel, 2, func, 0, LINEAR);
	auto animation = mAnimator->getAnimation(index);
	animation->mLooping=true;
	
	// demonstrate inheritence for coloring and position. Note all positions are relative to parent, like ofNode.
}





//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
