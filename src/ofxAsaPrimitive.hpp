//
//  ofxAsaPrimitive.hpp
//
//  Created by Daniel Reeves on 10/15/16.
//
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/* 
  Imparts the benefits of an ofxAsaObject on an arbitrary ofNode
   -  Make the node, as a shared_ptr
   -  call ofxAsaPrimitive::make(node)
   -  Add the returned object as a child to a parent ofxAsaObject
*/

#ifndef ofxAsaPrimitive_hpp
#define ofxAsaPrimitive_hpp

#include <stdio.h>
#include "ofxAsaObject.hpp"

class ofxAsaPrimitive:
public ofxAsaObject{
protected:
	ofxAsaPrimitive(){};
	void init(shared_ptr<ofNode> _primitive);
public:
	static shared_ptr<ofxAsaPrimitive> make(shared_ptr<ofNode> _primitive)
	{
		auto o = shared_ptr<ofxAsaPrimitive>(new ofxAsaPrimitive());
		o->init(_primitive);
		return o;
	};
	~ofxAsaPrimitive();

	
	void customDraw();
	
public:
	shared_ptr<ofNode>		mPrimitive;
};

#endif /* ofxAsaPrimitive_hpp */
