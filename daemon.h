/* skdal - a simple keyboard-driven app launcher
 *
 * Copyright (c) 2011 Sean Yeh
 * Distributed under the MIT License (see LICENSE file).
 */

#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <signal.h>
#include <time.h>
#include <mxml.h>

#define ESC_CODE 9

int accepting_input;
Display* display;
Window root;
XEvent event;

char input_char[2];


int run_daemon( mxml_node_t *tree );


#endif
