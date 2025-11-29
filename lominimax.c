#include "lojoc4r.h"
#include "lominimax.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void copiaTauler(char taula1[N+1][N+1], char taula2[N+1][N+1]){
  for(int i=0; i<N+1; i++){
    for(int j=0; j<N+1; j++){
      taula1[i][j]=taula2[i][j];
    }
  }
}

void inicialitzacionode(Node* arrel){
  arrel->fills=NULL;
  arrel->n_fills=0;
  arrel->valor=0;
  arrel->nivell=0;
  arrel->columna=0;
}

void crearArbreRec( Node* pare, int nivell_actual){
  // No expandim si és estat terminal (algú ha guanyat) o si ja hem superat el nivell_max.
  int gg = guanyar(pare->taula, "XXXX", "OOOO");
  if (gg != 0 || nivell_actual > nivell_max) {
    pare->n_fills = 0;
    pare->fills = NULL;
    return;
  }

  reservarespai(pare);
  if (pare->n_fills == 0) {
    pare->fills = NULL;
    return;
  }

  creaNivell(pare);
  for (int i = 0; i < pare->n_fills; i++) {
    if(nivell_actual < nivell_max){
      crearArbreRec(pare->fills[i],nivell_actual+1);
    }
  }
}

void reservarespai(Node *arrel){
  arrel->n_fills=calculaNumFills(arrel->taula);
  if (arrel->n_fills > 0) {
    arrel->fills=malloc(arrel->n_fills * sizeof(Node*));
    memset(arrel->fills, 0, arrel->n_fills * sizeof(Node*));
  } else {
    arrel->fills = NULL;
  }
  // reservo espai de l'apuntador Node*
}

int calculaNumFills(char taula[N+1][N+1]){
  int comptador=0;
  for(int columna=0; columna<N; columna++){
    for(int fila=N-1; fila>-1; fila--){
      if(taula[fila][columna]=='.'){
        comptador++;
        break;
      }
    }
  }

  return comptador;
}

void creaNivell(Node* pare){
    for(int i=0; i< pare->n_fills; i++){
        pare->fills[i] = creaNode(pare, i+1);
        // crea el node amb el malloc,
        // l'inicialitza tot a zeros,
        // copia el tauler del pare
        // i assigna la tirada segons el numdefill que és (i+1)
    }
}
Node* creaNode(Node* pare, int numDeFill){
  Node *p=malloc(sizeof(Node));
  // reservo espai del node com a tal
  inicialitzacionode(p);
  copiaTauler(p->taula, pare->taula);
  p->nivell = pare->nivell + 1;
  tirada(p, numDeFill);

  return p;
}
void tirada(Node* p, int numDeFill){
  // la columna no és numDeFill
  quinacolumnaes(p,numDeFill); // i es guarda en p->columna
  int col=p->columna;
  //mirem quina és la fila lliure de la columna col
  int fila;
  for(fila=N-1; fila>-1; fila--){
    if(p->taula[fila][col]=='.'){
      break;
    }
  }
  if(p->nivell%2){ // és imparell
    p->taula[fila][col] = 'O';
  }
  if(p->nivell%2==0){ // és parell
    p->taula[fila][col] = 'X';
  }
}
void quinacolumnaes(Node* p, int numDeFill) {
    int columnes[N];
    int n_columnes = 0;

    // Recollim les columnes no plenes
    for (int j = 0; j < N; j++) {
        for (int i = N-1; i >= 0; i--) {
            if (p->taula[i][j] == '.') {
                columnes[n_columnes++] = j;
                break;
            }
        }
    }

    // Si numDeFill és fora de rang, triem la primera columna no plena
    if (numDeFill <= 0 || numDeFill > n_columnes) {
        if (n_columnes > 0) p->columna = columnes[0];
        else p->columna = 0;
    } else {
        p->columna = columnes[numDeFill-1];
    }
}

int funcioheuristica(Node* arrel){
  int gg=guanyar(arrel->taula,"XXXX","OOOO");
  if(gg==1){
    return -1000 ; // guanyen les X (persona)
  }
  if(gg==2){
    return 1000 ; // guanyen les O (ordinador)
  }
  int gg2=guanyar(arrel->taula,"XXX.","OOO.");
  int gg3=guanyar(arrel->taula,".XXX",".OOO");
  int gg22=guanyar(arrel->taula,"XX.X","OO.O");
  int gg33=guanyar(arrel->taula,"X.XX","O.OO");
  if(gg2==1 || gg3==1 || gg22==1 || gg33==1){
    return -50; // tres X i espai (persona)
  }
  if(gg2==2|| gg3==2 || gg22==2|| gg33==2){
    return 50; // tres O i espai (ordinador)
  }

  int gg4=guanyar(arrel->taula,"XX..","OO..");
  int gg5=guanyar(arrel->taula,"..XX","..OO");
  int gg44=guanyar(arrel->taula,"X.X.","O.O.");
  int gg55=guanyar(arrel->taula,".X.X",".O.O");
  if(gg4==1 || gg5==1 || gg44==1 || gg55==1){
    return -10; // tres X i espai (persona)
  }
  if(gg4==2|| gg5==2 || gg44==2|| gg55==2){
    return 10; // tres O i espai (ordinador)
  }

// qualsevol altra cosa neutra:
  return 0;
}

// Helper: retorna 1 si des del node 'fill' existeix una jugada de l'oponent (X) que guanyi immediatament
static int fill_deixa_oponent_guanyar_immediat(Node* fill) {
    for (int col = 0; col < N; col++) {
        for (int fila = N-1; fila >= 0; fila--) {
            if (fill->taula[fila][col] == '.') {
                // simula colocar 'X' a (fila,col)
                char copia[N+1][N+1];
                for(int i=0;i<N;i++) for(int j=0;j<N;j++) copia[i][j]=fill->taula[i][j];
                copia[fila][col] = 'X';
                int gg = guanyar(copia, "XXXX", "OOOO");
                if (gg == 1) return 1; // l'oponent pot guanyar la següent jugada
                break; // ja hem simulat aquesta columna
            }
        }
    }
    return 0;
}

int assignovalors(Node *arrel, int maximitzo){
  // Si som a fulla real (no hi ha fills) avaluem directament
  if (arrel->nivell==nivell_max || arrel->n_fills == 0){
    arrel->valor=funcioheuristica(arrel);
    return arrel->valor;
  }

  if(maximitzo==0){ // maximitzo -> vull que guanyen les O
    int max=assignovalors(arrel->fills[0], 1);
    for (int i = 1; i < arrel->n_fills; i++) {
      int este= assignovalors(arrel->fills[i], 1);
      if(este>max){
        max=este;
      }
    }
    arrel->valor=max;
    return max;
  }

  if(maximitzo==1){ // minimitzo -> vull que guanyin les X
    int min=assignovalors(arrel->fills[0], 0);
    for (int i = 1; i < arrel->n_fills; i++) {
      int este= assignovalors(arrel->fills[i], 0);
      if(este<min){
        min=este;
      }
    }
    arrel->valor=min;
    return min;
  }
  return 0;
}

int buscoquinacolumnaes(Node* arrel, int valorfinal){
  // Si totes les jugades perden, triem la que fa que el joc duri més temps
  if(valorfinal == -1000){
    int max_fills = -1;
    int millor_columna = 0;

    for(int i=0 ; i<arrel->n_fills ; i++) {
        // Comptem quants fills té aquesta jugada (més fills = més opcions = joc més llarg)
        if(arrel->fills[i]->n_fills > max_fills){
            max_fills = arrel->fills[i]->n_fills;
            millor_columna = arrel->fills[i]->columna;
        }
    }
    return millor_columna;
  }

  // 1) preferir fills amb valor==valorfinal i que NO deixin oponent guanyar immediat
  for(int i=0 ; i<arrel->n_fills ; i++) {
      if(arrel->fills[i]->valor==valorfinal && !fill_deixa_oponent_guanyar_immediat(arrel->fills[i])){
        return arrel->fills[i]->columna;
      }
  }

  // 2) fallback: qualsevol fill amb valor exact
  for(int i=0 ; i<arrel->n_fills ; i++) {
      if(arrel->fills[i]->valor==valorfinal){
        return arrel->fills[i]->columna;
      }
  }

  // 3) si no hi ha cap fill amb el valor exacte, triem el que tingui valor més proper
  int millor_idx = 0;
  int millor_diff = INT_MAX;
  for(int i=0; i<arrel->n_fills; i++){
      int diff = abs(arrel->fills[i]->valor - valorfinal);
      if(diff < millor_diff){
          millor_diff = diff;
          millor_idx = i;
      } else if(diff == millor_diff){
          // desempatem per la que té més fills (joc més llarg)
          if(arrel->fills[i]->n_fills > arrel->fills[millor_idx]->n_fills){
              millor_idx = i;
          }
      }
  }
  return arrel->fills[millor_idx]->columna;
}

void recorreArbreRecfetProfe(Node *arrel,FILE *fp) {
    for(int i=0;i<arrel->nivell;i++) {
        fprintf(fp,"  ");
    }
    fprintf(fp,"nivell:%d -> columna:%i -> valor:%d\n",arrel->nivell,arrel->columna,arrel->valor);
    for(int i=0 ; i<arrel->n_fills ; i++) {
        recorreArbreRecfetProfe(arrel->fills[i],fp);
    }
}



void alliberarEspai(Node *arrel) {
    if (arrel == NULL) return;

    for (int i = 0; i < arrel->n_fills; i++) {
        alliberarEspai(arrel->fills[i]);
    }

    if (arrel->fills != NULL) {
      free(arrel->fills);
    }

    free(arrel);
}