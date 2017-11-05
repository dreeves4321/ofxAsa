//
//  ofxAsaType.hpp
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
/*
  Simple wrapper to apply ofxAsaObject behavior and styling to true type fonts, using ofxFontStash
   - make a type library in the application to define styles and faces
   - call ofxAsaType::make() with the desired string and name of the face and style from the library
   - Use setString to modify the text
   - note: Inefficient in that it must write the text every render.
 
  PEND: add support for fontstash drawmultilinecolumnformatted
 PEND: Adopt an fbo/textured quad approach for performance improvement. The one I have is terrible, don't use it.
*/


#ifndef ofxAsaType_hpp
#define ofxAsaType_hpp

#include <stdio.h>
#include "ofxAsaObject.hpp"
#include "ofxAsaTypeLibrary.hpp"
#include "ofxFontStash.h"

using namespace std;

class ofxAsaType :
public ofxAsaObject {
protected:
	ofxAsaType(){};
	void init(string _text, string _face, string _style, bool _useFBO);
public:
	static shared_ptr<ofxAsaType> make(string _text, string _face, string _style, bool _useFBO = false)
	{
		auto o = shared_ptr<ofxAsaType>(new ofxAsaType());
		o->init(_text, _face, _style, _useFBO);
		return o;
	};
	~ofxAsaType();

public:
	// Access
	void setString(string _text);
	ofxAsaTypeLibrary::style  getStyle(){return mStyle;}
	void					setStyle(ofxAsaTypeLibrary::style style){mStyle = style;};
	
	// Computed dimensions
	float getWidth();
	float getHeight(bool _includeTopSpace = true);
	float getAscent();
	ofRectangle getBBox(){return mBBox;};
	
	// called automatically by scene
	void doIdle(float _t, float _dt);
	void customDraw();

	/// FBO use. Not very functional now
	void  setUseFBO(bool _use);
	void  initFBO();
	
private:
	//void makeMesh();
	ofRectangle layout(bool _skipDraw);

public:

	
private:
	string				mText;
	
	bool				mUseFBO = false;
	ofFbo				mFBO;
	ofTexture			mFBOtex;
	ofMesh				mFBOMesh;
	
	ofxFontStash*					mFont;
	ofxAsaTypeLibrary::style		mStyle;
	ofRectangle						mBBox;
	
	bool						mDirty = true;
	//ofVboMesh*				mMesh = nullptr;

	// results of layout
	string		mFormattedString = "";
	bool		mWordsWereCropped;
	int			mNumLines;
	
};
#endif /* ofxAsaType_hpp */
