#define WINVER 0x0500
#include <windows.h>
#include <stdio.h>

#define CONFIG_PATH "autologin.dat"
#define MAXBUF 1024 
#define DELIM "="

// global variables
INPUT ip;

// routines
void pressKey(SHORT key);
void chomp(char *s);

typedef struct _config_t {
   char userName[MAXBUF];
   char password[MAXBUF];
   char path[MAXBUF];
} config_t;

config_t get_config(char *filename);

//struct config get_config(char *filename);