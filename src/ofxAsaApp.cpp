//  ofxAsaApp.cpp
//
/*
ofxAsa - an addon for OpenFrameworks
 
Copyright (c) 2016 Daniel Reeves

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "ofxAsaApp.h"
#include "ofxAsaObject.hpp"
#include "ofxAsaType.hpp"
#include "ofxAsaScene.hpp"

ofxAsaApp*	ofxAsaApp::sApp = nullptr;

//--------------------------------------------------------------
ofxAsaApp::ofxAsaApp(){
	
}

void ofxAsaApp::setup(){
	sApp = this;
	
	ofSetVerticalSync(true);

	setBackground(ofFloatColor(.1,.1,.1,0));
	mWidth = ofGetWidth();
	mHeight = ofGetHeight();
	
	ofDisableLighting();
	
	auto typeLibrary = ofxAsaTypeLibrary::make();
	typeLibrary->addFace("default", "verdana.ttf");
	typeLibrary->addStyle("default", 32, 1.3, 1.03);
	
	//sRoot = new ofxAsaObject();
	//sRoot->setShown(false);
	
	setupFonts();
	makeDefaultScene("default");
	setupScenes();
	//setupLights();
	//setupCamera();

	//ofEnableNormalizedTexCoords();
	makeWelcomeSign();
	mWelcomeSign->setShown(false);
	
}



///  LOOP  ///
//--------------------------------------------------------------
void ofxAsaApp::update(){
	
	auto t = ofGetElapsedTimef();
	auto dt = ofGetLastFrameTime();

	bool ready = false;
	if (!mLoaded){
		if(mShowWelcomeSign){
			if (!mWelcomeSign->getShown()){
				ofResetElapsedTimeCounter();
				mWelcomeSign->setShown(true);
			}
		}
		else{
			ready = true;
		}
		
		mLoaded = true;
		// finish init on all scenes
		for (auto &s:mScenes){
			s.second->finishInit();
		}
	}
	
	if (!mInit && mShowWelcomeSign && t>mWelcomeTime){
		ready = true;
		mWelcomeSign->setShown(false);
	}
	
	if (ready){
		mInit = true;
		// activate all scenes
		for (auto &s:mScenes){
			s.second->setActive(true);
		}
		ofResetElapsedTimeCounter();
	}
	
	for (auto &s:mSceneOrder){
		if (mScenes.count(s)==0) continue;
		
		auto scene = mScenes[s];
		if (scene->isActive()){
			scene->update(t, dt);
		}
	}
	
}

//--------------------------------------------------------------
void ofxAsaApp::draw(){
	
	auto t = ofGetElapsedTimef();
	auto dt = ofGetLastFrameTime();
	
	// primitive protection against debugging breaking things...
	if (dt>1.0) dt = 1.0;
	
	ofSetBackgroundColor(ofColor(mBackground));
	
	for (auto &s:mSceneOrder){
		if (mScenes.count(s)==0) continue;
		
		auto scene = mScenes[s];
		if (scene->isActive()){
			scene->drawScene();
		}
	}
	
	
	////  FPS reporting
	if (mReportFPS){
		static float fpshistory[20];
		static int j = 0;
		float rate = ofGetFrameRate();
		fpshistory[(j++)%20] = rate;
		float min = fpshistory[0];
		for (int i=0; i< 20; i++){
			if (fpshistory[i]<min) min = fpshistory[i];
		}
		float limit = 50;
		if (rate < limit){
			printf("rate under limit: %f\n", rate);
		}
		ofSetColor(255,255,255,255);
		ofDrawBitmapString("fps: "+ofToString(rate), 10, 10);
		ofDrawBitmapString("min: "+ofToString(min), 10, 40);
	}
}

void ofxAsaApp::setupFog(ofVec4f _color, float density, float start, float end, GLint fogMode){
	
	glFogi(GL_FOG_MODE, fogMode);
	GLfloat fogColor[4] = {_color[0], _color[1], _color[2], _color[3]};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, density);
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
}

void ofxAsaApp::enableFog(bool _enable){
	mFogEnabled = _enable;
	if (_enable){
		glEnable(GL_FOG);
	}
	else{
		glDisable(GL_FOG);
	}
}

//// Scenes
void ofxAsaApp::addScene(string _name, shared_ptr<ofxAsaScene> _scene, bool _addToList){
	mScenes[_name] = _scene;
	_scene->setActive(false);
	if (_addToList) mSceneOrder.push_back(_name);
}

void ofxAsaApp::setSceneOrder(vector<string> _names){
	mSceneOrder = _names;
}

shared_ptr<ofxAsaScene> ofxAsaApp::getScene(string _name){
	if (mScenes.count(_name)==0) return NULL;
	else return mScenes[_name];
}

void ofxAsaApp::removeScene(string _name){
	if (mScenes.count(_name)==0) return;
	auto sc = mScenes[_name];
	
	auto fboh = sc->getFBOHolder();
	
	for (auto &s : mScenes){
		if (fboh){
			// PEND: somehow gracfully remove fbos that have been passed around?  I think to be able to really remove scenes, we'll ahve to switch to shared pointers!
		}
	}
	
	auto iter = std::find(mSceneOrder.begin(), mSceneOrder.end(), _name);
	mSceneOrder.erase(iter);
	
	mScenes.erase(_name);
	
	sc = nullptr;
	
}

void ofxAsaApp::makeDefaultScene(string _name){
	auto scene = ofxAsaScene::make(_name, mWidth, mHeight, false);
	addScene(_name, scene);
	scene->setActive(true);
	
	float zpos = mWidth;
	auto light = make_shared<ofLight>();
	light->setPosition(mWidth*.5, mHeight*0.5, zpos);
	//light->enable();
	scene->addLight(light);
	
	// camera
	auto camera = make_shared<ofCamera>();
	camera->setPosition(mWidth*.5, mHeight*0.5, zpos);
	camera->setVFlip(true);
	ofVec3f upVector, lookAt;
	upVector.set(0, 1, 0);
	lookAt.set(.5*mWidth, 0.5*mHeight, 0);
	camera->setNearClip(1);
	camera->setFarClip(zpos+1);
	camera->lookAt(lookAt, upVector);
	
	scene->addCamera(camera);
	scene->useCamera(true);
}

void ofxAsaApp::makeWelcomeSign(){
	if (mWelcomeSign) return;
	
	auto holder = ofxAsaObject::make();
	mWelcomeSign = holder;
	mWelcomeSign->setPosition(0.5*mWidth, 0.5*mHeight, 1);
	mScenes["default"]->getRoot()->addChild(holder);
	
	holder->setAllColors(1., 1, 1, 0.5);
	holder->mName = "holder";
	
	auto box = make_shared<ofPlanePrimitive>(300, 200, 2, 2);
	box->setPosition(0, 0, 0);
	holder->addPrimitive(box);
	
	auto box2 = make_shared<ofPlanePrimitive>(300, 100, 2, 2);
	box2->setPosition(0, -50, 1);
	holder->addPrimitive(box2);
	
	auto help = ofxAsaType::make("ofxAsa!!!", "default", "default");
	help->setAllColors(0, 1, 1, 1.0);
	help->setPosition(-50, -30, 20);
	holder->addChild(help);
	holder->sortAfterMe(help);
	
	
}