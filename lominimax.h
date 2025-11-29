// inicialitzacio de lo minimax

#define nivell_max 6

int mainminimax(Tauler *t);

typedef struct node {
  struct node** fills; // array de fills (pointers a nodes)
  int n_fills;
  char taula[N+1][N+1];
  int valor;

  int nivell;

  int columna; //on tira
} Node;

void copiaTauler(char taula1[N+1][N+1], char taula2[N+1][N+1]);

void inicialitzacionode(Node* arrel);

void crearArbreRec( Node* pare, int nivell_actual);
  void reservarespai(Node *arrel);
    int calculaNumFills(char taula[N+1][N+1]);
  void creaNivell(Node* pare);
    Node* creaNode(Node* pare, int numDeFill);
    void tirada(Node* p, int numDeFill);
      void quinacolumnaes(Node* p, int numDeFill);

int funcioheuristica(Node* arrel);
int assignovalors(Node* arrel, int maximitzo);
int buscoquinacolumnaes(Node* arrel, int valorfinal );

void recorreArbreRecfetProfe(Node *arrel, FILE *fp);


void alliberarEspai(Node *arrel);
