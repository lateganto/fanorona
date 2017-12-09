#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dati_globali.h"

partita nuova_partita(rec *recptr,
                      config *imp) {//recptr contiene tutte le informazioni riguardo il tempo "Record" realizzato durante la partita
    //imp contiene tutte le informazioni riguardo alle impostazioni di gioco
    partita *gameptr;
    partita game = {FALSO, {0, 0, 0, 0, ""}, "", 0}; //Contiene tutte le informazioni relative alla partita giocata
    gameptr = &game;

    creare_matrice_pedine(gameptr, imp);
    creare_matrice_grafica();
    stampare_interfaccia_campo(gameptr);
    //Copio i nomi dei giocatori dalla struttura dati "config"
    strcpy(gameptr->nomeg1, imp->ng1);
    strcpy(gameptr->nomeg2, imp->ng2);
    effettuare_mossa(gameptr);
    return game;
}

partita carica_partita(FILE *fpt) {//dichiarazione variabili locali
    int scelta;//Puo' assumere valori compresi tra 0 e 4 e codESC
    boolean esito = FALSO;//Indica se posso caricare la partita selezionata

    partita part;
    partita *partptr;
    partptr = &part;

    do {
        stampare_interfaccia_carica_partita();
        leggere_file_salvataggio(fpt);
        if ((fpt = fopen("salvataggio.dat", "rb")) !=
            NULL)//Se il file esiste esegue le operazioni per caricare la partita
        {
            scelta = scegliere_slot();
            if (scelta != codESC)//Se l'utente non ha premuto il tasto ESC
            {
                esito = controllare_slot_caricamento(fpt, scelta);
                if (esito == VERO)//Se l'utente ha scelto uno slot non vuoto
                {
                    part = estrarre_partita(fpt, scelta);
                    riprendere_partita(partptr);
                }
            } else esito = VERO;//L'utente ha premuto il tasto ESC, assegno a "esito" il valore "VERO" per uscire dal ciclo
        } else esito = VERO;
    } while (esito != VERO);
    return part;
}

void salva_partita(FILE *fpt, partita *p) {//dichiarazione variabili locali
    int scelta;//Puo' assumere valori compresi tra 0 e 4 e codESC
    boolean esito;//

    do {
        stampare_interfaccia_salva_partita();
        if ((fpt = fopen("salvataggio.dat", "rb")) == NULL)//se il file non e' presente lo creo
            creare_file_salvataggio(fpt);
        leggere_file_salvataggio(fpt);
        scelta = scegliere_slot();
        if (scelta != codESC)//Se l'utente non ha premuto il tasto ESC
        {
            esito = controllare_slot_salvataggio(fpt, scelta);
            if (esito ==
                VERO)//Se l'utente ha scelto uno slot vuoto o ha deciso di sovrascrivere uno slot, "esito" cambia il suo valore a "VERO"
                effettuare_salvataggio_partita(fpt, scelta, p);
        } else esito = VERO;//L'utente ha premuto il tasto ESC, assegno a "esito" il valore "VERO" per uscire dal ciclo
    } while (esito != VERO);
    return;
}

void impostazioni(config *imp) {//dichiarazione variabili locali
    int cgx = 11, scelta;//"cgx" indica la riga grafica a video, "scelta" puo' assumere valori da 0 a 2

    do {
        stampare_interfaccia_impostazioni();
        GotoXY(2, cgx);
        printf("%c Inserire scelta: ", 16);
        scanf("%d", &scelta);
        if ((scelta < 0) || (scelta > 2)) {//Stampo messaggio di errore
            printf("\nERRORE! Inserire un numero compreso tra 0 e 2\n");
            system("pause");
        } else if (scelta == 1)
            cambiare_simbolo_pedine(imp);
        else if (scelta == 2)
            cambiare_nome_giocatori(imp);
    } while (scelta != 0);
    return;
}

void topten() {//dichiarazione variabili locali
    FILE *fpt;
    int cgx = 18;

    stampare_interfaccia_topten();
    GotoXY(1, cgx);
    if ((fpt = fopen("topten.txt", "r")) == NULL)//Se il file non esiste, stampo a video il messaggio di errore
        printf("Errore! File TopTen inesistente\n");
    else {
        leggere_file_topten(fpt);
        printf("\n\n");
        fclose(fpt);
    }
    GotoXY(1, cgx + 1);
    system("pause");
    return;
}
