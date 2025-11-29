#include "lojoc4r.h"
#include "lominimax.h"
#include <time.h>

// nivell imparell és O    (p->nivell%2)
// nivell parell és X    (p->nivell%2==0)
int mainminimax(Tauler *t){
  srand(time(NULL));  // Inicialitza random
  printf("\n=== INICI MINIMAX ===\n");
  
  // Depuració: mostra l'estat actual del tauler
  printf("Tauler actual:\n");
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      printf("%c ", t->taula[i][j]);
    }
    printf("\n");
  }
  
  Node *arrel = malloc(sizeof(Node));
  copiaTauler(arrel->taula, t->taula);
  inicialitzacionode(arrel);
  
  printf("Creant arbre...\n");
  crearArbreRec(arrel, 1);
  
  printf("Assignant valors...\n");
  int valorfinal = assignovalors(arrel, 0);
  printf("Valor final: %d\n", valorfinal);
  
  int col = buscoquinacolumnaes(arrel, valorfinal);
  printf("Columna triada: %d\n", col+1);
  
  alliberarEspai(arrel);
  printf("=== FI MINIMAX ===\n\n");
  
  return col;
}