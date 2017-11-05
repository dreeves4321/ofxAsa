//
//  ofxAsaScene.cpp
//
//  Created by Daniel Reeves on 10/30/16.
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ofxAsaScene.hpp"
#include "ofxAsaObject.hpp"
#include "ofxAsaPrimitive.hpp"


void ofxAsaScene::init(string _name, float _w, float _h, bool _useFBO)
{
	mName = _name;
	mWidth = _w;
	mHeight = _h;
	mUseFBO = _useFBO;
	
	auto me = shared_from_this();
	auto weak_me = weak_ptr<ofxAsaScene>(me);
	
	if (mUseFBO){
		mFBO = make_shared<ofFbo>();
		mFBO->allocate(mWidth, mHeight, GL_RGBA, 4);  // PEND: look into more options for fine tuning this fbo
		
		mFBOHolder = ofxAsaObject::make();
		auto drawf = [weak_me](){
			auto me = weak_me.lock();
			if (me){
				if (me->mFBO){
					me->mFBO->draw(0,0);
				};
			}
		};
		mFBOHolder->setCustomDrawFunc(drawf);
		mFBOHolder->forceTransparency(true);
	}
	
	mZSortManualOffset = ofVec3f(mWidth, mHeight,0).length();
	mSceneAmbient.set(0,0,0,1);
	
	mRoot = ofxAsaObject::make();
	mRoot->setScene(me);
}

ofxAsaScene::~ofxAsaScene(){
	
}

void ofxAsaScene::finishInit(){
	for (auto &l : mLights){
		l.first->disable();
	}
}

bool ofxAsaScene::drawFBO(float x, float y){
	if (!mUseFBO) return false;
	
	mFBO->draw(x,y);
	return true;
}

/*
void ofxAsaScene::addFBOHolderToRender(shared_ptr<ofxAsaObject> _obj){
	mFBOHoldersToRender.push_back(_obj);
}

//void ofxAsaScene::removeFBOToRender(ofFbo* _fbo){
//	mFBORecsToRender.erase(_fbo);
//}
void ofxAsaScene::removeFBOToRender(shared_ptr<ofxAsaObject> _obj){
	auto iter = std::find(mFBOHoldersToRender.begin(), mFBOHoldersToRender.end(), _obj);
	mFBOHoldersToRender.erase(iter);
}
*/

void ofxAsaScene::update(float _t, float _dt){
	// Inherit down from root and populate sorting lists
	mRoot->sortAndInherit(_t, _dt);
}

void ofxAsaScene::drawScene(){
	
	
	bool lightsOn = false;
	if (!mLightsPaused){
		for (auto &l : mLights){
			if (l.second == true){
				l.first->enable();
				lightsOn = true;
			}
		}
	}
	if (mAmbientSet) ofSetGlobalAmbientColor(mSceneAmbient);
	if (lightsOn || mAmbientSet) ofEnableLighting();
	else ofDisableLighting();
	
	
	// before sorting, use the sort after list to assign the locations.
	float dz = 0.01;
	map<shared_ptr<ofxAsaObject>, float> newValues;
	for (auto walker = mSortAfterMap.begin(); walker != mSortAfterMap.end(); ++walker){
		auto before = walker->first;
		if (before){
			float beforevalue = before->getZSortedValue();
			for (auto &w : walker->second){
				auto after = w.lock();
				if (after){
					// compute new value for each "after" object if necessary
					newValues[after] = beforevalue - dz;
				}
			}
		}
	}
	// go through transparent list and change each object if in this new "after" vector
	if (newValues.size()>0){
		for (auto walker = mTransparentList.begin(); walker != mTransparentList.end(); ++walker){
			auto o = walker->second;
			if (newValues.count(o)>0){
				float newval = newValues[o];
				if (walker->first > newval)
					walker->first = newval;
			}
		}
	}
	
	auto zsortdown = [](std::pair<float, shared_ptr<ofxAsaObject>> &left, std::pair<float, shared_ptr<ofxAsaObject>> &right){
		return (left.first < right.first);
	};
	auto zsortup = [](std::pair<float, shared_ptr<ofxAsaObject>> &left, std::pair<float, shared_ptr<ofxAsaObject>> &right){
		return (left.first > right.first);
	};
	std::sort(mOpaqueList.begin(), mOpaqueList.end(), zsortdown);
	std::sort(mTransparentList.begin(), mTransparentList.end(), zsortup);
	
	
	///-- Now do drawing
	if (mUseFBO){
		mFBO->begin();
		ofClear(ofxAsaApp::sApp->getBackground());
	}
	
	if (mUseCamera) mActiveCamera->begin();

	ofEnableDepthTest();
	
	mBlend.mBlendingEnabled = false;
	for (auto &o:mOpaqueList){
		mBlend.enact();
		o.second->doDraw();
	}
	mBlend.mBlendingEnabled = true;
	for (auto &o:mTransparentList){
		mBlend.enact();
		o.second->doDraw();
	}
	
	if (mUseCamera) mActiveCamera->end();
	
	if (mUseFBO){
		mFBO->end();
	}
	
	/// Clear sorting lists
	mOpaqueList.clear();
	mTransparentList.clear();
	mSortAfterMap.clear();

	// turn off the lights
	for (auto &l : mLights){
		l.first->disable();
	}
	ofDisableLighting();
	
	
	// draw attached fbo objects
	/*if (mFBOHoldersToRender.size()>0){
		ofDisableDepthTest();
	
		mBlend.mBlendingEnabled = true;
		mBlend.enact();
		if (!mCompositingCamera){
			// make a default compositing camera
			
			auto camera = new ofCamera();
			float zpos = 10;
			float shift = 1;
			camera->setPosition(.5*mWidth, 0.5*mHeight, zpos);
			camera->enableOrtho();
			camera->setNearClip(shift);
			camera->setFarClip(zpos + 2*shift);
			camera->setVFlip(true);
			setCompositingCamera(camera);
		}
		
		mCompositingCamera->begin();
		for (auto &p: mFBOHoldersToRender){
			p->doDraw();
		}
		mCompositingCamera->end();
	}
	 */
}


void ofxAsaScene::addLight(shared_ptr<ofLight> _light){
	//check to make sure it's not already here
	for (auto &p: mLights){
		if (p.first == _light) return;
	}
	
	mLights.push_back(make_pair(_light, true));
}

void ofxAsaScene::activateLight(shared_ptr<ofLight>_light, bool _on){
	for (auto &p: mLights){
		if (p.first == _light) p.second = _on;
	}
}

void ofxAsaScene::pauseLights(){
	mLightsPaused = true;
}

void ofxAsaScene::resumeLights(){
	mLightsPaused = false;
}

void ofxAsaScene::setAmbient(ofFloatColor _color){
	mSceneAmbient = _color;
	mAmbientSet = true;
}

void ofxAsaScene::addCamera(shared_ptr<ofCamera> _camera){
	mCameras.push_back(_camera);
}


void ofxAsaScene::useCamera(bool _on, shared_ptr<ofCamera> _camera){
	mUseCamera = _on;
	if (mCameras.size() < 1) mUseCamera = false;
	if (mUseCamera){
		if (_camera == nullptr){
			mActiveCamera = mCameras[0];
		}
		else{
			mActiveCamera = _camera;
		}
		auto camp = mActiveCamera->getPosition();
		ofVec3f diff = camp - ofVec3f(mWidth,mHeight,0);
		mZSortManualOffset = diff.length();
	}
	else{
		mZSortManualOffset = ofVec3f(mWidth, mHeight,0).length();
	}
	
	
}

shared_ptr<ofCamera> ofxAsaScene::getActiveCamera(){
	return mActiveCamera;
}

void ofxAsaScene::registerToSortedList(float _z, shared_ptr<ofxAsaObject> _obj, bool _transparent){
	
	if (_transparent){
		mTransparentList.push_back(std::make_pair(_z, _obj));
	}else
	{
		mOpaqueList.push_back(std::make_pair(_z, _obj));
	}
}

void ofxAsaScene::addToSortAfter(shared_ptr<ofxAsaObject> _before, weak_ptr<ofxAsaObject>_after){
	
	mSortAfterMap[_before].push_back(_after);
}


shared_ptr<ofCamera> ofxAsaScene::addCompositingCamera(){
	auto camera = make_shared<ofCamera>();
	float zpos = 10;
	float shift = 1;
	camera->setPosition(.5*mWidth, 0.5*mHeight, zpos);
	camera->enableOrtho();
	camera->setNearClip(shift);
	camera->setFarClip(zpos + 2*shift);
	camera->setVFlip(true);
	
	addCamera(camera);
	useCamera(true, camera);
	return camera;
}

void ofxAsaScene::setupDefaultCameraAndLights(){
	
	float zpos = mWidth; // set back camera one width
	auto light = make_shared<ofLight>();
	light->setDiffuseColor(ofFloatColor(1.f, 1.f, 1.f) );
	light->setAmbientColor(0.1 * ofFloatColor(1.f,1.f,1.f));
	light->setSpecularColor( 0.1 * ofFloatColor(1.f, 1.f, 1.f));
	
	light->setPosition(mWidth*.5, mHeight*0.5, zpos);
	addLight(light);
	
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
	
	addCamera(camera);
	useCamera(true);

}

