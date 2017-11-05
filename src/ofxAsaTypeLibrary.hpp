//
//  ofxAsaTypeLibrary.hpp
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
//// Dependencies:  ofxFontStash

/*
	Collection of type faces and style parameters for making ofxAsaType objects.
     - make a library in the App
     - Add styles and type faces with identifying names
     - addStyle(
		string (identifying name),
		float (point size of type),
		float (linespace as multiplicative factor),
		float (letterspacing as multiplicative factor),
		bool (centered or not),
		float (column width),
		int (maximum lines of text before curtailed)
		)
	- addFace(
		string (identifying name),
		string (path to font file)
		)
 
*/


#ifndef ofxAsaTypeLibrary_hpp
#define ofxAsaTypeLibrary_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxFontStash.h"

using namespace std;

class ofxAsaTypeLibrary:
public enable_shared_from_this<ofxAsaTypeLibrary>
{
protected:
	ofxAsaTypeLibrary(){};
	void init();
public:
	static shared_ptr<ofxAsaTypeLibrary> make()
	{
		auto o = shared_ptr<ofxAsaTypeLibrary>(new ofxAsaTypeLibrary());
		o->init();
		return o;
	};
	~ofxAsaTypeLibrary();
	
	static void addFace(string _name, string _path);
	static ofxFontStash* getFace(string _name);
	
	struct style{
		string name;
		float pointSize = 26;
		float lineSpacing = 1.2;
		float letterSpacing = 1.02;
		
		float columnWidth = 1024;
		int		maxLines = 20;
		bool	centered = false;
	};
	
	static void addStyle(string _name, float _pointSize, float _lineSpacing, float _letterSpacing, bool _centered = false, float _columnWidth = 2048, int _maxLines = 20);
	static style getStyle(string _name);
	
	
	
public:
	static shared_ptr<ofxAsaTypeLibrary> sTypeLibrary;
	
private:
	string						mFolder;
	
	map<string, ofxFontStash*>		mFaces;
	map<string, style>				mStyles;
};


#endif /* ofxAsaTypeLibrary_hpp */
