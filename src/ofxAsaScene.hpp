//
//  ofxAsaScene.hpp
//
//  Created by Daniel Reeves on 10/30/16.
//
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* 
   Represents a universe with lighting, cameras, and objects. Each scene is rendered to the screen, at the position defined, or is rendered to an fbo for pasting as you wish.
 
*/


#ifndef ofxAsaScene_hpp
#define ofxAsaScene_hpp

#include <stdio.h>
#include "ofxAsaApp.h"

class ofxAsaObject;
class ofxAsaPrimitive;


//// ---  Blending Settings --- ///
struct ofxAsaBlend{
	bool mBlendingEnabled = true;
	bool mUseCustomBlend = true;
	std::function<void()> mCustomBlendSetter;
	ofBlendMode		mBlendMode = OF_BLENDMODE_ALPHA;
	
	
	void enact(){
		if (mBlendingEnabled){
			glEnable(GL_BLEND);
		}
		else{
			glDisable(GL_BLEND);
		}
		if (mUseCustomBlend){
			mCustomBlendSetter();
		}
		else{
			ofEnableBlendMode(mBlendMode);
		}
	}
	
	void setCustomToAsaBlending(){
		mCustomBlendSetter = [](){
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		};
	}
	
	ofxAsaBlend(){
		setCustomToAsaBlending();
	}
	
};



/// ---- SCENE --- ///
class ofxAsaScene :
public enable_shared_from_this<ofxAsaScene>
{
protected:
	ofxAsaScene(){};
	void init(string _name, float _w, float _h, bool _useFBO);
public:
	static shared_ptr<ofxAsaScene> make(string _name, float _w, float _h, bool _useFBO = false)
	{
		auto o = shared_ptr<ofxAsaScene>(new ofxAsaScene());
		o->init(_name, _w, _h, _useFBO);
		return o;
	};
	~ofxAsaScene();
	
	
	// these are called automatically by ofxAsaApp if the scene is added to the app
	void update(float _t, float _dt);
	void drawScene();
	void finishInit();
	
	// Turn scene on and off
	bool		isActive(){return mActive;}
	void		setActive(bool _active){mActive = _active;}
	
	// Accessors
	shared_ptr<ofxAsaObject> getRoot(){return mRoot;} // Add your objects as children of mRoot
	string		getName(){return mName;}
	ofVec2f		getSize(){return ofVec2f(mWidth,mHeight);}
	
	/// FBOs (own and to render)
	shared_ptr<ofxAsaObject> getFBOHolder(){return mFBOHolder;} // add the fbo holder to a parent object in a compositing scene to draw this scenes fbo at that location
	bool drawFBO(float x = 0, float y = 0); // draw the fbo diretly to the screen
	shared_ptr<ofFbo> getFBO(){return mFBO;}
	///// I couldn't get these to work right :(
	//void addFBOHolderToRender(shared_ptr<ofxAsaObject> _obj);
	//void removeFBOToRender(shared_ptr<ofxAsaObject> _obj);
	
	/// Drawing and Sorting
	// Default blending for scene. Can be overwritten by individual objects.
	ofxAsaBlend   getBlendSettings(){return mBlend;}
	void		  setDefaultBlendSettings(ofxAsaBlend blend){mBlend = blend;}

	// Sorting
	void registerToSortedList(float _z, shared_ptr<ofxAsaObject> _obj, bool _transparent); // called automatically by ofxAsaObject to sort in the scene
	// manual overrides for sorting
	void setZSortManualOffset(float _val){mZSortManualOffset = _val;};
	float getZSortManualOffset(){return mZSortManualOffset;};
	 // Force to draw an object after a specific object
	void addToSortAfter(shared_ptr<ofxAsaObject> before, weak_ptr<ofxAsaObject> _after);
	
	
	/// Lights
	void addLight(shared_ptr<ofLight> _light);
	void activateLight(shared_ptr<ofLight> _light, bool _on);
	vector<pair<shared_ptr<ofLight>, bool>> getLights(){return mLights;}
	
	void pauseLights();
	void resumeLights();
	void setAmbient(ofFloatColor _color);
	
	/// Cameras
	void addCamera(shared_ptr<ofCamera> _camera);
	void useCamera(bool _on, shared_ptr<ofCamera> _camera= nullptr);
	shared_ptr<ofCamera> getActiveCamera();
	shared_ptr<ofCamera> addCompositingCamera(); //convenience method to make an ortho camera, specifically for adding fbo scenes to this compositing scene.
	
	void setupDefaultCameraAndLights();
	
private:
	
	
public:
	
	
private:
	float mWidth;
	float mHeight;
	
	string mName;
	bool  mUseFBO;
	shared_ptr<ofFbo>	mFBO = nullptr;
	shared_ptr<ofxAsaObject>	mFBOHolder = nullptr;
	//std::vector<shared_ptr<ofxAsaObject>> mFBOHoldersToRender;
		
	shared_ptr<ofxAsaObject>	mRoot;
	bool			mActive = true;
	
	std::vector<pair<shared_ptr<ofLight>, bool>>   mLights;
	bool					mLightsPaused = false;
	ofFloatColor			mSceneAmbient;
	bool					mAmbientSet = false;
	
	std::vector<shared_ptr<ofCamera>>	mCameras;
	shared_ptr<ofCamera>	mActiveCamera = nullptr;
	bool					mUseCamera = false;
	
	ofxAsaBlend				mBlend; // default blending mode
	
	std::vector<std::pair<float, shared_ptr<ofxAsaObject>>>	mOpaqueList;
	std::vector<std::pair<float, shared_ptr<ofxAsaObject>>>	mTransparentList;
	float				mZSortManualOffset;
	std::map<shared_ptr<ofxAsaObject>, vector<weak_ptr<ofxAsaObject>>> mSortAfterMap;
	
};



#endif /* ofxAsaScene_hpp */
