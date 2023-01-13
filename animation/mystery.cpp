#include "canvas.h"
#include <stdint.h>
#include <iterator>
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;

# define M_PI  3.14159265358979323846

class Animation {
    private:
        WebCanvas* canvas;
        float elapsedTime;

    public:
        /**
        * @brief Construct a new animation object
        * 
        * @param port Port on which to connect to browser
        */
        Animation(int port) {
            canvas = new WebCanvas(port);
            elapsedTime = 0;
        }

        /**
        * @brief Destroy the Animation:: Animation object
        * 
        */
        ~Animation() {
            delete canvas;
        }

        void run() {
            while (true) {
                // Step 1: Determine elapsed time
                float dt = canvas->getElapsedTime();
                // Step 2: Do a step of the animation
                step(dt);
                // Step 3: Draw the results
                canvas->draw(dt);
            }
        }

        /**
        * @brief Wrapper around canvas circle method for drawing dots
        * 
        * @param x Location of dot in [0, 1]
        * @param y Location of dot in [0, 1]
        * @param diameter Diameter of dot, in [0, 1]
        * @param r Red component in [0, 255]
        * @param g Green component in [0, 255]
        * @param b Blue component in [0, 255]
        */
        void circle(float x, float y, float diameter, uint8_t r, uint8_t g, uint8_t b) {
            canvas->circle(x, y, diameter, r, g, b);
        }

        /**
        * @brief Do a step of the animation
        * 
        * @param dt The amount of time elapsed since the last step
        */
        void step(float dt) {
            elapsedTime += dt;
            double r = 0.1;
            int N = 60;
            float cx = 0.5;
            float cy = 0.5 + 0.1*sin(2*M_PI*elapsedTime);
            for (int i = 0; i < N; i++) {
                double t = M_PI*0.2 + i*M_PI*(1.3)/N;
                float x = cx - r*cos(t);
                float y = cy + r*sin(t);
                circle(x, y, 0.01, 255, (uint8_t)(255.0*i/N), 255);
            }
            cy -= 2*r;
            for (int i = 0; i < N; i++) {
                double t = M_PI/2 - i*M_PI*1.3/N;
                float x = cx - r*cos(t);
                float y = cy + r*sin(t);
                circle(x, y, 0.01, 255, 255, (uint8_t)(255.0-(int)(255.0*i/N)));
            }
        }
};


int main() {
  Animation animation(8081); // TODO: Change this to your port!!!
  animation.run();
  return 0;
}