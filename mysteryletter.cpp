// Programmer: Chris Tralie
// Purpose: To create a video tracing a letter

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include "SimpleImage.h"
# define M_PI  3.14159265358979323846

int main() {
    SimpleImage canvas(400, 400);
    canvas.clearRect(0xFF, 0xFF, 0xFF);
    double r = 80;
    int N = 60;
    float cx = 200;
    float cy = 100;
    float x0, y0;
    for (int i = 0; i < N; i++) {
        double t = M_PI*0.2 + i*M_PI*(1.3)/N;
        float x1 = cx + r*cos(t);
        float y1 = cy - r*sin(t);
        if (i > 0) {
            canvas.drawLine(x0, y0, x1, y1, 4, 0, 0, 0);
        }
        
        x0 = x1;
        y0 = y1;
        std::stringstream fname;
        fname << "Mystery" << i << ".png";
        canvas.write(fname.str());
    }
    cy += 2*r;
    for (int i = 0; i < N; i++) {
        double t = M_PI/2 - i*M_PI*1.3/N;
        float x1 = cx + r*cos(t);
        float y1 = cy - r*sin(t);
        canvas.drawLine(x0, y0, x1, y1, 4, 0, 0, 0);
        x0 = x1;
        y0 = y1;
        std::stringstream fname;
        fname << "Mystery" << (i+N) << ".png";
        canvas.write(fname.str());
    }
    std::string pcmd = "ffmpeg -i Mystery50.png -vf palettegen Mysterypalette.png";
    system(pcmd.c_str());
    std::string vcmd = "ffmpeg -y -r 30 -i Mystery%d.png -i Mysterypalette.png -lavfi paletteuse MysteryLetter.gif";
    system(vcmd.c_str());
    std::string rcmd = "rm Mystery*.png";
    system(rcmd.c_str());
    return 0;
}
