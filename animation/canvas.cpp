/**
 * @file canvas.cpp
 * @author Chris Tralie (ctralie@alumni.princeton.edu)
 * @brief A file for sending drawing events to the browser
 * @version 0.1
 * @date 2023-01-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "canvas.h"
#include <ws.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <stdio.h>
using namespace std;

// GLOBAL VARIABLES FOR COORDINATING COMMUNICATIONS
bool drawReady = false;
mutex drawMutex;

// GLOBAL VARIABLES FOR SHAPE TYPES
const uint8_t CIRCLE = 0;
const uint8_t SQUARE = 1;


/**
 * @brief Called when a client connects to the server.
 *
 * @param client Client connection. The @p client parameter is used
 * in order to send messages and retrieve informations about the
 * client.
 */
void onopen(ws_cli_conn_t *client) {
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection opened, addr: %s\n", cli);
}

/**
 * @brief Called when a client disconnects to the server.
 *
 * @param client Client connection. The @p client parameter is used
 * in order to send messages and retrieve informations about the
 * client.
 */
void onclose(ws_cli_conn_t *client) {
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection closed, addr: %s\n", cli);
}

/**
 * @brief Called when a client connects to the server.
 *
 * @param client Client connection. The @p client parameter is used
 * in order to send messages and retrieve informations about the
 * client.
 *
 * @param msg Received message, this message can be a text
 * or binary message.
 *
 * @param size Message size (in bytes).
 *
 * @param type Message type.
 */
void onmessage(ws_cli_conn_t *client,
	const unsigned char *msg, uint64_t size, int type) {
    drawMutex.lock();
    drawReady = true;
    drawMutex.unlock();
}


WebCanvas::WebCanvas(int port) {
    lastTime = Clock::now();
    evs.onopen = &onopen;
    evs.onclose = &onclose;
    evs.onmessage = &onmessage;
    ws_socket(&evs, port, 1, 1000);

}

/**
 * @brief Write the 4 bytes of a float to a stringstream
 * 
 * @param ss String stream
 * @param f Float
 */
void writeFloatBytes(stringstream& ss, float f) {
    uint8_t* fptr = (uint8_t*)&f;
    for (int i = 0; i < 4; i++) {
        ss << fptr[i];
    }
}


void WebCanvas::circle(float x, float y, float diameter, uint8_t r, uint8_t g, uint8_t b) {
    shapes << CIRCLE;
    shapes << r << g << b;
    writeFloatBytes(shapes, x);
    writeFloatBytes(shapes, y);
    writeFloatBytes(shapes, diameter);
}


void WebCanvas::draw(float dt) {
    // Step 1: Block until ready to draw
    while (true) {
        drawMutex.lock();
        if (!drawReady) {
            drawMutex.unlock();
            this_thread::sleep_for(chrono::milliseconds(5));
        }
        else {
            drawMutex.unlock();
            break;
        }
    }
    

    // Step 1: Send over shapes to browser
    string s = shapes.str();

    ws_sendframe_bin(NULL, s.c_str(), s.length());

    shapes.clear();
    shapes.str("");

    drawMutex.lock();
    drawReady = false;
    drawMutex.unlock();
}

/**
 * @brief Return the amount of time elapsed since the last time this
 *        method was called, in milliseconds
 * 
 * @return float Elapsed time
 */
float WebCanvas::getElapsedTime() {
    Clock::time_point now = Clock::now();
    std::chrono::duration<float, std::milli> dt = now-lastTime;
    lastTime = now;
    float ret = (float)(dt.count())/1000.0;
    if (ret > 0.2) {
        ret = 0.2; // Bottom out at 5fps
    }
    return ret;
}