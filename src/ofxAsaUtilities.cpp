//
//  ofxAsaUtilities.cpp
//
//  Created by Daniel Reeves on 9/28/16.
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ofxAsaUtilities.hpp"
#include "ofxAsaApp.h"
#include "ofxAsaScene.hpp"
#include <cstdio>

ofxAsaUtilities* ofxAsaUtilities::sUtilities = nullptr;

ofxAsaUtilities::ofxAsaUtilities(){
	if (sUtilities) delete sUtilities;
	sUtilities = this;
	
}

ofxAsaUtilities::~ofxAsaUtilities(){
	sUtilities = nullptr;
}

Json::Value ofxAsaUtilities::readJsonFromFile(std::string _file){
	
	ofxJSONElement element;
	element.open(_file);
	
	return element;
}

ofVec4f ofxAsaUtilities::interpColors(float _val, float _val0, ofVec4f _col0, float _val1, ofVec4f _col1){
	
	//interpolate colors through white/grey
	ofVec4f result;
	
	ofFloatColor col;
	float sat;
	float midval = 0.5*(_val0+_val1);
	
	if (_val < midval){
		sat = (midval - _val)/(midval - _val0);
		col = ofFloatColor(_col0[0], _col0[1], _col0[2], _col0[3]);
	}
	else{
		sat = (_val - midval)/(_val1 - midval);
		col = ofFloatColor(_col1[0], _col1[1], _col0[2], _col0[3]);
	}
	col.setSaturation(sat);
	result.set(col[0],col[1],col[2],col[3]);
	return result;
}


std::pair<float,float> ofxAsaUtilities::getMinMaxOfVerts(vector<ofVec3f> _vecs, int _dim)
{
	float min = 1000000;
	float max = -1000000;
	
	for (int i=0; i< _vecs.size(); i++){
		float val = _vecs[i][_dim];
		if (val>max) max = val;
		if (val<min) min = val;
	}

	return std::make_pair(min,max);
}
