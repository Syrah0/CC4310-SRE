#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {
  if(argc == 2 || argc == 4) {
    char *file = argv[1];
    int t = atoi(argv[2]); // t in minute
    if(argc == 4) {
      char *byDomOrIP = argv[3]; // xip : by IP, xdom: by Dom
      char *domOrIṔ = argv[4];
    }

    time_t myTime;
    struct tm * timeInfo;
    time(&myTime);
    timeInfo = localtime(&myTime);

    struct tm * mySearchTime = timeInfo;
    if(mySearchTime.tm_min >= t) {
      mySearchTime.tm_min -= t;
    } else {
      t -= mySearchTime.tm_min;
      mySearchTime.tm_hour -= 1;
      mySearchTime.tm_min = 60; // VER
    }
  } else {
    fprintf(stderr, "%s\n", "Error with the parameters");
  }
  return 0;
}
