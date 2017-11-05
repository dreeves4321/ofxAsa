//
//  ofxAsaUtilities.hpp
//
//  Created by Daniel Reeves on 9/28/16.
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
  A simple set of utilities. Should probably just be a namespace. There's really no reason to ever instantiate an object.
 */

#ifndef ofxAsaUtilities_hpp
#define ofxAsaUtilities_hpp

#include <stdio.h>
#include "ofxAsaObject.hpp"
#include "ofxJSONElement.h"

class ofxAsaUtilities
{
public:
	ofxAsaUtilities();
	~ofxAsaUtilities();
	
	static Json::Value readJsonFromFile(std::string _file);
 
	static ofVec4f interpColors(float _val, float _val0, ofVec4f _col0, float _val1, ofVec4f _col1);

	static std::pair<float,float>   getMinMaxOfVerts(vector<ofVec3f> _vecs, int _dim);
	
	
private:
	
	
public:
	static ofxAsaUtilities*  sUtilities;

private:
	
};

#endif /* ofxAsaUtilities_hpp */
