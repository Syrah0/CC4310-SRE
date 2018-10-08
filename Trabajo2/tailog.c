#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

struct tm Str2Tm(const char* str, const char* str2)
{
    struct tm var = {0};
    var.tm_isdst = -1; // daylight saving time information is not available
    var.tm_year = 1000*(str[0]-'0')+100*(str[1]-'0')+10*(str[2]-'0')+1*(str[3]-'0')-1900;
    var.tm_mon  = 10*(str[ 5]-'0')+1*(str[ 6]-'0')-1;
    var.tm_mday = 10*(str[ 8]-'0')+1*(str[ 9]-'0');
    var.tm_hour = 10*(str2[0]-'0')+1*(str2[1]-'0');
    var.tm_min  = 10*(str2[3]-'0')+1*(str2[4]-'0');
    var.tm_sec  = 10*(str2[6]-'0')+1*(str2[7]-'0');
    return var;
}

int tailog(int argc, char **argv) {
  char *filename, *byDomIP, *domIP;
  if(argc == 3 || argc == 5) {
    filename = argv[1];
    int t = atoi(argv[2]); // t in minute
    if(argc == 5) {
      byDomIP = argv[3]; // xip : by IP, xdom: by Dom
      domIP = argv[4];
    }

    time_t myTime, xMinutesAgo, xMinutesAgo2;
    char* c_time_string;
    myTime = time(NULL);
    xMinutesAgo = myTime - t*60; //the time t minutes ago

    // Abrir archivo
    FILE *file = fopen(filename, "r");
    if (file == NULL){
      perror("Error opening file");
    } else {
      // size file
      fseek(file, 0, SEEK_END); // partir de final de archivo
      long sizeFile = ftell(file); // calcula posicion del puntero
      fseek(file, 0, SEEK_SET); // para volver al inicio del archivo

      // comenzar busqueda binaria del tiempo
      long initPos = 0;
      long lastPos = sizeFile  - 1;
      long med;
      int flag = 0;
      while(initPos < lastPos){
        med = (initPos + (lastPos - initPos)/2);

        // leer linea de la mitad
        size_t len = 0;
        char *line = NULL;
        while(fgetc(file)!= '\n'){
            fseek(file, --med, SEEK_SET);
        }
        ssize_t read = getline(&line, &len, file); // lee linea de la mitad

        // obtener fecha y hora
        char *date = strtok(line, ",");
        char *hour = strtok(NULL, ",");

        struct tm actualTime = Str2Tm(date,hour);

        time_t actualDate = mktime(&actualTime);

        // ver si coincide con fecha buscada
        if(actualDate == xMinutesAgo){
          flag = 1;
          break;
        } else if(actualDate < xMinutesAgo){
          // debo aumentar
          initPos = med + read;
        } else if(actualDate > xMinutesAgo){
          // debo disminuir
          lastPos = med;
        }
      }

      long minInit;
      if(flag){
        minInit = med;
      } else {
        minInit = initPos;
      }
      fseek(file, minInit, SEEK_SET);
      ssize_t readFile;
      size_t lenFile = 0;
      char *lineFile = NULL;

      if(argc == 5){
        if(strcmp(byDomIP, "xip") == 0){
          while((readFile = getline(&lineFile, &lenFile, file)) != -1){
            char auxLine[strlen(lineFile)];
            strcpy(auxLine, lineFile);
            strtok(auxLine, ",");
            strtok(NULL, ",");
            char *IP = strtok(NULL, ",");
            if(IP != NULL){
              if(strcmp(domIP, IP) == 0){
                fprintf(stderr, "%s", lineFile);
              }
            }
          }
        } else {
          while((readFile = getline(&lineFile, &lenFile, file)) != -1){
            char auxLine[strlen(lineFile)];
            strcpy(auxLine, lineFile);
            strtok(auxLine, ",");
            strtok(NULL, ",");
            strtok(NULL, ",");
            char *Dom = strtok(NULL, ",");
            if(Dom != NULL){
              Dom[strlen(Dom)-1] = '\0';
              if(strcmp(domIP, Dom) == 0){
                fprintf(stderr, "%s", lineFile);
              }
            }
          }
        }
      } else {
        while((readFile = getline(&lineFile, &lenFile, file)) != -1){
          fprintf(stderr, "%s", lineFile);
        }
      }
    }

  } else {
    fprintf(stderr, "%s\n", "Error with the parameters");
  }
  return 0;
}

int main(int argc, char **argv) {
  return tailog(argc, argv);
}
