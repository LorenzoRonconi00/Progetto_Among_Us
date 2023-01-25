#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "gamelib.h"




//variabili static
static time_t t;
static int numeroGiocatori;
static int indice;
static int temp;
static int counterStato;
static int xStatoRandom;
static unsigned short int quest_da_finire;
static unsigned short int questFinite=0;
static int indiceTurnoCasuale;
static int numeroCasualeStanza;
static int controlloGiocatoriDaUccidere;
static int impostoriScoperti=0;
static int astronautiUccisi=0;
static int contaBotole;
static int contaStanze;
static int randomBotola;
static bool botolaDecisa;


static struct Giocatore *giocatori=NULL;
static struct Stanza *stanza_inizio=NULL;
static struct Stanza *lista_stanze=NULL;
static struct Stanza *puntatorePerBotole=NULL;
static struct Stanza *ricordaStanzaIniziale=NULL;



//funzioni static
static void stampaStato(int stato);      //riga 798
static void stampaColore(int colore);    //riga 823
static void stampa_giocatori();          //riga 737
static void inizia_gioco();              //riga 786
static void stampaTipoStanza(int tipo);  //riga 886
static void creaPrimaStanza();           //riga 718
static void avanza(int scelta);          //riga 580
static void impostaStanza(struct Stanza *puntatore, struct Stanza *puntatorePrecedente);      //riga 703
static void controlloGiocatoriStanza();  //riga 341
static void pulisciTurno();              //riga 323
static void esegui_quest(int array[], int indice);     //riga 649
static int probabilitaTipoStanza();      //riga 863
static void sabotaggio(int array[], int indice);    //riga 679
static void uccidi_astronauta();         //riga 505
static void chiamata_emergenza();        //riga 370
static void usa_botola();                //riga 343



///////////////
//MENU INIZIALE
void menuIniziale(){

  //scritta del gioco
  printf("\n\n\n\n\n        ████████╗██████╗░░█████╗░███╗░░██╗███╗░░██╗░█████╗░██╗\n        ╚══██╔══╝██╔══██╗██╔══██╗████╗░██║████╗░██║██╔══██╗██║\n        ░░░██║░░░██████╔╝███████║██╔██╗██║██╔██╗██║██║░░██║██║\n        ░░░██║░░░██║░░██║██║░░██║██║░╚███║██║░╚███║╚█████╔╝██║\n        ░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝╚═╝░░╚══╝░╚════╝░╚═╝\n\n\n\n");

  //descrizione e storia del gioco
  printf("        BENVENUTO NEL GIOCO CHE TI TRASPORTERA' NELLO SPAZIO\n\n\n\n\n ");
  printf("Siete un gruppo di astronauti in viaggio sull'astronave Kelt.\nIl vostro obiettivo e' riuscire a completare tutte le quest per il mantenimento della nave.\n\nATTENZIONE: Tra di voi si celano anche degli impostori, il cui scopo e' quello di eliminare ogni singolo componente della truppa galattica. Riuscirete ad arrivare a destinazione?\n\n\n");

}
//////////////




///////////////
//IMPOSTA GIOCO
void imposta_gioco(){

  static int confermaImpostazione;
  srand((unsigned) time(&t));
  printf("Iniziamo dalle basi...\nLa nave Skelt ha bisogno di almeno 4 astronauti, ma non esagerare (massimo 10)\n Inserire il numero di giocatori:  ");
  scanf("%d", &numeroGiocatori);
  //controllo se il numero di giocatori è corretto
  if(numeroGiocatori<4 || numeroGiocatori>10){
    while(numeroGiocatori<4 || numeroGiocatori>10){
      printf("\nNumero di astronauti errato, inserire un numero compreso tra 4 e 10:  ");
      scanf("%d", &numeroGiocatori);
    }
  }
  printf("\nNumero di astronauti corretto, siete in %d\n", numeroGiocatori);

  //probabilità di impostori per numero di giocatori
  if(numeroGiocatori<6){
    counterStato=1;
  }
  //assegno le varie possibilita' di impostori
  else if(numeroGiocatori>5 && numeroGiocatori<9){
    xStatoRandom=rand()%3;
    if(xStatoRandom<2){
      counterStato=2;
    }
    else{
      counterStato=1;
    }
  }
  else if(numeroGiocatori>=9){
    xStatoRandom=rand()%5;
    if(xStatoRandom>=0 && xStatoRandom<3){
      counterStato=3;
    }
    else{
      counterStato=2;
    }
  }
  //array di giocatori
  giocatori=(struct Giocatore *) calloc(numeroGiocatori, sizeof(struct Giocatore));
  //creazione della prima stanza
  creaPrimaStanza();
  //stampa dei giocatori
  pulisciTurno();
  stampa_giocatori();
  //inserimento delle quest
  printf("Inserire il numero di quest da completare:  ");
  scanf("%hu", &quest_da_finire);

  //controllo del numero di quest
  if(quest_da_finire<1){
    while(quest_da_finire<1){
      printf("\nNumero di quest troppo basse(almeno 1), reinserire: ");
      scanf("%hu", &quest_da_finire);
    }
  }
  printf("\n\nAvete %hu quest da completare prima di portare la nave a destinazione\nIn bocca al lupo astronauti, salite nella Skelt e percorrete l'universo!!\n\n\n", quest_da_finire);

  pulisciTurno();
  printf("\nImpostazioni corrette, premere 1 per confermare o 2(o un altro tasto) per reimpostare: ");
  scanf("%d", &confermaImpostazione);
  switch(confermaImpostazione){
    case 1:
      pulisciTurno();
      inizia_gioco();
      break;
    case 2:
      free(giocatori);
      giocatori=NULL;
      free(stanza_inizio);
      stanza_inizio=NULL;
      pulisciTurno();
      break;
  }
}
//////////////




////////////
//GIOCA
void gioca(){

  //stampa partenza
  printf("  ___________________\n  |                 |\n  | P A R T E N Z A |\n  |_________________|\n\n\n");

  static int contatoreTurni=0;
  static int numeroAstronauti=0;
  static int numeroImpostori=0;
  static int mossaGiocatore;
  static int sceltaAvanza;
  static int saltaTurno=0;
  int arrayTurni[numeroGiocatori];
  lista_stanze=stanza_inizio;

  //CONTROLLO ASTRONAUTI E IMPOSTORI
  for(int i=0;i<numeroGiocatori;i++){
    if(giocatori[i].stato==0){
      numeroAstronauti++;
    }
    else if(giocatori[i].stato==1){
      numeroImpostori++;
    }
  }

  //CONTROLLO NUMERO ASTRONAUTI E NUMERO QUEST
  while(quest_da_finire>questFinite && (numeroAstronauti-astronautiUccisi>numeroImpostori) && numeroImpostori>impostoriScoperti){

    //RINIZIO DEL GIRO
    if(contatoreTurni<numeroGiocatori){

      //TURNO CASUALE
      indiceTurnoCasuale=rand()%numeroGiocatori;

      //IL GIOCATORE DEVE ANCORA GIOCARE IL TURNO?
      if(arrayTurni[indiceTurnoCasuale]!=1 && arrayTurni[indiceTurnoCasuale]!=2 && arrayTurni[indiceTurnoCasuale]!=3){

        //ASTRONAUTA
        if(giocatori[indiceTurnoCasuale].stato==0){
          printf("\n\nE' il turno del ");
          stampaColore(giocatori[indiceTurnoCasuale].nome);
          printf("\nIl tipo della stanza e': ");
          lista_stanze=giocatori[indiceTurnoCasuale].posizione;
          stampaTipoStanza(lista_stanze->tipo);

          //CONTROLLO GIOCATORI NELLA STANZA
          controlloGiocatoriStanza();
          printf("\nPremere 1 per avanzare\nPremere 2 per eseguire la quest della stanza\nPremere 3 per effettuare la chiamata d'emergenza(solo nel caso in cui ci sia un assassinato)\nPremere qualsiasi altro tasto per non effettuare operazioni\n");
          printf("\nCosa vuoi fare: ");
          scanf("%d", &mossaGiocatore);
          switch(mossaGiocatore){
            case 1:
              printf("\nPremere 1 per andare avanti\nPremere 2 per andare a destra\nPremere 3 per andare a sinistra\nPremere 4 per rimanere fermo");
              printf("\nInserire scelta: ");
              scanf("%d", &sceltaAvanza);
              avanza(sceltaAvanza);
              arrayTurni[indiceTurnoCasuale]=1;
              break;
            case 2:
              printf("\nStai eseguendo la quest\n");
              esegui_quest(arrayTurni, indiceTurnoCasuale);
              break;
            case 3:
              chiamata_emergenza();
              arrayTurni[indiceTurnoCasuale]=1;
              break;
            default:
              printf("\nTasto errato, salti il turno\n");
              arrayTurni[indiceTurnoCasuale]=1;
              break;
          }
          contatoreTurni++;
        }

        //IMPOSTORE
        else if(giocatori[indiceTurnoCasuale].stato==1){
          printf("\n\nE' il turno del ");
          stampaColore(giocatori[indiceTurnoCasuale].nome);
          printf("\nIl tipo della stanza e': ");
          lista_stanze=giocatori[indiceTurnoCasuale].posizione;
          stampaTipoStanza(lista_stanze->tipo);

          //CONTROLLO GIOCATORI NELLA STANZA
          controlloGiocatoriStanza();
          printf("\nPremere 1 per avanzare\nPremere 2 per effettuare la chiamata d'emergenza\nPremere 3 per uccidere un astronauta\nPremere 4 per usare la botola\nPremere 5 per effettuare un sabotaggio\nPremere qualsiasi per non effettuare operazioni\n");
          printf("\nCosa vuoi fare: ");
          scanf("%d", &mossaGiocatore);
          switch(mossaGiocatore){
            case 1:
              printf("\nPremere 1 per andare avanti\nPremere 2 per andare a destra\nPremere 3 per andare a sinistra\nPremere 4 per rimanere fermo");
              printf("\nInserire scelta: ");
              scanf("%d", &sceltaAvanza);
              avanza(sceltaAvanza);
              arrayTurni[indiceTurnoCasuale]=1;
              break;
            case 2:
              chiamata_emergenza();
              arrayTurni[indiceTurnoCasuale]=1;
              break;
            case 3:
              uccidi_astronauta();
              arrayTurni[indiceTurnoCasuale]=1;
              break;
            case 4:
              usa_botola();
              arrayTurni[indiceTurnoCasuale]=1;
              break;
            case 5:
              printf("\nStai sabotando");
              sabotaggio(arrayTurni, indiceTurnoCasuale);
              printf("\nOra la stanza e' diventata ");
              stampaTipoStanza(lista_stanze->tipo);
              break;
            default:
              printf("\nTasto errato, salti il turno\n");
              arrayTurni[indiceTurnoCasuale]=1;
              break;
          }
          contatoreTurni++;
        }
        //ASSASSINATO
        else if(giocatori[indiceTurnoCasuale].stato==2){
          printf("\nE' il turno del ");
          stampaColore(giocatori[indiceTurnoCasuale].nome);
          printf("\n\nSei stato assassinato\n");
          arrayTurni[indiceTurnoCasuale]=1;
          contatoreTurni++;
        }
        //DEFENESTRATO
        else if(giocatori[indiceTurnoCasuale].stato==3){
          printf("\nE' il turno del ");
          stampaColore(giocatori[indiceTurnoCasuale].nome);
          printf("\n\nSei stato defenestrato\n");
          arrayTurni[indiceTurnoCasuale]=1;
          contatoreTurni++;
        }
        //se le quest sono state finite
        if((quest_da_finire-questFinite)<=0){
          printf("\nRimangono 0 quest\n\n\n           GLI ASTRONAUTI HANNO VINTO!!!\n\n");
          gameover=true;
        }
        else{
          printf("\nRimangono %d quest\n", quest_da_finire-questFinite);
        }
        pulisciTurno();
      }
    }
    //INIZIALIZZO A 0 SALTATURNO E DIMINUISCO TUTTI GLI ELEMENTI DI 1
    else{
      saltaTurno=0;
      for(int i=0;i<numeroGiocatori;i++){
        arrayTurni[i]--;
      }
      //SE DOPO LA SOTTRAZIONE DI UNO C'È' UN ELEMENTO UGUALE A 1/2 VUOL DIRE CHE QUEL GIOCATORE HA ESEGUITO UNA QUEST
      for(int i=0;i<numeroGiocatori;i++){
        if(arrayTurni[i]>=1){
          saltaTurno++;
        }
      }
      //PER EVITARE CHE IL CONTATORE RIMANGA MINORE DEL NUMERO DI GIOCATORI QUANDO C'È UN GIOCATORE CHE E' UGUALE A 1 O 2, CIOE DEVE SALTARE UN TURNO
      contatoreTurni=0+saltaTurno;
    }
  }
  //SE TUTTI GLI IMPOSTORI SONO STATI SCOPERTI
  if(numeroImpostori==impostoriScoperti){
    printf("\n\nTutti gli impostori sono stati defenestrati, gli astronauti hanno vinto!!\n\n");
    gameover=true;
  }
  //SE TUTTI GLI ASTRONAUTI TRANNE 1 SONO STATI UCCISI
  else if(numeroAstronauti-astronautiUccisi==numeroImpostori){
    printf("\n\nGli impostori hanno vinto!!\n\n");
    gameover=true;
  }
}
//////////



////////////////
//TERMINA GIOCO
void termina_gioco(){
  printf("\n\nOra deallocheremo lo spazio creato in memoria\n\n");
  free(giocatori);
  giocatori=NULL;
  free(stanza_inizio);
  stanza_inizio=NULL;
  free(lista_stanze);
  lista_stanze=NULL;
}
/////////




///////////
//CLEAR
static void pulisciTurno(){
  static unsigned int tasto;
  printf("\nPremere qualsiasi numero per andare avanti: ");
  scanf("%d", &tasto);
  switch(tasto){
    default:
      system("clear");
      break;
  }
}
/////////////



//////////////
//USA BOTOLA
static void usa_botola(){

  contaStanze=0;
  contaBotole=0;
  botolaDecisa=false;
  stanza_inizio=ricordaStanzaIniziale;
  //se l'impostore si trova in una stanza con botola
  if(giocatori[indiceTurnoCasuale].posizione->tipo==3){
    printf("\nHai attivato la botola\n");
    //scorro la lista contando le stanze con botola e le stanze totali
    while(stanza_inizio!=NULL){
      //se la stanza ha una botola e se la stanza in cui si trova è diversa da quella dell'impostore
      if(stanza_inizio->tipo==3 && stanza_inizio!=giocatori[indiceTurnoCasuale].posizione){
        contaBotole++;
      }
      contaStanze++;
      //vado avanti con il puntatore
      stanza_inizio=stanza_inizio->next;
    }
    stanza_inizio=ricordaStanzaIniziale;
    int arrayBotole[contaStanze];
    //inizializzo l'array con 1 se botola, 0 non botola
    for(int i=0;i<contaStanze;i++){
      if(stanza_inizio->tipo==3 && stanza_inizio!=giocatori[indiceTurnoCasuale].posizione){
        arrayBotole[i]=1;
      }
      else{
        arrayBotole[i]=0;
      }
      stanza_inizio=stanza_inizio->next;
    }
    stanza_inizio=ricordaStanzaIniziale;
    printf("\nCi sono %d stanze totali\n", contaStanze);
    //se ci sono stanze con botole
    if(contaBotole>0){
      printf("Di cui %d con una botola\nVerrai portato in una stanza casuale con botola...\n\n", contaBotole);
      //variabile random per scegliere la stanza
      randomBotola=rand()%contaStanze;
      while(!botolaDecisa){
        //se l'elemento dell'array indica che la stanza non ha una botola(oppure è la stanza dell'impostore)
        if(arrayBotole[randomBotola]==0){
          randomBotola=rand()%contaStanze;
        }
        else{
          //se invece ha una botola scorro la lista fino ad arrivare alla stanza con botola
          while(randomBotola!=0){
            stanza_inizio=stanza_inizio->next;
            randomBotola--;
          }
          //esco dal ciclo
          botolaDecisa=true;
        }
      }
      //assegno la nuova posizione al giocatore
      giocatori[indiceTurnoCasuale].posizione=stanza_inizio;
      printf("\nSei stato portato nella stanza ");
      stampaTipoStanza(giocatori[indiceTurnoCasuale].posizione->tipo);
      printf(", in posizione %p", giocatori[indiceTurnoCasuale].posizione);
    }
    //se non ci sono altre stanze con botola lo trasporto in una stanza casuale
    else{
      printf("\nMa non ci sono altre stanze con botola\n");
      printf("\nVerrai trasportato in una stanza casuale...\n\n");
      randomBotola=rand()%contaStanze;
      while(randomBotola!=0){
        stanza_inizio=stanza_inizio->next;
        randomBotola--;
      }
      giocatori[indiceTurnoCasuale].posizione=stanza_inizio;
      printf("\nSei stato portato nella stanza ");
      stampaTipoStanza(giocatori[indiceTurnoCasuale].posizione->tipo);
      printf(", in posizione %p", giocatori[indiceTurnoCasuale].posizione);
    }
  }
  else{
    printf("\nNessuna botola utilizzabile in questa stanza\n");
  }
}




////////////////////////////
//CONTROLLO GIOCATORI STANZA
static void controlloGiocatoriStanza(){
  controlloGiocatoriDaUccidere=0;
  for(int i=0;i<numeroGiocatori;i++){
    //controllo tutti tranne il giocatore stesso
    if(i!=indiceTurnoCasuale){
      if(giocatori[indiceTurnoCasuale].posizione==giocatori[i].posizione){
        //se ci sono degli assassinati
        if(giocatori[i].stato==2){
          printf("\nIn questa stanza il ");
          stampaColore(giocatori[i].nome);
          printf(" e' stato assassinato\n");
        }
        else{
          //controllo tutti i restanti giocatori nella stanza, tranne i defenestrati
          if(giocatori[i].stato!=3){
            if(giocatori[i].stato==0){
              controlloGiocatoriDaUccidere++;
              printf("\nIn questa stanza c'è anche il giocatore ");
              stampaColore(giocatori[i].nome);
            }
            else if(giocatori[i].stato==1){
              printf("\nIn questa stanza c'è anche il giocatore ");
              stampaColore(giocatori[i].nome);
            }
          }
        }
      }
    }
  }
}
///////////////////////////


/////////////////////////
//CHIAMATA D'EMERGENZA
static void chiamata_emergenza(){
  static int scelta;
  static int counterAssassinati;
  static int probabilitaDefenestratoImpostore;
  static int probabilitaDefenestratoAstronauta;
  static int randomDefenestrato;
  static bool soloUno;
  //se la stanza ha il flag == false
  if(giocatori[indiceTurnoCasuale].posizione->emergenza_chiamata==false){
    counterAssassinati=0;
    for(int i=0;i<numeroGiocatori;i++){
      //aumento il numero degli assassinati nel caso ci siano e si trovino nella stessa stanza
      if(giocatori[i].stato==2 && giocatori[i].posizione==giocatori[indiceTurnoCasuale].posizione){
        counterAssassinati++;
      }
    }
    //se ci sono assassinati
    if(counterAssassinati>0){
      printf("\nCi sono %d assassinati\n", counterAssassinati);
      printf("\nVuoi fare la chiamata d'emergenza?\nPremere 1 per attivarla, premere 2 per NON attivarla: ");
      scanf("%d", &scelta);
      switch(scelta){
        case 1:
          printf("\nChiamata d'emergenza attivata\n");
          probabilitaDefenestratoImpostore=30;
          probabilitaDefenestratoAstronauta=30;
          for(int i=0;i<numeroGiocatori;i++){
            //scorro l'array dei giocatori vedendo quali si trovano nella stanza insieme al giocatore che ha chiamato l'emergenza
            if(giocatori[indiceTurnoCasuale].posizione==giocatori[i].posizione){
              //se ASTRONAUTA
              if(giocatori[i].stato==0){
                for(int j=0;j<numeroGiocatori;j++){
                  //scorro con un indice diverso, vedendo se ci sono altri astronauti/impostori(tranne il giocatore stesso)
                  if(i!=j){
                    if(giocatori[j].posizione==giocatori[indiceTurnoCasuale].posizione){
                      if(giocatori[j].stato==1){
                        probabilitaDefenestratoAstronauta+=20;
                      }
                      else if(giocatori[j].stato==0){
                        probabilitaDefenestratoAstronauta-=30;
                      }
                    }
                  }
                }
              }
              //se IMPOSTORE
              else if(giocatori[i].stato==1){
                for(int j=0;j<numeroGiocatori;j++){
                  if(i!=j){
                    if(giocatori[j].posizione==giocatori[indiceTurnoCasuale].posizione){
                      if(giocatori[j].stato==0){
                        probabilitaDefenestratoImpostore+=20;
                      }
                      else if(giocatori[j].stato==1){
                        probabilitaDefenestratoImpostore-=30;
                      }
                    }
                  }
                }
              }
            }
          }
          //lo inizializzo a false in modo che ogni volta che viene attivata solo un impostore/astronauta può essere defenestrato
          soloUno=false;
          //se ha una probabilita maggiore di essere defenestrato l'impostore
          if(probabilitaDefenestratoImpostore>probabilitaDefenestratoAstronauta){
            while(!soloUno){
              for(int i=0;i<numeroGiocatori;i++){
                //cerco gli impostori in quella stanza
                if(giocatori[i].posizione==giocatori[indiceTurnoCasuale].posizione){
                  if(giocatori[i].stato==1){
                    printf("\nIl giocatore ");
                    stampaColore(giocatori[i].nome);
                    printf(" è stato defenestrato\nEra un impostore!\n");
                    impostoriScoperti++;
                    giocatori[i].stato=3;
                    soloUno=true;
                  }
                }
              }
            }
          }
          //se ha una probabilità maggiore di essere defenestrato l'astronauta
          else if(probabilitaDefenestratoAstronauta>probabilitaDefenestratoImpostore){
            while(!soloUno){
              for(int i=0;i<numeroGiocatori;i++){
                //cerco gli astronauti in quella stanza
                if(giocatori[i].posizione==giocatori[indiceTurnoCasuale].posizione){
                  if(giocatori[i].stato==0){
                    printf("\nIl giocatore ");
                    stampaColore(giocatori[i].nome);
                    printf(" è stato defenestrato\nNon era un impostore!\n");
                    astronautiUccisi++;
                    giocatori[i].stato=3;
                    soloUno=true;
                  }
                }
              }
            }
          }
          //se hanno la stessa probabilità
          else if(probabilitaDefenestratoAstronauta==probabilitaDefenestratoImpostore){
            //faccio uno switch di un numero randomico per scegliere il defenestrato
            randomDefenestrato=rand()%2;
            switch(randomDefenestrato){
              case 0:
                while(!soloUno){
                  for(int i=0;i<numeroGiocatori;i++){
                    if(giocatori[i].posizione==giocatori[indiceTurnoCasuale].posizione){
                      if(giocatori[i].stato==0){
                        printf("\nIl giocatore ");
                        stampaColore(giocatori[i].nome);
                        printf(" è stato defenestrato\nNon era un impostore!\n");
                        astronautiUccisi++;
                        giocatori[i].stato=3;
                        soloUno=true;
                      }
                    }
                  }
                }
                break;
              case 1:
                while(!soloUno){
                  for(int i=0;i<numeroGiocatori;i++){
                    if(giocatori[i].posizione==giocatori[indiceTurnoCasuale].posizione){
                      if(giocatori[i].stato==1){
                        printf("\nIl giocatore ");
                        stampaColore(giocatori[i].nome);
                        printf(" è stato defenestrato\nEra un impostore!\n");
                        impostoriScoperti++;
                        giocatori[i].stato=3;
                        soloUno=true;
                      }
                    }
                  }
                }
                break;
            }
          }
          //in quella stanza non si può più chiamare l'emergenza
          giocatori[indiceTurnoCasuale].posizione->emergenza_chiamata=true;
          break;
        case 2:
          printf("\nNon hai attivato la chiamata d'emergenza\n");
          break;
        default:
          printf("\nTasto non valido, salti il turno\n");
      }
    }
    else{
      printf("\nNon ci sono assassinati in questa stanza\n");
    }
  }
  else{
    printf("\nChiamata d'emergenza già attivata in questa stanza o l'impostore e' gia' stato scoperto\n");
  }
}




/////////////////////
//UCCIDI ASTRONAUTA
static void uccidi_astronauta(){
  static int sceltaUccisione;
  static bool haUcciso;
  static int probabilitaDefenestrato;
  static int probabilitaTotale;
  static int tieniConto;
  static int i;
  //se non ci sono astronauti da uccidere
  if(controlloGiocatoriDaUccidere==0){
    printf("\nNon ci sono astronauti da uccidere in questa stanza\n");
  }
  else{
    i=0;
    haUcciso=false;
    //scorro tutti gli astronauti nella stanza, scegliendo se ucciderli o meno
    while(i<numeroGiocatori && !haUcciso){
      //controllo tutti tranne il giocatore stesso
      if(i!=indiceTurnoCasuale){
        //se il giocatore si trova nella stessa stanza ed è un astronauta
        if(giocatori[indiceTurnoCasuale].posizione==giocatori[i].posizione && giocatori[i].stato==0){
          printf("\nPuoi uccidere il giocatore ");
          stampaColore(giocatori[i].nome);
          printf("\nPremere 1 per ucciderlo, premere 2 per andare avanti: ");
          scanf("%d", &sceltaUccisione);
          switch(sceltaUccisione){
            case 1:
              printf("\nHai ucciso il giocatore ");
              stampaColore(giocatori[i].nome);
              //il giocatore che viene ucciso non deve sommare la probabilita
              tieniConto=i;
              astronautiUccisi++;
              giocatori[i].stato=2;
              haUcciso=true;
              break;
            case 2:
              printf("\nNon hai ucciso il giocatore ");
              stampaColore(giocatori[i].nome);
              break;
            default:
              printf("\nTasto errato\n");
              break;
          }
        }
      }
      i++;
    }
    //se l'impostore ha ucciso un astronauta
    if(haUcciso){
      probabilitaDefenestrato=0;
      //scorro i giocatori vedendo quali astronauti si trovano nella stessa stanza e nella stanza precedente(tranne l'impostore stesso e l'assassinato)
      for(int i=0;i<numeroGiocatori;i++){
        if(i!=indiceTurnoCasuale && i!=tieniConto && giocatori[i].stato==0){
          if(giocatori[indiceTurnoCasuale].posizione==giocatori[i].posizione){
            probabilitaDefenestrato+=50;
          }
          else if(giocatori[indiceTurnoCasuale].posizione->stanza_precedente==giocatori[i].posizione){
            probabilitaDefenestrato+=20;
          }
        }
      }
    }
    probabilitaTotale=rand()%101;
    //se la probabilità è maggiore di zero e minore di quella di essere defenestrato
    if(probabilitaTotale>0 && probabilitaTotale<=probabilitaDefenestrato){
      printf("\n\nSei stato scoperto e defenestrato!!\n\n");
      giocatori[indiceTurnoCasuale].posizione->emergenza_chiamata=true;
      giocatori[indiceTurnoCasuale].stato=3;
      impostoriScoperti++;
    }
    else{
      printf("\n\nNon sei stato scoperto\n\n");
    }
  }
}
/////////////////////



///////////////
//AVANZA
static void avanza(int scelta){
  switch(scelta){
    //AVANTI
    case 1:
      lista_stanze=giocatori[indiceTurnoCasuale].posizione;
      //se non esiste
      if(lista_stanze->avanti==NULL){
        lista_stanze->avanti=(struct Stanza *) malloc(sizeof(struct Stanza));
        impostaStanza(lista_stanze->avanti, lista_stanze);
        printf("\nSei andato avanti nella stanza ");
        stampaTipoStanza(lista_stanze->avanti->tipo);
        giocatori[indiceTurnoCasuale].posizione=lista_stanze->avanti;
        puntatorePerBotole->next=giocatori[indiceTurnoCasuale].posizione;
        puntatorePerBotole=giocatori[indiceTurnoCasuale].posizione;
      }
      //se esiste già
      else{
        printf("\nSei andato avanti nella stanza ");
        stampaTipoStanza(lista_stanze->avanti->tipo);
        giocatori[indiceTurnoCasuale].posizione=lista_stanze->avanti;
      }
      controlloGiocatoriStanza();
      break;
    //DESTRA
    case 2:
      lista_stanze=giocatori[indiceTurnoCasuale].posizione;
      if(lista_stanze->destra==NULL){
        lista_stanze->destra=(struct Stanza *) malloc(sizeof(struct Stanza));
        impostaStanza(lista_stanze->destra, lista_stanze);
        printf("\nSei andato a destra nella stanza ");
        stampaTipoStanza(lista_stanze->destra->tipo);
        giocatori[indiceTurnoCasuale].posizione=lista_stanze->destra;
        puntatorePerBotole->next=giocatori[indiceTurnoCasuale].posizione;
        puntatorePerBotole=giocatori[indiceTurnoCasuale].posizione;
      }
      else{
        printf("\nSei andato a destra nella stanza ");
        stampaTipoStanza(lista_stanze->destra->tipo);
        giocatori[indiceTurnoCasuale].posizione=lista_stanze->destra;
      }
      controlloGiocatoriStanza();
      break;
    //SINISTRA
    case 3:
      lista_stanze=giocatori[indiceTurnoCasuale].posizione;
      if(lista_stanze->sinistra==NULL){
        lista_stanze->sinistra=(struct Stanza *) malloc(sizeof(struct Stanza));
        impostaStanza(lista_stanze->sinistra, lista_stanze);
        printf("\nSei andato a sinistra nella stanza ");
        stampaTipoStanza(lista_stanze->sinistra->tipo);
        giocatori[indiceTurnoCasuale].posizione=lista_stanze->sinistra;
        puntatorePerBotole->next=giocatori[indiceTurnoCasuale].posizione;
        puntatorePerBotole=giocatori[indiceTurnoCasuale].posizione;
      }
      else{
        printf("\nSei andato a sinistra nella stanza ");
        stampaTipoStanza(lista_stanze->sinistra->tipo);
        giocatori[indiceTurnoCasuale].posizione=lista_stanze->sinistra;
      }
      controlloGiocatoriStanza();
      break;
      //FERMO
    case 4:
      lista_stanze=giocatori[indiceTurnoCasuale].posizione;
      printf("\nRimani fermo nella stanza ");
      stampaTipoStanza(lista_stanze->tipo);
      controlloGiocatoriStanza();
  }
}
////////////////




///////////////
//ESEGUI QUEST
static void esegui_quest(int array[], int indice){

  //se la stanza ha quest semplice aspetta un turno ed aumenta le quest fatte di 1
  if(giocatori[indiceTurnoCasuale].posizione->tipo==1){
    giocatori[indiceTurnoCasuale].posizione->tipo=0;
    printf("\nOra la stanza e' diventata ");
    stampaTipoStanza(lista_stanze->tipo);
    questFinite++;
    array[indice]=2;
  }
  //se la stanza ha quest complicata aspetta due turni ed aumenta le quest fatte di 2
  else if(giocatori[indiceTurnoCasuale].posizione->tipo==2){
    giocatori[indiceTurnoCasuale].posizione->tipo=0;
    printf("\nOra la stanza e' diventata ");
    stampaTipoStanza(lista_stanze->tipo);
    questFinite+=2;
    array[indice]=3;
  }
  //se la stanza è vuota o di tipo botola
  else if(giocatori[indiceTurnoCasuale].posizione->tipo==0 || giocatori[indiceTurnoCasuale].posizione->tipo==3){
    printf("\nNessuna quest in questa stanza!\n");
    array[indice]=1;
  }
}
//////////////////



/////////////////
//SABOTAGGIO
static void sabotaggio(int array[], int indice){
  //se la stanza ha quest semplice faccio aspettare solo un turno
  if(giocatori[indiceTurnoCasuale].posizione->tipo==1){
    giocatori[indiceTurnoCasuale].posizione->tipo=0;
    array[indice]=2;
  }
  //se la stanza ha quest complicata faccio aspettare due turni
  else if(giocatori[indiceTurnoCasuale].posizione->tipo==2){
    giocatori[indiceTurnoCasuale].posizione->tipo=0;
    array[indice]=3;
  }
  //se la stanza è vuota o di tipo botola
  else if(giocatori[indiceTurnoCasuale].posizione->tipo==0 || giocatori[indiceTurnoCasuale].posizione->tipo==3){
    printf("\nNessuna quest da sabotare in questa stanza!\n");
    array[indice]=1;
  }
}
////////////////



////////////////////
//IMPOSTA STANZA
//ogni volta che creo una nuova stanza la imposto, prendendo come parametri anche il puntatore alla stanza precedente
static void impostaStanza(struct Stanza *puntatore, struct Stanza *puntatorePrecedente){
  puntatore->avanti=NULL;
  puntatore->sinistra=NULL;
  puntatore->destra=NULL;
  puntatore->stanza_precedente=puntatorePrecedente;
  puntatore->next=NULL;
  puntatore->tipo=probabilitaTipoStanza();
  stanza_inizio->emergenza_chiamata=false;
}
//////////////




//////////////
//PRIMA STANZA
static void creaPrimaStanza(){
  stanza_inizio=(struct Stanza *) malloc(sizeof(struct Stanza));
  stanza_inizio->avanti=NULL;
  stanza_inizio->sinistra=NULL;
  stanza_inizio->destra=NULL;
  stanza_inizio->stanza_precedente=NULL;
  stanza_inizio->next=NULL;
  stanza_inizio->tipo=probabilitaTipoStanza();
  stanza_inizio->emergenza_chiamata=false;
  puntatorePerBotole=stanza_inizio;
  ricordaStanzaIniziale=stanza_inizio;
}
//////////////







//////////////////
//STAMPA GIOCATORI
static void stampa_giocatori(){

  //inizializzo l'array di giocatori
  for(int i=0;i<numeroGiocatori;i++){
    giocatori[i].posizione=stanza_inizio;
    if(counterStato!=0){
      giocatori[i].stato=rand()%2;
      if(giocatori[i].stato==1){
        counterStato--;
      }
    }
    else{
      giocatori[i].stato=0;
    }
    //l'elemento del nome è, inizialmente, uguale ad i

    giocatori[i].nome=i;
  }
  //uso le variabili temp ed indice per assegnare randomicamente i colori senza ripetizioni

  for(int i=0;i<numeroGiocatori;i++){
    temp=giocatori[i].nome;
    indice=rand()%numeroGiocatori;

    giocatori[i].nome=giocatori[indice].nome;
    giocatori[indice].nome=temp;
  }

  //stampo il contenuto dell'array
  for(int i=0;i<numeroGiocatori;i++){
    printf("\n%d GIOCATORE\n\n", i+1);
    printf("Posizione: %p", giocatori[i].posizione);
    printf("\nTipo della stanza: ");
    stampaTipoStanza(giocatori[i].posizione->tipo);
    printf("\nStato: ");
    stampaStato(giocatori[i].stato);
    printf("\nColore: ");
    stampaColore(giocatori[i].nome);
    printf("\n\n");
  }
}
///////////////





///////////////
//INIZIA GIOCO
static void inizia_gioco(){
  impostatoGioco=true;
}
/////////////





////////////////////////
//STAMPA STATO GIOCATORE
//utilizzata per stampare lo stato del giocatore
static void stampaStato(int stato){
  switch(stato){
    case 0:
      printf("astronauta");
      break;
    case 1:
      printf("impostore");
      break;
    case 2:
      printf("assassinato");
      break;
    case 3:
      printf("defenestrato");
      break;
  }
}
/////////////





/////////////////////////
//STAMPA COLORE GIOCATORE
//utilizzata per stampare il nome del giocatore
static void stampaColore(int colore){
  switch(colore){
    case 0:
      printf("%srosso", RED);
      printf("%s", KNORMAL);
      break;
    case 1:
      printf("%sblu", BLU);
      printf("%s", KNORMAL);
      break;
    case 2:
      printf("%sgiallo", YELLOW);
      printf("%s", KNORMAL);
      break;
    case 3:
      printf("%sverde", GREEN);
      printf("%s", KNORMAL);
      break;
    case 4:
      printf("%snero", BLACK);
      printf("%s", KNORMAL);
      break;
    case 5:
      printf("%sviola", PURPLE);
      printf("%s", KNORMAL);
      break;
    case 6:
      printf("marrone");
      break;
    case 7:
      printf("%sceleste", CYAN);
      printf("%s", KNORMAL);
      break;
    case 8:
      printf("%srosa", ROSE);
      printf("%s", KNORMAL);
      break;
    case 9:
      printf("%sbianco", WHITE);
      printf("%s", KNORMAL);
      break;
  }
}
////////////////



////////////////////////////////////////////////////
//GENERA IL TIPO DELLA STANZA SECONDO LE POSSIBILITA
static int probabilitaTipoStanza(){
  numeroCasualeStanza=rand()%101;
  if(numeroCasualeStanza>=0 && numeroCasualeStanza<=25){
    numeroCasualeStanza=3;
  }
  else if(numeroCasualeStanza>25 && numeroCasualeStanza<=40){
    numeroCasualeStanza=2;
  }
  else if(numeroCasualeStanza>40 && numeroCasualeStanza<=70){
    numeroCasualeStanza=1;
  }
  else if(numeroCasualeStanza>70){
    numeroCasualeStanza=0;
  }
  return numeroCasualeStanza;
}
//////////////////////////




////////////////////
//STAMPA TIPO STANZA
static void stampaTipoStanza(int tipo){
  if(tipo==3){
    printf("botola");
  }
  else if(tipo==2){
    printf("quest complicata");
  }
  else if(tipo==1){
    printf("quest semplice");
  }
  else if(tipo==0){
    printf("vuota");
  }
}
/////////////////