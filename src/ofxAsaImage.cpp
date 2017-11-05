//
//  ofxAsaImage.cpp
//
//  Created by Daniel Reeves on 10/22/16.
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ofxAsaImage.hpp"


void ofxAsaImage::init(string _path){
	ofxAsaObject::init();
	
	mImage = make_shared<ofImage>(_path);
	if (mImage->getWidth()>0) load();
}


void ofxAsaImage::load(){
	forceTransparency(true);
	mLoaded = true;
	mDrawingSize.set(mImage->getWidth(), mImage->getHeight());
}

void ofxAsaImage::customDraw(){
	if (!mLoaded) return;
	
	mImage->draw(0,0, mDrawingSize[0], mDrawingSize[1]);
}

void ofxAsaImage::center(){
	mImage->setAnchorPercent(0.5,0.5);
}

void ofxAsaImage::setSize(float w, float h){
	mDrawingSize.set(w,h);
}