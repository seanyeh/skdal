/* skdal - a simple keyboard-driven app launcher
 *
 * Copyright (c) 2011 Sean Yeh
 * Distributed under the MIT License (see LICENSE file).
 */

#ifndef __XMLTREE_H__
#define __XMLTREE_H__

#include <mxml.h>
 
extern char* get_text( mxml_node_t* node );
extern void print_node( mxml_node_t* node, mxml_node_t* tree );
extern mxml_node_t* get_child_element( mxml_node_t* node );
extern mxml_node_t* get_next_sibling_element( mxml_node_t* node );
extern mxml_node_t* send_char( mxml_node_t* cur, mxml_node_t* tree, char* key );


    
#endif

