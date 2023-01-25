#include "gamelib.h"


int main(){

  menuIniziale();

  do{
    //scelta dell'utente
    if(!impostatoGioco){
      printf("Premere 1 per impostare il gioco\nPremere 2 per avviare il gioco(impostare prima il gioco)\nPremere 3 per uscire dal gioco\n\n");
      printf("Inserire scelta:  ");
      scanf("%d", &sceltaIniziale);

      switch(sceltaIniziale){
        case 1:
          imposta_gioco();
          break;
        case 2:
          printf("\nIMPOSTARE PRIMA IL GIOCO\n\n");
          break;
        case 3:
          termina_gioco();
          break;
        default:
          printf("\nTasto non valido\n");
          break;
      }
    }
    else if(impostatoGioco && !gameover){
      printf("Sei pronto a partire?\nPremi 2, altrimenti premi 3 per rinunciare:  ");
      scanf("%d", &sceltaIniziale);
      switch(sceltaIniziale){
        case 2:
          gioca();
          break;
        case 3:
          termina_gioco();
          break;
        default:
          printf("\nTasto non valido\n");
          break;
      }
    }
    else if(gameover){
      printf("\nLa partita è finita, terminare il gioco premendo 3: ");
      scanf("%d", &sceltaIniziale);
      switch(sceltaIniziale){
        case 3:
          termina_gioco();
          break;
        default:
          printf("\nTasto non valido\n");
          break;
      }
    }
  }while(sceltaIniziale!=3);
}
