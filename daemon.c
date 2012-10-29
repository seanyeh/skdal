/* skdal - a simple keyboard-driven app launcher
 *
 * Copyright (c) 2011 Sean Yeh
 * Distributed under the MIT License (see LICENSE file).
 */

#include "xmltree.h"
#include "daemon.h"

void set_accepting_input( int flag ){
  accepting_input = flag;

  if (flag){
    XGrabKeyboard(display, root, 0, GrabModeAsync, GrabModeAsync, CurrentTime);
    XSelectInput(display, root, KeyPressMask);


    XEvent temp_event;
    XNextEvent(display, &temp_event);
  } else{
    puts("stop grabbing keyboard?");
    XUngrabKeyboard(display, CurrentTime);
  }
}

void receive_signal (int signum) {
  printf("Debug: received signal!\n");
  set_accepting_input(1);
}

int run_daemon( mxml_node_t *tree ){
  signal(SIGUSR1, receive_signal);

  mxml_node_t *current_node = tree;
  mxml_node_t *next_node;

  // Start X11 "display"
  display = XOpenDisplay(0);
  root = DefaultRootWindow(display);

  // For delay (10ms = 10000000ns)
  struct timespec t1, t2;
  t1.tv_sec = 0;
  t1.tv_nsec = 10000000;

  accepting_input = 0;

  while( 1 )
  {
    while( !accepting_input ){
      // Slight delay to not eat up too much cpu
      nanosleep(&t1, &t2);
    }

    while ( XPending(display) > 0 ){
      XNextEvent(display, &event);

      KeySym keysym   = XLookupKeysym(&event.xkey, 0);

      sprintf(input_char,"%s", XKeysymToString(keysym));

      printf("unicode: %s len: %d\n", input_char, strlen(input_char) );

      if ( event.type != KeyPress ){}
      else if ( XKeysymToKeycode(display, keysym) == ESC_CODE ){
        set_accepting_input(0);
        current_node = tree;
      } else if (strlen(input_char)){
        // send char
        next_node = send_char(current_node, tree, input_char);
        
        /* If execute */
        if ( next_node == current_node ){
          char* command = get_text(current_node);
          printf("\n~~ Execute: %s\n\n", command);

          // execute command
          int val = system(command);
          printf("command status: %d\n",val);
          // free the malloc'ed command char* in get_text
          free(command);

          // Set next_node to null, so current_node will be reset below
          next_node = NULL;
        }
        /* If command executed above or not valid keystroke combination,
         * reset current_node */
        if (next_node == NULL){
          printf("Reset node\n");
          current_node = tree;
          set_accepting_input(0);
        }
        /* Continue traversing xml tree */
        else{
          current_node = next_node;
          printf("new current node is: ");
          print_node(current_node,tree);
        }    
      }
    }

  }

  XCloseDisplay(display);

  free(input_char);
  return 0;
}
