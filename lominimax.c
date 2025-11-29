
#include "lojoc4r.h"
#include "lominimax.h"


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
  
  reservarespai(pare);
  creaNivell(pare);
    for (int i = 0; i < pare->n_fills; i++) {
      if(nivell_actual<nivell_max){
        crearArbreRec(pare->fills[i],nivell_actual+1);
      }
    }
}
  void reservarespai(Node *arrel){
    arrel->n_fills=calculaNumFills(arrel->taula);
    arrel->fills=malloc(arrel->n_fills * sizeof(Node*));
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

    // Si numDeFill és més gran que el nombre de columnes no plenes, això és un error.
    if (numDeFill > n_columnes) {
        // Tractament d'error: per exemple, triar la primera columna no plena?
        p->columna = columnes[0];
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

int assignovalors(Node *arrel, int maximitzo){
  if(arrel->nivell==nivell_max){
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

  if(maximitzo==1){ // minimitzo -> vull que guanyen les X
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
  
  // Comportament normal
  for(int i=0 ; i<arrel->n_fills ; i++) {
      if(arrel->fills[i]->valor==valorfinal){
        return arrel->fills[i]->columna;
      }
  }
  return 0;
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
