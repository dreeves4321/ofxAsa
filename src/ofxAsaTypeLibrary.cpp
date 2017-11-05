//
//  ofxAsaTypeLibrary.cpp
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

#include "ofxAsaTypeLibrary.hpp"

shared_ptr<ofxAsaTypeLibrary> ofxAsaTypeLibrary::sTypeLibrary = nullptr;

void ofxAsaTypeLibrary::init(){
	sTypeLibrary = shared_from_this();
	
	mFolder = "fonts/";
	//ofTrueTypeFont::setGlobalDpi(72);
}

ofxAsaTypeLibrary::~ofxAsaTypeLibrary(){
	sTypeLibrary = nullptr;
	
	for (auto &f:mFaces){
		delete f.second;
	}
}

void ofxAsaTypeLibrary::addFace(string _name, string _path){
	if (!sTypeLibrary){
		auto lib = new ofxAsaTypeLibrary();
	}
	auto lib = sTypeLibrary;
	
	auto face = new ofxFontStash();
	face->setup(lib->mFolder + _path, 1.0, 2048, true, 8, 4.0);
	lib->mFaces[_name] = face;
}


ofxFontStash* ofxAsaTypeLibrary::getFace(string _name){
	return sTypeLibrary->mFaces[_name];
}

void ofxAsaTypeLibrary::addStyle(string _name, float _pointSize, float _lineSpacing, float _letterSpacing, bool _centered, float _columnW, int _maxLines){
	
	if (!sTypeLibrary){
		auto lib = new ofxAsaTypeLibrary();
	}
	auto lib = sTypeLibrary;
	
	auto s = style();
	s.name = _name;
	s.pointSize = _pointSize;
	s.lineSpacing = _lineSpacing;
	s.letterSpacing = _letterSpacing;
	s.columnWidth = _columnW;
	s.maxLines = _maxLines;
	s.centered = _centered;
	
	lib->mStyles[_name] = s;
}

ofxAsaTypeLibrary::style ofxAsaTypeLibrary::getStyle(string _name){
	return sTypeLibrary->mStyles[_name];
}

