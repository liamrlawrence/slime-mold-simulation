#include "ofApp.h"
#include "slimeMold.hh"

// GLOBALS //
static SlimeMold *smModel = nullptr;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetFrameRate(120);     // try 120 with 50k agents, also try -O0 optimization level, also try (60, 100k, -O3)

    float trail_map_constant = 50.0f;
    SlimeMold::model_params_t params = {
            .nAgents = 50000,
            .size = {
                    .width = ofGetWidth(),
                    .height = ofGetHeight(),
            },
            .diffK = 3,
            .decayT = 0.1f*trail_map_constant,
            .wProj = 0.01f,
            .boundary = SlimeMold::model_params_t::boundary_t::periodic,
            .SA = 45.0f,
            .RA = 45.0f,
            .SO = 9,
            .SW = 1,
            .SS = 1,
            .depT = (int)(5*trail_map_constant),
            .pCD = 0.0f,
            .sMin = 0.0f,
            .color = ofColor(200,100,0, 255),
    };

    smModel = new SlimeMold(params);
    smModel->init();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    smModel->update();
    smModel->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}