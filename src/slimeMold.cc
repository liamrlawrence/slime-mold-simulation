//
// Created by liam on 3/29/21.
//

#include "ofApp.h"
#include "slimeMold.hh"
#include <algorithm>
#include <random>

static ofVec2f random_direction() {
    float xd, yd, uv;
    xd = ofRandomf();
    yd = ofRandomf();
    uv = sqrt(xd*xd + yd*yd);
    xd /= uv;
    yd /= uv;
    return ofVec2f{xd, yd};
}

static ofVec2f two_points_direction(ofVec2f a, ofVec2f b) {
    float x, y, uv;
    x = a[0] - b[0];
    y = a[1] - b[1];
    uv = sqrt(x*x + y*y);
    x /= uv;
    y /= uv;
    return ofVec2f{x, y};
}

void SlimeMold::init() {
    float xp, yp;
    for (int i = 0; i < this->params.nAgents; i++) {
        // annulus (try random, facing in, and facing out)
        float radius = 200;
        float orx = this->params.size.width/2;
        float ory = this->params.size.height/2;
        float ang;
        ang = ofRandomf() * PI;
        xp = orx + cos(ang)*radius;
        yp = ory + sin(ang)*radius;
        this->agents.emplace_back(agent_t{ofVec2f{xp, yp}, two_points_direction(ofVec2f{orx, ory}, ofVec2f{xp, yp})});

        // Circle pointing inwards
        //float radius = 200;
        //float ang = ofRandom(0, 1) * 2 * PI;
        //float r = radius * sqrt(ofRandom(0, 1));
        //xp = orx + cos(ang) * r;
        //yp = ory + sin(ang) * r;
        //this->agents.emplace_back(agent_t{ofVec2f{xp, yp}, two_points_direction(ofVec2f{xp, yp}, ofVec2f{orx, ory})});

        // Square
        //xp = ofRandom(this->params.size.width/3, 2*this->params.size.width/3);
        //yp = ofRandom(this->params.size.height/3, 2*this->params.size.height/3);
        //this->agents.emplace_back(agent_t{ofVec2f(xp, yp), random_direction()});
    }

    this->agent_indices.resize(this->agents.size());
    std::iota(this->agent_indices.begin(), this->agent_indices.end(), 0);
}

void SlimeMold::update() {
    std::shuffle(this->agent_indices.begin(), this->agent_indices.end(), std::default_random_engine(0));
    ofColor c;
    ofxCvColorImage ocvM;
    float xPos, yPos;
    int sense_left, sense_forward, sense_right;

    this->data_map.setColor(ofColor(0, 0, 0, 0));
    for (int i : this->agent_indices) {
        // MOTOR STAGE //
        xPos = this->agents[i].position[0] + this->agents[i].direction[0] * this->params.SS;
        yPos = this->agents[i].position[1] + this->agents[i].direction[1] * this->params.SS;
        //todo: add collision detection and toggle to prevent overlap

        // X wall collision
        if (xPos <= 0) {
            this->agents[i].direction = random_direction();
            xPos = 0;
        }
        if (xPos >= (float)this->params.size.width) {
            this->agents[i].direction = random_direction();
            xPos = this->params.size.width-1;
        }

        // Y wall collision
        if (yPos <= 0) {
            this->agents[i].direction = random_direction();
            yPos = 0;
        }
        if (yPos >= (float)this->params.size.height) {
            this->agents[i].direction = random_direction();
            yPos = this->params.size.height-1;
        }
        this->agents[i].position[0] = xPos;
        this->agents[i].position[1] = yPos;

        // Deposit onto trail
        c = this->trail_map.getColor(this->agents[i].position[0], this->agents[i].position[1]);
        c.a += this->params.depT;
        if (c.a <= 255-this->params.depT)
            c.a += this->params.depT;
        else
            c.a = 255;
        this->trail_map.setColor(this->agents[i].position[0], this->agents[i].position[1], c);

        // SENSORY STAGE //
        sense_left    = sensor_scan(agents[i], this->params.SA);
        sense_forward = sensor_scan(agents[i], 0.0f);
        sense_right   = sensor_scan(agents[i], -this->params.SA);
        if (sense_forward > sense_left && sense_forward > sense_right) {
            // pass
        } else if (sense_forward < sense_left && sense_forward < sense_right) {
            int sign = (ofRandomf() > 0) ? -1 : 1;
            agents[i].direction.rotate(this->params.RA * sign);
        } else if (sense_left < sense_right) {
            agents[i].direction.rotate(this->params.RA * -1);
        } else if (sense_right < sense_left) {
            agents[i].direction.rotate(this->params.RA);
        }
        this->data_map.setColor(this->agents[i].position[0], this->agents[i].position[1], this->params.color);
    }
    this->data_map.update();

    // Decay trail_map
    //ocvM.setFromPixels(this->trail_map.getPixels());
    //ocvM.blur(2);
    //this->trail_map.setFromPixels(ocvM.getPixels());
    for (int y = 0; y < this->params.size.height; y++) {
        for (int x = 0; x < this->params.size.width; x++) {
            c = this->trail_map.getColor(x, y);
            if (c.a == 0)
                continue;
            if (c.a >= this->params.decayT)
                c.a -= this->params.decayT;
            else
                c.a = 0;
            this->trail_map.setColor(x, y, c);
        }
    }
    this->trail_map.update();
}


void SlimeMold::draw() {
    this->trail_map.draw(0, 0);
    this->data_map.draw(0, 0);
//    ofSetColor(this->params.color);
//    for (auto a : this->agents) {
//        ofDrawCircle(a.position[0], a.position[1], 5);
//    }
}


int SlimeMold::sensor_scan(agent_t a, float angle) {
    ofVec2f d = a.direction.getRotated(angle)*this->params.SO;
    d[0] += a.position[0];
    d[1] += a.position[1];

    int sum = 0;
    for (int x = this->params.SW/-2; x <= this->params.SW/2; x++) {
        for (int y = this->params.SW/-2; y <= this->params.SW/2; y++) {
            if (d[0]+x >= 0 && d[0]+x < this->params.size.width && d[1]+y >= 0 && d[1]+y < this->params.size.height) {
                sum += this->trail_map.getColor(d[0]+x, d[1]+y).a;
            }
        }
    }

    return sum;
}
