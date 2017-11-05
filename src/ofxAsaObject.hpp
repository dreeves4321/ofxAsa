//
//  ofxAsaObject.hpp
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


//// TEMPLATE FOR FACTORY ///
/*
protected:
	ofxAsaT(){};
	void init(args);
public:
static shared_ptr<ofxAsaT> make(args)
{
	auto o = shared_ptr<ofxAsaT>(new ofxAsaT());
	o->init(args);
	return o;
};
~ofxAsaT();
*/
///////////////////////////

/*
  Base class for scene objects or scene nodes that inherit color, showing, and other properties.
 
  - For custom draw function, you may set mCustomDrawFunc, rather than extending customDraw().
  - To draw of node objects (such as planes) use addPrimitive()
  - turn on and off with setShown(). If hidden will neither show nor process idle function
  - Do processing and updating with doIdle()
  - Have parents and children. Inherit matrices, material properties and hidden status from parents. 
 
 
*/



#ifndef ofxAsaObject_hpp
#define ofxAsaObject_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxAsaScene.hpp"

class ofxAsaObject:
public ofNode, public std::enable_shared_from_this<ofxAsaObject>
{
protected:
	ofxAsaObject(){};
	void init();
public:
	static shared_ptr<ofxAsaObject> make()
	{
		auto o = shared_ptr<ofxAsaObject>(new ofxAsaObject());
		o->init();
		return o;
	};
	~ofxAsaObject();
	
	virtual void customDraw(){if (mCustomDrawFunc) mCustomDrawFunc();};  // draw things that are NOT self, children, or primitives
	virtual void doIdle(float t, float dt);
	
	
	void doDraw();
	void sortAndInherit(float t, float dt); // handle material and inheritence and idling
	void addPrimitive(shared_ptr<ofNode> _prim);
	void clearPrimitives();
	
	
	
	
	
	// showing and hide
	bool		getShown(){return mShown;};
	void		setShown(bool _show){mShown = _show;};
	void		setForceDrawChildren(bool _draw){mForceDrawChildren = _draw;}; //draw children even if I am hidden
	void		setForceIdle(bool _idle){mForceIdle = _idle;}; //idle even when hidden
	
	
	/// GRAPHICS
	void		setCustomDrawFunc(function<void()> _f){mCustomDrawFunc = _f;}
	void		setShader(std::shared_ptr<ofShader> _shader);
	void		setShaderFunc(std::function<void(std::shared_ptr<ofShader>)> _shaderFunc);

	// sorting
	float		getZSortedValue(){return mZSortPosition;};
	float		computeZSortedValue();
	void		setZSortedValue(float _zvalue);
	void		sortAfterMe(shared_ptr<ofxAsaObject> _after);
	
	
	/// Materials
	void		setFilled(bool _filled){mFilled = _filled;};
	void		setMaterial(shared_ptr<ofMaterial> material){mMaterial = material;};
	void		setInheritedMaterial(shared_ptr<ofMaterial> material){mInheritedMaterial = material;};
	void		setShininess(float _shine, bool _inherit);
	void		setColor(float r, float g, float b, float a);

	
	void		setAllColors(float r, float g, float b, float a, bool _diff = true, bool _amb = true, bool _spec = false, bool _emiss = false);
	void		setAllColors(ofFloatColor _color, bool _diff = true, bool _amb = true, bool _spec = false, bool _emiss = false);
	void		setAllColors(ofColor _color, bool _diff = true, bool _amb = true, bool _spec = false, bool _emiss = false);
	void		setAlpha(float _alpha);
	
	void		forceTransparency(bool _transparent){mForcedTransparent = _transparent;};
	void		setAlphaInherit(bool _in){ mInheritAlpha = _in;};
	void		setColorInherit(bool _in){mInheritColor = _in;};
	bool		getAlphaInherit(){return mInheritAlpha;};
	bool		getColorInherit(){return mInheritColor;};

	shared_ptr<ofMaterial> getMaterial(){return mMaterial;};

	// Blending
	void		setOverrideBlending(bool _over){mOverrideBlending = _over;}
	void		setBlending(ofxAsaBlend _blend){mOwnBlend = _blend;}
	void		useOFBlendMode(ofBlendMode _blendmode);
	
	// lighting
	void		useOwnLights(bool _useLights){mUseOwnLights = _useLights;};
	void		setOwnLights(std::vector<ofLight*> _lights);

	
	/// parents and children
	void		clearParent();
	void		setParent(shared_ptr<ofxAsaObject> _parent);
	weak_ptr<ofxAsaObject>	getParent(){return mParent;};
	void		clearChild(shared_ptr<ofxAsaObject> _child);
	void		addChild(shared_ptr<ofxAsaObject> _child);
	void		clearAllChildren();
	
	
	//// SCENES
	void		setScene(shared_ptr<ofxAsaScene> _scene, bool _inherit = true);
	weak_ptr<ofxAsaScene>	getScene(){return mScene;}
	
	
	/// textures -- untested!!!
	bool		getIsTextured(){return mIsTextured;}
	void		setTexture(ofTexture* _tex);
	ofTexture*	getTexture(){return mTexture;}

	

private:
	shared_ptr<ofMaterial>  combineMaterial(shared_ptr<ofMaterial> _m0, shared_ptr<ofMaterial> _m1);
	void drawPrimitives(); // draw things that are NOT asaObjects (this can be elliminated if everything is an asaObject).	void addPrimitive(of3dPrimitive* _prim);
	void activateBlend();
	
public:
	std::string		mName;
	
protected:
	shared_ptr<ofMaterial>  mMaterial;
	shared_ptr<ofMaterial>  mInheritedMaterial;
	shared_ptr<ofMaterial>  mCombinedMaterial;
	
	
private:
	weak_ptr<ofxAsaScene>	mScene;
	
	bool		mShown = true;
	bool        mForceDrawChildren = false;
	bool		mForceIdle = false;
	bool		mFilled = true;
	bool		mInheritAlpha = true;
	bool		mInheritColor = true;
	bool		mForcedTransparent = false;
	
	vector<shared_ptr<ofxAsaObject>>		mChildren;
	weak_ptr<ofxAsaObject>				mParent;
	
	vector<shared_ptr<ofNode>>	mPrimitives; //also plain nodes
	
	bool		mUseOwnLights = false;
	vector<ofLight*>		mLights;
	
	float		mZSortPosition = 0;
	float		mZSortOverridePos = 0;
	bool		mZSortOverride = false;
	weak_ptr<ofxAsaObject>	mSortAfterMeObj;
	
	bool		mOverrideBlending = false;
	ofxAsaBlend	mOwnBlend;
	
	
	std::weak_ptr<ofShader>	mShader;
	std::function<void(std::shared_ptr<ofShader>)>		mShaderPreDraw;
	
	/// Untested!!!
	bool		mIsTextured = false;
	ofTexture*	mTexture = nullptr;
	
	std::function<void()> mCustomDrawFunc;
};

#endif /* ofxAsaObject_hpp */
