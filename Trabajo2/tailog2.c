#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {
  fprintf(stderr, "%i\n", argc);
  if(argc == 3 || argc == 5) {
    char *filename = argv[1];
    int t = atoi(argv[2]); // t in minute
    if(argc == 5) {
      char *byDomIP = argv[3]; // xip : by IP, xdom: by Dom
      char *domIP = argv[4];
    }

    time_t myTime;
    struct tm * timeInfo;
    time(&myTime);
    timeInfo = localtime(&myTime);

    /* VER CALCULO DEL TIEMPO */
    struct tm * mySearchTime = timeInfo;
    if(mySearchTime->tm_min >= t) {
      mySearchTime->tm_min -= t;
    } else {
      t -= mySearchTime->tm_min;
      mySearchTime->tm_hour -= 1;
      mySearchTime->tm_min = 60; // VER
    }
    char *mySearchDate;
    char *mySearchHour;

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
      long lastPos = sizeFile;
      long med;
      int flag = 0;
      while(initPos < lastPos){
        med = (initPos + (lastPos - initPos)/2);

        // leer linea de la mitad
        //fseek(file, med, SEEK_SET);
        size_t len = 0;
        char *line = NULL;
        while(fgetc(file)!= '\n'){
      //      fprintf(stderr, "med1 %li\n", med);
            fseek(file, --med, SEEK_SET);
      //      fprintf(stderr, "med2 %li\n", med);
        }
      //  fprintf(stderr, "med3 %li\n", med);
        ssize_t read = getline(&line, &len, file); // lee linea de la mitad

      //  fprintf(stderr, "%s", line);

        // obtener fecha y hora
        char *date = strtok(line, ",");
        char *hour = strtok(NULL, ",");

      //  fprintf(stderr, "d: %s h: %s\n", date, hour);
        int cmpDate = strcmp(date, mySearchDate);
        int cmpHour = strcmp(hour, mySearchHour);

        // ver si coincide con fecha buscada
        if(cmpDate == 0 && cmpHour == 0){
          flag = 1;
          break;
        } else if(cmpDate < 0){
          // debo aumentar
          initPos = med + read;
        } else if(cmpDate > 0){
          // debo disminuir
          lastPos = med;
        } else if(cmpHour < 0){ // fechas iguales
          // debo aumentar
          initPos = med + read;
        } else {
          // debo disminuir
          lastPos = med;
        }
      }

      long minInit;
      if(flag){
        minInit = med;
      } else {
        minInit = initPos; // = lastPos  = med + read (?)
      }

      // Empezar a leer, linea por linea desde minInit
      // Si argc == 5, entonces filtrar por dom o ip
      // Sino, entonces mostrar la ultima actividad (print lineas)
      //fprintf(stderr, "size %li, sizeline %zu\n", sizeFile, read);

    }

  } else {
    fprintf(stderr, "%s\n", "Error with the parameters");
  }
  return 0;
}
