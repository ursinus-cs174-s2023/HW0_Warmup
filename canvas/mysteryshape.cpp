// Programmer: Chris Tralie
// Purpose: To create a video tracing a letter

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include "simplecanvas/SimpleCanvas.h"
#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

int main() {
    SimpleCanvas canvas(200, 200);
    canvas.clearRect(0xFF, 0xFF, 0xFF);
    double r = 80;
    int N = 60;
    float cx = 100;
    float cy = 100;
    float x0, y0;
    for (int i = 0; i < N; i++) {
        double t = 3*((float)i)*M_PI/N;
        float x1 = cx + r*cos(t);
        float y1 = cy - r*sin(t);
        if (i > 0) {
            canvas.drawLine(x0, y0, x1, y1, 4, 0, 0, 0);
        }
        x0 = x1;
        y0 = y1;
    }
    canvas.write("mystery.png");
    return 0;
}
