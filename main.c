#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "return_codes.h"
#include "compress.h"
#include "extract.h"

#define prt_n_quit(msg) perror(msg); exit(1)

#define QUIT_MODE -1
#define COMPRESS_MODE 1
#define EXTRACT_MODE 2

void print_usage(char * prog_name);
void unknown_option(char * unknown, char * prog_name);

int main(int argc, char ** argv) {
  int mode = QUIT_MODE;
  char * in_file;
  char * out_file;

  if (argc < 3) {
    print_usage(argv[0]);
    exit(0);
  }
  
// parse command line arguments
//------------------------------------------------------------------------------
  if (strcmp(argv[1], "-x") == 0 || strcmp(argv[1], "--extract") == 0) {
    mode = EXTRACT_MODE;
  } else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--compress") == 0) {
    mode = COMPRESS_MODE;
  } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    print_usage(argv[0]);
    exit(0);
  } else {
    unknown_option(argv[1], argv[0]);
    exit(1);
  }

  in_file = argv[2];
  if (argc > 3) {
    out_file = malloc(strlen(argv[3]) + 1);
    strcpy(out_file, argv[3]);
  } else if (mode == COMPRESS_MODE) {
    out_file = malloc(strlen(argv[2])+4);
    if (!out_file) {
      prt_n_quit("malloc");
    }
    strcpy(out_file, argv[2]);
    strcat(out_file, ".hf");
  } else {
    printf("output filename required.\n\n");
    print_usage(argv[0]);
    exit(1);
  }

// real work
//------------------------------------------------------------------------------

  if (mode == COMPRESS_MODE) {
    if (compress(in_file, out_file) != SUCCESS) {
      fprintf(stderr, "Error compressing");
    }
  } else if (mode == EXTRACT_MODE) {
    if (extract(in_file, out_file) != SUCCESS) {
      fprintf(stderr, "Error extracting");
    }
  } else {
    print_usage(argv[0]);
  }
  

// cleanup
//------------------------------------------------------------------------------  
  free (out_file);
  out_file = 0;
  return 0;
}

void print_usage(char * prog_name) {
  printf("usage: %s [-xc] [FILE] [OUTPUT]\n", prog_name);
  printf("\n-x, --extract\t\tExtract file\n");
  printf("-c, --compress\t\tCompress file\n");
  printf("[FILE]\t\t\tFile to compress or extract\n");
  printf("[OUTPUT]\t\t(Optional) Name of output file\n");
}

void unknown_option(char * unknown, char * prog_name) {
    printf("Unknown option: %s\n\n", unknown);
    print_usage(prog_name);
}
