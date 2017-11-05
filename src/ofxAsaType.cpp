//
//  ofxAsaType.cpp
//
//  Created by Daniel Reeves on 9/19/16.
//
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ofxAsaType.hpp"
#include "ofxAsaUtilities.hpp"

void ofxAsaType::init(string _text, string _face, string _style, bool _useFBO){
	ofxAsaObject::init();
	
	mText = _text;
	mFont = ofxAsaTypeLibrary::getFace(_face);
	mStyle = ofxAsaTypeLibrary::getStyle(_style);
	mUseFBO = _useFBO;
	
	forceTransparency(true);
	setString(mText);
	
	mDirty = true;
}

ofxAsaType::~ofxAsaType(){
	
}

void ofxAsaType::setString(string _text){
	mText = _text;
	mDirty = true;
	layout(true); // get dimensions, but don't draw
	
	if (mUseFBO){
		initFBO();  //allocate, but don't draw till we need to
	}
}

void ofxAsaType::setUseFBO(bool _use){
	mUseFBO = _use;
	initFBO();
}

void ofxAsaType::initFBO(){
	if (!mUseFBO) return;
	mFBO.allocate(getWidth(), getHeight(false));
}

void ofxAsaType::doIdle(float _t, float _dt){
	if (mUseFBO){
		if (mDirty){
			layout(false);
		}
	}
}

void ofxAsaType::customDraw(){
	
	bool skipDraw = false;
	if (mUseFBO){
		if (!mDirty){
			//mFBO.getTexture().bind();
			//mFBOMesh.draw();
			//mFBO.getTexture().unbind();
			//mFBOtex.draw(mBBox.getTopLeft(), mBBox.getTopRight(), mBBox.getBottomRight(), mBBox.getBottomLeft());
			ofVec3f shift(0, -getAscent(), 0);
			mFBOtex.draw(mBBox.getTopLeft()+shift, mBBox.getTopRight()+shift, mBBox.getBottomRight()+shift, mBBox.getBottomLeft()+shift);
			
		}
		
	}
	else{
		layout(skipDraw);
	}
}


ofRectangle ofxAsaType::layout(bool _skipDraw){
	
	mFont->setLineHeight(mStyle.lineSpacing);
	mFont->setCharacterSpacing(mStyle.letterSpacing);
	
	mFormattedString = mText;
	float y = 0;
	
	if (!_skipDraw && mUseFBO){
		mFBO.begin();
		//ofSetColor(mCombinedMaterial->getDiffuseColor());
		y+= getAscent();
	}
	
	if (mStyle.maxLines == 1){
		mBBox.width = mFont->draw(mFormattedString, mStyle.pointSize, 0, y);
	}
	else{
		mBBox = mFont->drawMultiLineColumn(	mFormattedString,
									   mStyle.pointSize,			//size
									   0, y,		//where
									   mStyle.columnWidth, //column width
									   mNumLines,	//get back the number of lines
									   _skipDraw,		// if true, we wont draw (just get bbox back)
									   mStyle.maxLines,			// max number of lines to draw, crop after that
									   true,		//get the final text formatting (by adding \n's) in the supplied string;													 BE ARWARE that using TRUE in here will modify your supplied string!
									   &mWordsWereCropped, // this bool will be set to true if the box was too small to fit all text
									   mStyle.centered		//centered
									   );
	}
	
	if (!_skipDraw && mUseFBO){
		mFBO.end();
		mFBOtex = mFBO.getTexture();
		//mFBOMesh = tex.getQuad(mBBox.getTopLeft(), mBBox.getBottomLeft(), mBBox.getBottomRight(), mBBox.getTopRight());
		//mFBOMesh = tex.getMeshForSubsection(0,0,0, mBBox.width, mBBox.height,0,0,);
	}
	if (!_skipDraw) mDirty = false;
	
	return mBBox;
}


float ofxAsaType::getWidth(){
	float w = mBBox.width;
	return w;
}

float ofxAsaType::getHeight(bool _includeTopSpace){
	float h = mStyle.lineSpacing*mStyle.pointSize*(mNumLines-1);
	if (_includeTopSpace){
		h+= mStyle.lineSpacing*mStyle.pointSize;
	}
	else{
		h+= mStyle.pointSize;
	}
	
	return h;
}

float ofxAsaType::getAscent(){
	float a = mStyle.pointSize; // pend: include the ascender ratio
	return a;
}
