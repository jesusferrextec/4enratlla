#include "lojoc4r.h"
#include "lominimax.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Helper: simula una tirada del token ('O' per ordinador, 'X' per persona) a la columna col.
// Retorna el resultat de guanyar(copia): 0 cap, 1 X guanya, 2 O guanya.
static int prova_tirada_i_comprova(char taula[N+1][N+1], int col, char token) {
    if(col < 0 || col >= N) return 0;
    char copia[N+1][N+1];
    // copia tauler
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) copia[i][j]=taula[i][j];
    // busca fila lliure
    int fila = -1;
    for(int f=N-1; f>=0; f--){
        if(copia[f][col]=='.'){ fila=f; break; }
    }
    if(fila==-1) return 0; // columna plena -> no és una jugada possible
    copia[fila][col] = token;
    // comprova guanyador
    int gg = guanyar(copia, "XXXX", "OOOO");
    return gg;
}

int mainminimax(Tauler *t){
  // NOTA: srand s'ha d'inicialitzar una vegada a main() (no aquí).
  printf("\n=== INICI MINIMAX ===\n");
  
  // Depuració: mostra l'estat actual del tauler
  printf("Tauler actual:\n");
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      printf("%c ", t->taula[i][j]);
    }
    printf("\n");
  }

  // 1) Si ordinador pot guanyar ara, fer-ho directament
  for(int col=0; col<N; col++){
      if(prova_tirada_i_comprova(t->taula, col, 'O') == 2){
          printf("Trobat guany immediat per O a la columna %d\n", col+1);
          return col;
      }
  }

  // 2) Si jugador pot guanyar a continuació, bloquejar (si és possible)
  for(int col=0; col<N; col++){
      if(prova_tirada_i_comprova(t->taula, col, 'X') == 1){
          // Intentem bloquejar posant O a la mateixa columna si hi cap
          int fila=-1;
          for(int f=N-1; f>=0; f--){
              if(t->taula[f][col]=='.'){ fila=f; break; }
          }
          if(fila!=-1){
              printf("Bloqueig d'amenaca: tiro O a la columna %d\n", col+1);
              return col;
          }
          // si no cap a la mateixa columna, no farem res aquí: el minimax pot trobar una solució alternativa
      }
  }
  
  // Cap guany ni amenaça immediata -> fem Minimax
  Node *arrel = malloc(sizeof(Node));
  copiaTauler(arrel->taula, t->taula);
  inicialitzacionode(arrel);
  
  printf("Creant arbre...\n");
  crearArbreRec(arrel, 1);
  
  printf("Assignant valors...\n");
  int valorfinal = assignovalors(arrel, 0);
  printf("Valor final: %d\n", valorfinal);
  
  int col = buscoquinacolumnaes(arrel, valorfinal);
  printf("Columna triada per Minimax: %d\n", col+1);
  
  alliberarEspai(arrel);
  printf("=== FI MINIMAX ===\n\n");
  
  return col;
}