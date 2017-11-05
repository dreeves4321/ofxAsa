//
//  ofxAsaAnimator.cpp
//
//  Created by Daniel Reeves on 11/24/16.
//
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ofxAsaAnimator.hpp"

shared_ptr<ofxAsaAnimator>		ofxAsaAnimator::sAnimator = nullptr;

void ofxAsaAnimator::init(){
	sAnimator = shared_from_this();
}

ofxAsaAnimator::~ofxAsaAnimator(){
	sAnimator = nullptr;
}

shared_ptr<ofxAsaAnimator> ofxAsaAnimator::instance(){
	return sAnimator;
}

void ofxAsaAnimator::update(){
	float nowT = ofGetElapsedTimef();
	for (auto walker = mAnimations.begin(); walker != mAnimations.end(); ){
		// have we started yet?
		auto anim = walker->second;
		if (anim->mStartT > nowT){
			++walker;
			continue;
		}
		
		// get the interped time
		float tfrac = 1.0;
		while (anim->mRepeats >0 && nowT>anim->mStartT+anim->mDuration){
			anim->mRepeats--;
			anim->mStartT += anim->mDuration;
		}
		while (anim->mLooping && nowT>anim->mStartT+anim->mDuration){
			anim->mStartT += anim->mDuration;
		}

		if (anim->mDuration > 0) tfrac = (nowT - anim->mStartT)/ anim->mDuration;
		bool remove = false;
		if (tfrac > 1.0){
			remove = true;
			anim->mFrac = 1.0;
		}
		else anim->mFrac = getInterpolatedValue(tfrac, anim->mInterp);
		
		// resolve the function
		resolveAnimation(anim);
		
		// remove if old, and continue
		if (remove){
			walker = mAnimations.erase(walker);
		}
		else{
			++walker;
		}
	}
}

anim_index_t ofxAsaAnimator::addAnimation(shared_ptr<ofxAsaAnimation> _animation){
	anim_index_t i = increment();
	_animation->mIndex = i;
	mAnimations[i] = _animation;
	
	return i;
}

anim_index_t ofxAsaAnimator::animateLambda(shared_ptr<ofxAsaObject> _obj, float _dur, std::function<void(float _t)> _lam, float _delay, INTERPOLATION _interp){
	
	auto anim = make_shared<ofxAsaAnimation>();
	
	anim->mType = ANIM_TYPE::LAMBDA;
	anim->mObject = _obj;
	anim->mDuration = _dur;
	anim->mLambda = _lam;
	anim->mStartT = ofGetElapsedTimef() + _delay;
	anim->mInterp=_interp;
	
	return addAnimation(anim);
}

anim_index_t ofxAsaAnimator::animateAlpha(shared_ptr<ofxAsaObject> _obj, float _start, float _end, float _dur, float _delay, bool _initialize , INTERPOLATION _interp){
	
	auto anim = make_shared<ofxAsaAnimation>();
	
	anim->mType = ANIM_TYPE::ALPHA;
	anim->mObject = _obj;
	anim->mInitializeMatchFlag = _initialize;
	anim->mStartValue[0] = _start;
	anim->mEndValue[0] = _end;
	anim->mDuration = _dur;
	anim->mStartT = ofGetElapsedTimef() + _delay;
	anim->mInitializeMatchFlag = _initialize;
	
	return addAnimation(anim);
}

void ofxAsaAnimator::removeAllAnimations(bool complete){
	if (complete){
		for (auto &a : mAnimations){
			a.second->mFrac = 1.0;
			resolveAnimation(a.second);
		}
	}
	mAnimations.clear();
}

void ofxAsaAnimator::removeAnimations(shared_ptr<ofxAsaObject> _obj, bool complete){
	
	for (auto w = mAnimations.begin(); w != mAnimations.end(); ){
		auto o = w->second->mObject.lock();
		if (o && o == _obj){
			if (complete){
				w->second->mFrac = 1.0;
				resolveAnimation(w->second);
			}
			w = mAnimations.erase(w);
		}
		else{
			++w;
		}
	}
}

void ofxAsaAnimator::removeAnimation(anim_index_t _ind, bool complete){
	auto a = mAnimations[_ind];
	auto o = a->mObject.lock();
	if (o && complete){
		a->mFrac = 1.0;
		resolveAnimation(a);
	}
	
	mAnimations.erase(_ind);
}

float ofxAsaAnimator::getAnimationFraction(anim_index_t _ind){
	auto a = mAnimations[_ind];
	return a->mFrac;
}

float ofxAsaAnimator::getInterpolatedValue(float _tfrac, INTERPOLATION _interp){
	
	float frac = 0;
	switch (_interp) {
  case LINEAR:
			frac =  _tfrac;
			break;
			
  default:
			frac =  _tfrac;
			break;
	}
	
	return frac;
}

anim_index_t ofxAsaAnimator::increment(){
	mIndex++;
	return mIndex;
}

void ofxAsaAnimator::resolveAnimation(shared_ptr<ofxAsaAnimator::ofxAsaAnimation> _animation){
	
	// the animation should already have the correct frac value. Do the work now.
	auto o = _animation->mObject.lock();
	if (!o) return;
	
	switch (_animation->mType) {
		case ALPHA:
			{
				float a = _animation->mStartValue[0] + _animation->mFrac*(_animation->mEndValue[0] - _animation->mStartValue[0]);
				o->setAlpha(a);
				break;
			}
			
		case LAMBDA:
		default:
			_animation->mLambda(_animation->mFrac);
	}
}