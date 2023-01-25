#include <stdbool.h>
#include <stdio.h>
//colori
#define RED "\x1b[31m"
#define BLU "\x1b[34m"
#define YELLOW "\x1b[33m"
#define GREEN "\x1b[32m"
#define BLACK "\e[0;30m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define ROSE "\x1b[35m"
#define WHITE "\e[0;37m"
#define KNORMAL "\x1b[0m"








//enum
enum Stato_giocatore{astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore{rosso, blu, giallo, verde, nero, viola, marrone, celeste, rosa, bianco};
enum Tipo_stanza{vuota, quest_semplice, quest_complicata, botola};

//variabili
bool gameover;
bool impostatoGioco;
int sceltaIniziale;




//strutture
struct Stanza{
  struct Stanza *avanti;
  struct Stanza *sinistra;
  struct Stanza *destra;
  struct Stanza *stanza_precedente;
  struct Stanza *next;
  enum Tipo_stanza tipo;
  bool emergenza_chiamata;
};

struct Giocatore{
  struct Stanza *posizione;
  enum Stato_giocatore stato;
  enum Nome_giocatore nome;
};



//funzioni
void menuIniziale();
void imposta_gioco();
void gioca();
void termina_gioco();