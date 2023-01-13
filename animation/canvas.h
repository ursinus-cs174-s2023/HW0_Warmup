#ifndef CANVAS_H
#define CANVAS_H

#include <ws.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <stdio.h>
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

/**
 * @brief This is a helper class to wrap around web drawing functionality
 *        and animation timing
 */
class WebCanvas {
    private:
        Clock::time_point lastTime;
        struct ws_events evs;
        stringstream shapes;

    public:
        WebCanvas(int port);
        void circle(float x, float y, float diameter, uint8_t r, uint8_t g, uint8_t b);
        void draw(float dt);
        float getElapsedTime();
};

#endif