/* skdal - a simple keyboard-driven app launcher
 *
 * Copyright (c) 2011 Sean Yeh
 * Distributed under the MIT License (see LICENSE file).
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "xmltree.h"
#include "daemon.h"

static const struct option longopts[] =
{
  { "help", no_argument, NULL, 'h' },
  { "version", no_argument, NULL, 'v' },
  { "config", required_argument, NULL, 'c' },
  { "daemon", no_argument, NULL, 'd' },
  { NULL, 0, NULL, 0 }
};

const char* program_name;

static void print_help(int exit_status){
  printf("\nUsage: %s [options]\n"
      "A simple keyboard-driven app launcher\n\n"
      "  -h, --help\t\tDisplay this menu\n"
      "  -v, --version\t\tDisplay the current application version\n\n"
      "  -d, --daemon\t\tRun the launcher daemon. The daemon must be started before "
      "running the launcher normally.\n"
      "  -c, --config=FILE\tConfiguration file\n\n"
      , program_name);
  exit(exit_status);
}

static void print_version(){
  printf("skdal version %s\n", "0.1");
  exit(EXIT_SUCCESS);
}

static void print_error(char* err){
  fprintf(stderr,"Error: %s\n", err);
  print_help(EXIT_FAILURE);
}

/* Declared below */
static pid_t get_pid(void);

int main(int argc, char **argv){
  // Default config file at $XDG_CONFIG_HOME/skdal/skdal.xml
  char* config_file = (char*)malloc(256);
  char * config_dir = getenv("XDG_CONFIG_HOME");
  sprintf(config_file,"%s%s",config_dir,"/skdal/skdal.xml");

  program_name = argv[0];
  int optc;
  int isdaemon = 0;

  while ((optc = getopt_long (argc, argv, "hvc:d", longopts, NULL)) != -1){
    switch (optc){
      case 'h':
        print_help(EXIT_SUCCESS);
        break;
      case 'v':
        print_version();
        break;
      case 'c':
        sprintf(config_file,"%s",optarg);
        break;
      case 'd':
        isdaemon = 1;
        break;
      default:
        print_help(EXIT_FAILURE);
        break;
    }
  }


  if (isdaemon){
    printf("Using config file: %s\n", config_file);
    // Read xml configuration file
    FILE *fp;
    mxml_node_t *tree;
    if ( !(fp = fopen(config_file, "r")) ){
      print_error("Configuration file is not valid.");
    }
    tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
    fclose(fp);
    free(config_file);

    // Get first element
    mxml_node_t *node = get_child_element(tree);
    if ( node == NULL){
      print_error("Configuration file is not valid.");
    }

    return run_daemon(node);
  } else{
    pid_t pid = get_pid();
    if ( pid == -1 ){
      print_error("You must run daemon first.");
    } else{
      return kill(get_pid(),SIGUSR1);
    }

    // Just to make the compiler happy. There's probably a better way...
    return 0;
  }
}

static pid_t get_pid(void){
  char buf[100];
  FILE *p = popen("pidof skdal", "r");

  pid_t pid = -1;
  if(p == NULL || fgets(buf, sizeof(buf), p) == NULL){
    print_error("Cannot find pid of the skdal daemon. Make sure to run the daemon first.");
  } else{
    pclose(p);

    fprintf(p, "%s", buf);

    // Find index of last ' ' (pidof returns space-separated pid's)
    int i = strlen(buf) - 1;
    while( buf[i] != ' ' ){
      if ( i == 0 ){
        return -1;
      }
      i--;
    }
    // Increment i to make it the index after the last space
    i++;
    // Index counter for pid_buf
    int j = 0;
    // Create pid_buf to store the last pid
    char pid_buf[strlen(buf)-i];

    while ( i < strlen(buf) ){
      pid_buf[j++] = buf[i++];
    }

    // Replace last newline(from pidof) with '\0'
    pid_buf[j-1] = '\0';

    pid = (pid_t)atoi(pid_buf);
  }
  return pid;
}
