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

/**struct config {
   char *userName;
   char *password;
   char *path;
};

struct config get_config(char *filename);*/

//struct config get_config(char *filename);