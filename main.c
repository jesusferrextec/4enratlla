// gcc main.c lojoc4r.c lominimax.c mainminimax.c -o q

#include <stdio.h>
#include <stdlib.h>

#include "lojoc4r.h"


int main(){

  Tauler t;
  int guanyarr=0;

  int torn=0;

  inicialitzacio(&t);
  for(int i=0; i<N*N; i++){
    torn++;
    jugades(&t, torn); // treure torn

    guanyarr=guanyar(t.taula,"XXXX" ,"OOOO" );
    if(guanyarr==1){
      printf("\nLA JUGADORA 1 (X) HA GUANYAT!!!\n");
      break;
    }
    if(guanyarr==2){
      printf("\nLA JUGADORA 2 (O) HA GUANYAT!!!\n");
      break;
    }
  }

  if(guanyarr==0){
    printf("\n  EMPAT!! \n\n");
  }

  return 0;
}
