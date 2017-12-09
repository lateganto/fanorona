#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dati_globali.h"

int main() {
    int scelta;

    FILE *Fsave;//puntatore al file di salvataggio

    //contiene tutte le informazioni di una nuova partita, non ancora salvata oppure di una partita caricata
    partita *lp;
    partita localp;
    lp = &localp;

    config *setting;
    config s = {2, 1, "Giocatore 1", "Giocatore 2"};//inizializzo impostazioni con i parametri di default
    setting = &s;

    rec *recordptr;
    rec r = {0, 0, 0, 0, ""};
    recordptr = &r;


    do {
        system("cls");
        printf("\n\n");
        stampare_titolo();
        stampare_interfaccia_menu();
        printf("%25c%c Inserire scelta: ", 32, 16);
        scanf("%d", &scelta);
        if ((scelta < 0) || (scelta > 5)) {
            printf("ERRORE! Inserire un numero compreso tra 0 e 5 \n");
            system("pause");
        } else if (scelta == 1)
            localp = nuova_partita(recordptr, setting);
        else if (scelta == 2)
            localp = carica_partita(Fsave);
        else if (scelta == 3)
            salva_partita(Fsave, lp);
        else if (scelta == 4)
            impostazioni(setting);
        else if (scelta == 5)
            topten();
    } while (scelta != 0);
    return 0;
}
