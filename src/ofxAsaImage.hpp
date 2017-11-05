//
//  ofxAsaImage.hpp
//
//  Created by Daniel Reeves on 10/22/16.
/*
 ofxAsa - an addon for OpenFrameworks
 
 Copyright (c) 2016 Daniel Reeves
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
//
//

#ifndef ofxAsaImage_hpp
#define ofxAsaImage_hpp

#include <stdio.h>


#include <stdio.h>
#include "ofxAsaObject.hpp"

class ofxAsaImage:
public ofxAsaObject{
protected:
	ofxAsaImage(){};
	void init(string _path);
public:
	static shared_ptr<ofxAsaImage> make(string _path)
	{
		auto o = shared_ptr<ofxAsaImage>(new ofxAsaImage());
		o->init(_path);
		return o;
	};
	~ofxAsaImage(){};

	void customDraw();
	void load();
	
	shared_ptr<ofImage>		getImage(){return mImage;};
	void			center();
	void			setSize(float w, float h);
	
	
public:
	bool			mLoaded = false;
	shared_ptr<ofImage>		mImage;
	
	ofVec2f			mDrawingSize;
	
};

#endif /* ofxAsaImage_hpp */
