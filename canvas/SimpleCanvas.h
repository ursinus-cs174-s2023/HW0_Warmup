/**
 * @brief A simple library for 2D drawing in C++, by Chris Tralie
 */
#ifndef SIMPLEIMG_H
#define SIMPLEIMG_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


class SimpleCanvas {
    private:
        // Helper method for Bresenham's line algorithm
        void drawLineLow(int x0, int y0, int x1, int y1, uint8_t R, uint8_t G, uint8_t B) {
            int dx = x1 - x0;
            int dy = y1 - y0;
            int yi = 1;
            if (dy < 0) {
                yi = -1;
                dy = -dy;
            }
            int D = (2 * dy) - dx;
            int y = y0;
            for (int x = x0; x <= x1; x++) {
                setPixel(x, y, R, G, B);
                if (D > 0) {
                    y = y + yi;
                    D = D + (2 * (dy - dx));
                }
                else {
                    D = D + 2*dy;
                }
            }
        }

        // Helper method for Bresenham's line algorithm
        void drawLineHigh(int x0, int y0, int x1, int y1, uint8_t R, uint8_t G, uint8_t B) {
            int dx = x1 - x0;
            int dy = y1 - y0;
            int xi = 1;
            if (dx < 0) {
                xi = -1;
                dx = -dx;
            }
            int D = (2 * dx) - dy;
            int x = x0;
            for (int y = y0; y <= y1; y++) {
                setPixel(x, y, R, G, B);
                if (D > 0) {
                    x = x + xi;
                    D = D + (2 * (dx - dy));
                }
                else {
                    D = D + 2*dx;
                }
            }
        }

        /**
         * @brief Draw a character at a particular position by loading its bitmap
         * image and copying it over pixel by pixel to a chosen position
         * @param c The character to draw
         * @param x x location of lower left corner of character
         * @param y y location of lower left corner of character
         * @param dx Change in x to move cursor after this character
         * @param dy Change in y to move cursor after this character
         * @return Width of character
         */
        void drawChar(char c, int x0, int y0, int* dx, int* dy) {
            if (((int)c > 31 || (int)c == 10) && !((int)c > 126 && (int)c < 160)) {
                std::stringstream ss;
                ss << "Font/" << ((int)c) << ".png";
                int w, h, n;
                uint8_t* img = stbi_load(ss.str().c_str(), &w, &h, &n, 3);
                if (img != NULL) {
                    if ((int)c == 10 || c == '\n') {
                        *dx = -1;
                        *dy = h+2;
                    }
                    else {
                        *dx = w+2;
                        *dy = 0;
                        for (int dy = 0; dy < h; dy++) {
                            int y = y0 + dy;
                            for (int dx = 0; dx < w; dx++) {
                                int x = x0 + dx;
                                uint8_t R = img[(dy*w+dx)*3];
                                uint8_t G = img[(dy*w+dx)*3+1];
                                uint8_t B = img[(dy*w+dx)*3+2];
                                if (!(R == 0xFF && G == 0xFF && B == 0xFF)) {
                                    setPixel(x, y, R, G, B);
                                }
                            }
                        }
                    }
                    stbi_image_free(img);
                }
                
            }
        }

        /**
         * @brief Create a 3D array wrapper around the flattened image format
         * 
         */
        void createWrapper() {
            data = new uint8_t**[height];
            for (size_t i = 0; i < height; i++) {
                data[i] = new uint8_t*[width];
                for (size_t j = 0; j < width; j++) {
                    data[i][j] = dataInternal + (i*width+j)*3;
                }
            }
        }

        void deallocate() {
            if (dataInternal != NULL) {
                for (size_t i = 0; i < height; i++) {
                    delete[] data[i];
                }
                delete[] data;
                stbi_image_free(dataInternal);
            }
        }

        uint8_t* dataInternal;

    public:
        int width, height;
        uint8_t*** data;

        /**
         * @brief Construct a new image object from a file.  
         * 
         * @param fname Path to file
         */
        SimpleCanvas(std::string fname) {
            dataInternal = NULL;
            read(fname);
        }

        /**
         * @brief Instantiate an image object of a particular dimension
         * 
         * @param width Width of image
         * @param height Height of image
         */
        SimpleCanvas(int32_t width, int32_t height) {
            this->width = width;
            this->height = height;
            dataInternal = (uint8_t*)malloc(width*height*3);
            createWrapper();
        }

        /**
         * @brief Destroy the Simple Image object by de-allocating the pixel data
         */
        ~SimpleCanvas() {
            deallocate();
        }

        /**
         * @brief Read in a ppm file.  Use ffmpeg to convert the file to a 
         * temporary .ppm file if it has a non ppm extension
         * 
         * @param fname Path to file
         */
        void read(std::string fname) {
            int numChannels;
            dataInternal = stbi_load(fname.c_str(), &this->width, &this->height, &numChannels, 3);
            if (dataInternal == NULL) {
                throw std::runtime_error("Unable to open the image file " + fname);
            }
            else {
                createWrapper();
            }
        }

        /**
         * Write the image to a file
         * If the filename ends with an extension other than ppm, write it to a
         * temporary ppm file, and use ffmpeg to convert it to the desired name
         * 
         * @param fname Name of the file
         * 
         */
        void write(std::string fname) {
            std::string newfname = fname;
            std::string ext = "";
            if (fname.length() >= 4) {
                ext = fname.substr(fname.length()-3, 3);
            }
            if (ext == "png") {
                stbi_write_png(fname.c_str(), this->width, this->height, 3, dataInternal, this->width*3);
            }
            else {
                throw std::runtime_error("Unable to write to image " + fname + " with extension " + ext);
            }
        }

        /**
         * @brief Set the color of a particular pixel
         * 
         * @param x X coordinate of pixel
         * @param y Y coordinate of pixel
         * @param R Red channel
         * @param G Green channel
         * @param B Blue channel
         */
        void setPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B) {
            if (x >= 0 && y >= 0 && x < width && y < height) {
                data[y][x][0] = R;
                data[y][x][1] = G;
                data[y][x][2] = B;
            }
        }

        /**
         * @brief Fill a solid rectangle with a color
         * 
         * @param x0 Left of rectangle
         * @param y0 Top of rectangle
         * @param w Width of rectangle
         * @param h Height of rectangle
         * @param R Red channel
         * @param G Green channel
         * @param B Blue channel
         */
        void fillRect(int x0, int y0, int w, int h, uint8_t R, uint8_t G, uint8_t B) {
            if (x0 < 0 || y0 < 0 || x0 + w > width || y0 + h > height) {
                throw std::runtime_error("The rectangle does not fit in the image!");
            }
            for (int y = y0; y < y0 + h; y++) {
                for (int x = x0; x < x0 + w; x++) {
                    data[y][x][0] = R;
                    data[y][x][1] = G;
                    data[y][x][2] = B;
                }
            }
        }

        /**
         * @brief Clear the entire canvas to be one color
         * 
         * @param R Red channel
         * @param G Green channel
         * @param B Blue channel
         */
        void clearRect(uint8_t R, uint8_t G, uint8_t B) {
            fillRect(0, 0, width, height, R, G, B);
        }

        /**
         * @brief Draw the outline of a rectangle with a particular color
         * 
         * @param x0 Left of rectangle
         * @param y0 Top of rectangle
         * @param w Width of rectangle
         * @param h Height of rectangle
         * @param R Red channel
         * @param G Green channel
         * @param B Blue channel
         * @param line_w Width of rectangle
         */
        void drawRect(int x0, int y0, int w, int h,
                            uint8_t R, uint8_t G, uint8_t B, uint8_t line_w) {
            if (x0 + w > width || y0 + h > height) {
                throw std::runtime_error("The rectangle does not fit in the image!");
            }
            fillRect(x0, y0, w, line_w, R, G, B);                                             // top line
            fillRect(x0, (y0 + h - line_w), w, line_w, R, G, B);                              // bottom line
            fillRect((x0 + w - line_w), (y0 + line_w), line_w, (h - (2 * line_w)), R, G, B);  // right line
            fillRect(x0, (y0 + line_w), line_w, (h - (2 * line_w)), R, G, B);                 // left line
        }

        /**
         * @brief Draw a thin line from one point to another
         * 
         * @param x0 X coordinate of first point
         * @param y0 Y coordinate of first point
         * @param x1 X coordinate of second point
         * @param y1 Y coordinate of second point
         * @param R Red channel
         * @param G Green channel
         * @param B Blue channel
         */
        void drawLine(int x0, int y0, int x1, int y1, uint8_t R, uint8_t G, uint8_t B) {
            if (abs(y1 - y0) < abs(x1 - x0)) {
                if (x0 > x1) {
                    drawLineLow(x1, y1, x0, y0, R, G, B);
                }
                else {
                    drawLineLow(x0, y0, x1, y1, R, G, B);
                }
            }
            else {
                if (y0 > y1) {
                    drawLineHigh(x1, y1, x0, y0, R, G, B);
                }
                else {
                    drawLineHigh(x0, y0, x1, y1, R, G, B);
                }
            }
        }

        
        /**
         * @brief Draw a thick line from one point to another
         * 
         * @param x0 X coordinate of first point
         * @param y0 Y coordinate of first point
         * @param x1 X coordinate of second point
         * @param y1 Y coordinate of second point
         * @param thickness Thickness of line
         * @param R Red channel
         * @param G Green channel
         * @param B Blue channel
         */
        void drawLine(int x0, int y0, int x1, int y1, int thickness, uint8_t R, uint8_t G, uint8_t B) {
            if (thickness == 1) {
                drawLine(x0, y0, x1, y1, R, G, B);
            }
            else {
                int T = thickness/2;
                for (int dx = -T; dx <= T; dx++) {
                    for (int dy = -T; dy <= T; dy++) {
                        drawLine(x0+dx, y0+dy, x1+dx, y1+dy, R, G, B);
                    }
                }
            }
        }

        /**
         * @brief Fill in a solid disc of a particular color
         * 
         * @param cx X coordinate of center of disc
         * @param cy Y coordinate of center of disc
         * @param r Radius of disc
         * @param R Red channel
         * @param G Green channel
         * @param B Blue channel
         */
        void fillCircle(int cx, int cy, double r, uint8_t R, uint8_t G, uint8_t B) {
            for (int x = cx-r-1; x <= cx+r+1; x++) {
                for (int y = cy-r-1; y <= cy+r+1; y++) {
                    double dx = x-cx;
                    double dy = y-cy;
                    if (dx*dx + dy*dy < r*r) {
                        setPixel(x, y, R, G, B);
                    }
                }
            }
        }

        /**
         * @brief Draw a character at a string at a position
         * @param s The string to draw
         * @param x x location of lower left corner of the string
         * @param y y location of lower left corner of the string
         */
        void drawString(std::string s, int x0, int y0) {
            int x = x0;
            int y = y0;
            const char* c = s.c_str();
            int dx = 0, dy = 0;
            for (size_t i = 0; i < s.length(); i++) {
                drawChar(c[i], x, y, &dx, &dy);
                if (dx == -1) {
                    // Carriage return
                    x = x0;
                }
                else {
                    x += dx;
                }
                y += dy;
            }
        }
};

#endif