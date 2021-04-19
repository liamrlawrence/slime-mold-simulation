//
// Created by liam on 3/29/21.
//

#ifndef SLIMEMOLD_HH
#define SLIMEMOLD_HH


class SlimeMold {
public:
    typedef struct {
        int nAgents;            // todo:remove me!
        struct {
            int width;
            int height;
        } size;
        int diffK;              // Diffusion kernel size
        float decayT;           // Trail-map chemoattractant diffusion decay factor
        float wProj;            // Pre-pattern stimuli projection weight
        enum boundary_t {       // Diffusion and agent environmental boundary conditions
            null = 0,           //      - Not specified
            periodic = 1,       //      - TODO: change me!
        } boundary;
        float SA;               // FL and FR sensor angle from forward position
        float RA;               // Agent rotation angle
        int SO;                 // Sensor offset distance
        int SW;                 // Sensor width
        int SS;                 // Step size - how far agent moves per step
        int depT;               // Chemoattractant deposition per step
        float pCD;              // Probability of a random change in direction
        float sMin;             // Sensitivity threshold
        ofColor color;          // Color of the agent
        // todo: add collision_en and speed variables?
    } model_params_t;
    model_params_t params;

    void init();
    void update();
    void draw();
    SlimeMold(model_params_t parameters) {
        this->params = parameters;
        this->data_map.allocate(this->params.size.width, this->params.size.height, OF_IMAGE_COLOR_ALPHA);
        this->trail_map.allocate(this->params.size.width, this->params.size.height, OF_IMAGE_COLOR_ALPHA);
        ofColor trail_color = ofColor(this->params.color.r, this->params.color.b, this->params.color.g, 0.0f);
        this->trail_map.setColor(trail_color);
    }

private:
    ofImage data_map;
    ofImage trail_map;
    typedef struct {
        ofVec2f position;
        ofVec2f direction;
    } agent_t;
    std::vector<agent_t> agents;
    std::vector<int> agent_indices;
    int sensor_scan(agent_t a, float angle);
};


#endif //SLIMEMOLD_HH
