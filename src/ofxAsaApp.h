//  ofxAsaApp.h
//
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
Use:
 - Make your ofApp a subclass of ofxAsaApp
 - Must define setup(), update(), and draw()
	- setup() should call ofxAsaSetup(), as well as construct all content
	- update() must call ofxAsaApp::update(). It should do any other per loop updating as well
	- draw() must call ofxAsaApp::draw()
 - Should define setupScenes() and setupFonts()
	- setupScenes() adds scenes with cameras, lights, viewports, and object roots to the overall scene
	- setupFonts() adds styles and faces to the type library
 - When constructing content, add objects as children of the scenes' roots.
 
*/

#pragma once

#include "ofMain.h"

class ofxAsaObject;
class ofxAsaScene;

class ofxAsaApp : public ofBaseApp{

public:
	ofxAsaApp();
	
	/// OF Required
	virtual void setup();
	virtual void update();
	virtual void draw();

	/// Scene management
	void addScene(string _name, shared_ptr<ofxAsaScene> _scene, bool _addToList = true);
	void setSceneOrder(vector<string> _names); // set the order of rendering
	shared_ptr<ofxAsaScene>	getScene(string _name); // return scene by name
	void removeScene(string _name);

	
	/// GL fog  -- for now, this is app based, should be scene based
	void setupFog(ofVec4f _color, float density, float start, float end, GLint fogMode = GL_EXP);
	void enableFog(bool _enable);
	bool getFogEnabled(){return mFogEnabled;};
	
	// Background
	void setBackground(ofFloatColor color){mBackground = color;}
	ofFloatColor getBackground(){return mBackground;}
	
	
protected:
	// suggested methods for setting up components
	virtual void setupFonts(){};
	virtual void setupScenes(){};
	
private:
	void makeDefaultScene(string _name); // makes a default perspective based scene
	void makeWelcomeSign(); // make simple ofxAsa sign
	
public:
	static ofxAsaApp*	sApp; // THE application
	shared_ptr<ofxAsaObject>		mWelcomeSign = nullptr;
	
protected:
	float				mWidth; //dimensions of application
	float				mHeight;
	ofFloatColor			mBackground;
	bool				mShowWelcomeSign = true;
	bool					mInit = false;  // done with intro screen
	float				mWelcomeTime = 6; //time to show intro screen
	bool mReportFPS = true;
	
private:
	bool					mLoaded = false; // switches after first loop
	bool					mFogEnabled = false;
	
	
	// scenes
	map<string, shared_ptr<ofxAsaScene>>	mScenes;
	vector<string>				mSceneOrder;
	
	
};
