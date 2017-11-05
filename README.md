# ofxAsa
A platform for making hierarchical scenes. Additional positional, automation, and material control over graphical and textual nodes.

### Features
- Objects. Extension of ofNode. In addition to inheriting positional matrices from their parents, objects inherit material parameters and on-off behavior. They can also be manually arranged in the render order. Objects can also "do" things in a custom doIdle() funciton in the render loop.
- Scenes. Each scene has its own set of lights, cameras, and child objects. Each scene universe can be rendered directly to the screen, or to an FBO to be plastered onto a compositing scene (as in example)
- Type management. Using ofxFontStash, ofxAsaType and ofxAsaTypeLibrary apply custom styles and font faces to properly layed out text.
- Animation. OfxAsaAnimator applies arbitrary animations to objects. Currently only supports linear interpolations, but the backbone is there for expanding this functionality.

### Requirements
 - ofxFontStash
 - ofxJSON

### Getting Started
 - Make your ofApp inherit from ofxAsaApp.
 - Define setup() to call ofxAsaApp::setup() and make your content
 - Define update() and draw() to call ofxAsaApp::update() ofxAsaApp::draw()
 - Define setupScenes() (follow example for an example)
 - Define setupFonts() if you will be using type.
