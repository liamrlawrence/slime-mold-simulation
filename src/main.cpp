#include <cstdio>
#include <iostream>
#include "ofMain.h"
#include "ofApp.h"

#ifndef CLION_IDE
#include "slimeMold.cc"
#endif

// TODO:
// add walls you can draw in and give agents a lifetime to "walk over food" before dying
// real time draw walls & food? food has toggle to deplete over time, toggle to kill starving agents
// save everything in PNG format including setup data (to start, have image + text file)
// agent on agent collisions
// WARNING:
// I modified ofxCvColorImage.cpp:39 to be 4 instead of 3 to add an alpha channel

int main() {
    ofSetupOpenGL(896, 504, OF_WINDOW);			// <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}
