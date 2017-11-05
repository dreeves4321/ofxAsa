//
//  ofxAsaAnimator.hpp
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

/* 
	Manages animations for objects.
	Currently handles arbitrary lambda functions, and transparency fades
	- Make an instance of ofxAsaAnimator, likely in the App)
	- Create the animation using one of the templates
    - use the returned index to access the animation later on
    - Call update() on the instance of ofxAsaAnimator (likely from the App)
 */



#ifndef ofxAsaAnimator_hpp
#define ofxAsaAnimator_hpp

#include <stdio.h>
#include "ofxAsaObject.hpp"


enum INTERPOLATION{LINEAR};
typedef unsigned int anim_index_t;


class ofxAsaAnimator:
public std::enable_shared_from_this<ofxAsaAnimator>
{
protected:
	ofxAsaAnimator(){};
	void init();
public:
	static shared_ptr<ofxAsaAnimator> make()
	{
		auto o = shared_ptr<ofxAsaAnimator>(new ofxAsaAnimator());
		o->init();
		return o;
	};
	~ofxAsaAnimator();

	static shared_ptr<ofxAsaAnimator> instance(); // get the Animator (or more precisely, the last one made)
	void update(); // call to update all animations
	
	///// Animations
	enum ANIM_TYPE{LAMBDA, ALPHA};
	struct ofxAsaAnimation{
		weak_ptr<ofxAsaObject>	mObject;  // manages lifetime, and is subject of the fade, for example

		anim_index_t			mIndex = 0; //internal index in animator
		float					mStartT = 0;
		float					mDuration = 0;
		INTERPOLATION			mInterp = LINEAR;
		int						mRepeats = 0;
		bool					mLooping = false;
		
		/// derived & record keeping
		float					mFrac = 0;
		
		/// function
		ANIM_TYPE				mType;
		std::function<void(float _t)> mLambda;
		float					mStartValue[4]; //stores up to 4 initial values
		float					mEndValue[4]; // stores up to 4 terminal values
		bool					mInitializeMatchFlag = false; // set the start values to the value of the object at first update.
	};
	////////
	
	// Making annimations and applying to objects
	anim_index_t addAnimation(shared_ptr<ofxAsaAnimation> _animation); // return index
	
	//// Lambda template
	anim_index_t animateLambda(shared_ptr<ofxAsaObject> _obj, float _dur, std::function<void(float _t)> _lam, float _delay = 0, INTERPOLATION _interp=LINEAR);
	
	/// Alpha template
	anim_index_t animateAlpha(shared_ptr<ofxAsaObject> _obj, float _start, float _end, float _dur, float _delay = 0, bool _initialize = false, INTERPOLATION _interp = LINEAR);
	
	
	// Removing and accessing animations by index
	void removeAllAnimations(bool complete = false);
	
	void removeAnimations(shared_ptr<ofxAsaObject> _obj, bool complete = false);
	void removeAnimation(anim_index_t _ind, bool complete = false);
	float getAnimationFraction(anim_index_t _ind);
	shared_ptr<ofxAsaAnimation>  getAnimation(anim_index_t index){return mAnimations[index];};
	
	
private:
	float getInterpolatedValue(float _tfrac, INTERPOLATION _interp);
	anim_index_t increment();
	void resolveAnimation(shared_ptr<ofxAsaAnimation> _animation); // the animation should already have the correct frac value.
	
public:
	static shared_ptr<ofxAsaAnimator>		sAnimator;
	
private:
	int									mIndex = 1;
	map<anim_index_t, shared_ptr<ofxAsaAnimation>>		mAnimations;
	
};
#endif /* ofxAsaAnimator_hpp */
