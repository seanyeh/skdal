/* skdal - a simple keyboard-driven app launcher
 *
 * Copyright (c) 2011 Sean Yeh
 * Distributed under the MIT License (see LICENSE file).
 */

#include <stdio.h>
#include "xmltree.h"


char* get_text( mxml_node_t* node ){
  char *text = (char*)malloc(64);
  text[0] = '\0';

  while ( node && node->type != MXML_TEXT ){
    printf("should be only once\n");
    node = node->child;
  }
  if ( node ){

    mxml_node_t* temp = node;
    while ( temp ){

      strcat(text,temp->value.text.string);
      strcat(text," ");

      temp = temp->next;
    }

  } else{
    // Null text node?
  }

  return text;
}


void print_node( mxml_node_t* node, mxml_node_t* tree ){
  if ( node->type == MXML_TEXT ){
    char* text = get_text(node);
    printf("TEXT: %s", text);
    free(text);
  } else if ( node->type == MXML_ELEMENT ){
    printf("ELEMENT: %s (", node->value.element.name);
    mxml_attr_t *attrs = node->value.element.attrs;

    mxml_attr_t* temp;
    int i=0;

    // iterate through attrs
    while ( (temp = &attrs[i++]) && temp->name != NULL ){
      printf(" %s=\"%s\"", temp->name, temp->value);
    }
    printf(" )");
  }
  printf("\n");
}


mxml_node_t* get_child_element( mxml_node_t* node ){
  mxml_node_t* child = node->child;

  // skip the TEXTS (and everything but ELEMENT)
  while ( child && child->type != MXML_ELEMENT ){
    child = child->next;
  }
  return child;
}

mxml_node_t* get_next_sibling_element( mxml_node_t* node ){
  mxml_node_t* sibling = node->next;
  while ( sibling && sibling->type != MXML_ELEMENT ){
    sibling = sibling->next;
  }
  return sibling;
}



mxml_node_t* send_char( mxml_node_t* cur, mxml_node_t* tree, char* key ){

  while ( cur->type == MXML_TEXT ){
    printf("descended...necessary???\n");
    cur = mxmlWalkNext( cur, tree, MXML_DESCEND );
  }

  // do sibling traversal
  while ( cur ){
    mxml_attr_t *attrs = cur->value.element.attrs;

    mxml_attr_t* temp;
    int i=0;

    print_node(cur, tree);
    // iterate through attrs
    while ( (temp = &attrs[i++]) && temp->name != NULL ){
      if ( strcmp(temp->name, "key") == 0 && 
          strcmp( temp->value, key ) == 0 ){
        printf("YAY FOUND: %s\n", cur->value.opaque);

        // If execute, return current (same) node
        if ( strcmp( cur->value.element.name, "execute") == 0 ){
          return cur;
        } else if (strcmp( cur->value.element.name, "group") == 0){
          // Found a matching group(?) return child
          return get_child_element(cur);
        } else{ // Unrecognized tag
          printf("!!!Unrecognized tag!!!!!!\n");
          return NULL;
        }
      }
    }

    // set it to next sibiling
    cur = get_next_sibling_element(cur);

  }
  printf("Nope not found\n");

  return NULL;


}

