//
//  ofxAsaObject.cpp
//
//  Created by Daniel Reeves on 9/15/16.
//
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ofxAsaObject.hpp"
#include "ofxAsaApp.h"

void ofxAsaObject::init(){
	
	mMaterial = make_shared<ofMaterial>();
	mInheritedMaterial = make_shared<ofMaterial>();
	mCombinedMaterial = make_shared<ofMaterial>();
	
	auto color = ofFloatColor(1.,1.,1.,1.);
	setAllColors(color, true, true, true, false);
	
	mInheritedMaterial->setAmbientColor(color);
	mInheritedMaterial->setSpecularColor(color);
	mInheritedMaterial->setDiffuseColor(color);
	
	mInheritedMaterial->setEmissiveColor(ofFloatColor(0,0,0,1));
	mMaterial->setEmissiveColor(ofFloatColor(0,0,0,1));
	
}

ofxAsaObject::~ofxAsaObject(){
	clearAllChildren();
	clearPrimitives();
}

void ofxAsaObject::doDraw(){
	// draw myself
	
	if (mShown){
		if (!mFilled){
			ofNoFill();
		}
		else{
			ofFill();
		}
		
		auto scene = mScene.lock();
		if (mUseOwnLights && ofGetLightingEnabled()){
			
			if (scene) scene->pauseLights();
			for (auto &l:mLights) l->enable();
		}
		
		activateBlend();
		
		mCombinedMaterial->begin();
		ofSetColor(mCombinedMaterial->getDiffuseColor());
		
		auto shader = mShader.lock();
		if (shader){
			shader->begin();
			if (mShaderPreDraw) mShaderPreDraw(shader);
		}
		
		if (mIsTextured) mTexture->bind();
		
		draw(); // compute my matrix, draw myself, call customDraw();
		drawPrimitives();
		
		if (mIsTextured) mTexture->unbind();
		
		if (shader) shader->end();
		
		mCombinedMaterial->end();
		
		if (scene && mUseOwnLights && ofGetLightingEnabled()){
			scene->resumeLights();
			for (auto &l:mLights) l->disable();
		}
	}

}

void ofxAsaObject::activateBlend(){
	if (mOverrideBlending){
		mOwnBlend.enact();
	}
}

float ofxAsaObject::computeZSortedValue(){
	float z = 0;
	auto scene = mScene.lock();
	if (!scene) return 0;
	if (mZSortOverride){
		float offset = scene->getZSortManualOffset();
		z = offset - mZSortOverridePos;
	}
	else{
		auto cam = scene->getActiveCamera();
		auto pos = getGlobalPosition();
		if (cam && !cam->getOrtho()){
			ofVec3f diff = cam->getPosition() - pos;
			z =  diff.length();
		}
		else{
			z =  -pos[2];
		}
	}
	return z;
}

void ofxAsaObject::setZSortedValue(float _zvalue){
	mZSortOverridePos = _zvalue;
	mZSortOverride = true;
};

void ofxAsaObject::sortAfterMe(shared_ptr<ofxAsaObject> _after){
	if (_after) mSortAfterMeObj = weak_ptr<ofxAsaObject>(_after);
	else mSortAfterMeObj.reset();
}



void ofxAsaObject::sortAndInherit(float t, float dt){
	auto scene = mScene.lock();
	if (!scene) return;
	
	auto me = shared_from_this();
	//auto weak_me = weak_ptr<ofxAsaObject>(me);
	
	// do inheritence
	mCombinedMaterial = combineMaterial(mMaterial, mInheritedMaterial);
	
	// register myself to the sorting lists
	bool transparent = mForcedTransparent;
	if (mCombinedMaterial->getDiffuseColor()[3]<1.0  || mCombinedMaterial->getAmbientColor()[3]<1.0) transparent = true;
	
	// simple z sorting.  PEND: use better distance sorting
	float z = computeZSortedValue();
	mZSortPosition = z;
	
	if (transparent){
		scene->registerToSortedList(z, me, true);
	}
	else{
		scene->registerToSortedList(z, me, false);
	}
	auto sortafter = mSortAfterMeObj.lock();
	if (sortafter){
		scene->addToSortAfter(me, mSortAfterMeObj);
	}
	else{
		mSortAfterMeObj.reset();
	}

	
	if (mShown || mForceDrawChildren){
		for (auto c: mChildren){
			c->setInheritedMaterial(mCombinedMaterial);
			c->sortAndInherit(t, dt);
		}
	}

	if (mShown || mForceIdle) doIdle(t, dt);
	
}

void ofxAsaObject::doIdle(float t, float dt){
	
}


void ofxAsaObject::drawPrimitives(){
	for (auto p:mPrimitives) p->draw();
}

void ofxAsaObject::addPrimitive(shared_ptr<ofNode> _prim){
	mPrimitives.push_back(_prim);
	_prim->ofNode::setParent(*this);
}

void ofxAsaObject::clearPrimitives(){
	for (auto &p : mPrimitives){
		p->ofNode::clearParent();
	}
	mPrimitives.clear();
}

/// parents and children
void ofxAsaObject::clearParent(){
	auto parent = mParent.lock();
	if (!parent) return;
	auto me = shared_from_this();
	parent->clearChild(me);
	mParent.reset();
	
	ofNode::clearParent();
	mScene.reset();
}

void ofxAsaObject::setParent(shared_ptr<ofxAsaObject> parent){
	if (!parent) clearParent();
	mParent = weak_ptr<ofxAsaObject>(parent);
	auto me = shared_from_this();
	parent->addChild(me);
	ofNode::setParent(*parent);
	auto pscenew = parent->getScene();
	auto pscene = pscenew.lock();
	if (pscene) setScene(pscene);
}

void ofxAsaObject::clearChild(shared_ptr<ofxAsaObject> _child){
	for (auto w = mChildren.begin(); w!=mChildren.end(); ){
		if ((*w)==_child){
			w = mChildren.erase(w);
			_child->clearParent();
			break;
		}
		else{
			++w;
		}
	}
}

void ofxAsaObject::addChild(shared_ptr<ofxAsaObject> _child){
	if ( std::find(mChildren.begin(), mChildren.end(), _child) ==mChildren.end())
	{
		auto me = shared_from_this();
		
		mChildren.push_back(_child);
		_child->setParent(me);
	}
}

void ofxAsaObject::clearAllChildren(){
	while (mChildren.size()>0){
		clearChild(mChildren.front());
	}
}

void ofxAsaObject::setScene(shared_ptr<ofxAsaScene> _scene, bool _inherit){
	if (!_scene) return;
	mScene = weak_ptr<ofxAsaScene>(_scene);
	if (_inherit){
		for (auto &c:mChildren) c->setScene(_scene, true);
	}
}

/// lights and materials
void ofxAsaObject::setOwnLights(std::vector<ofLight *> _lights){
	mLights = _lights;
	useOwnLights(true);
}

shared_ptr<ofMaterial> ofxAsaObject::combineMaterial(shared_ptr<ofMaterial> m0, shared_ptr<ofMaterial> m1){

	auto func = [this](ofFloatColor fa, ofFloatColor fb) {
		
		ofFloatColor result(fa);
		bool mixC = mInheritColor;
		bool mixA = mInheritAlpha;
		if (mixC){
			result.r = fa.r * fb.r;
			result.g = fa.g * fb.g;
			result.b = fa.b * fb.b;
		}
		if (mixA) result.a = fa.a * fb.a;
		
		return result;
	};
	
	auto result = make_shared<ofMaterial>();
	auto difCol =  func(m0->getDiffuseColor(), m1->getDiffuseColor());
	auto specCol = func(m0->getSpecularColor() , m1->getSpecularColor());
	auto emmisCol = func(m0->getEmissiveColor() ,m1->getEmissiveColor());
	auto ambientCol =func(m0->getAmbientColor() , m1->getAmbientColor());
	
	result->setColors(difCol, ambientCol, specCol, emmisCol);
	return result;
}

void ofxAsaObject::useOFBlendMode(ofBlendMode _blendmode){
	mOverrideBlending = true;
	mOwnBlend.mUseCustomBlend = false;
	mOwnBlend.mBlendMode = _blendmode;
}

void ofxAsaObject::setShininess(float _s, bool _inherit){
	mMaterial->setShininess(_s);
	if (_inherit){
		for (auto &c : mChildren){
			c->setShininess(_s, _inherit);
		}
	}
}

void ofxAsaObject::setColor(float r, float g, float b, float a){
	setAllColors(ofFloatColor(r,g,b,a), true, true, false, false);
}


void ofxAsaObject::setAllColors(float r, float g, float b, float a, bool _diff, bool _amb, bool _spec, bool _emiss){
	setAllColors(ofFloatColor(r,g,b,a), _diff, _amb, _spec, _emiss);
}

void ofxAsaObject::setAllColors(ofColor _color, bool _diff, bool _amb, bool _spec, bool _emiss){
	if (_amb) mMaterial->setAmbientColor(_color);
	if (_emiss) mMaterial->setEmissiveColor(_color);
	if (_spec) mMaterial->setSpecularColor(_color);
	if (_diff) mMaterial->setDiffuseColor(_color);
	
}

void ofxAsaObject::setAllColors(ofFloatColor _color, bool _diff, bool _amb, bool _spec, bool _emiss){
	int r = 255.*_color[0];
	int g = 255.*_color[1];
	int b = 255.*_color[2];
	int a = 255.*_color[3];
	
	setAllColors( ofColor(r,g,b,a), _diff, _amb, _spec, _emiss);
}

void ofxAsaObject::setAlpha(float _alpha){
	auto color = mMaterial->getDiffuseColor();
	color[3] = _alpha;
	mMaterial->setDiffuseColor(color);
}

/// Shaders
void ofxAsaObject::setShader(std::shared_ptr<ofShader> _shader){
	mShader = weak_ptr<ofShader>(_shader);
}

void ofxAsaObject::setShaderFunc(std::function<void(std::shared_ptr<ofShader>)> _func){
	mShaderPreDraw = _func;
}

/// Textures
void ofxAsaObject::setTexture(ofTexture* _tex){
	mTexture = _tex;
	mForcedTransparent = true;
	if (_tex == NULL){
		mIsTextured = false;
	}
	else{
		mIsTextured = true;
	}
}
