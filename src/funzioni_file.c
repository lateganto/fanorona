#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include <ctype.h>
#include <curses.h>
#include "dati_globali.h"

void creare_file_salvataggio(FILE *crea) {//dichiarazione variabili locali
    int i;//contatore
    partita part = {FALSO};

    crea = fopen("salvataggio.dat", "wb");
    i = 0;
    while (i < dimax) {//Scrivo su file una struttura dati di tipo "partita"
        fwrite(&part, sizeof(partita), 1, crea);
        i++;
    }
    fclose(crea);
    return;
}

void leggere_file_salvataggio(FILE *leggi) {//dichiarazione variabili locali
    int dimlocal;
    int x;
    partita part;

    if ((leggi = fopen("salvataggio.dat", "rb")) == NULL)//Se il file non esiste, stampo un messaggio d'errore
    {
        printf("\nErrore apertura file: ""File inesistente""\n");
        system("pause");
    } else {
        x = 0;
        while (!feof(leggi))//leggi fino alla fine del file
        {
            GotoXY(4, 5 + x);
            dimlocal = fread(&part, sizeof(partita), 1, leggi);
            if (dimlocal != 0) {//Se ho letto correttamente una struttura dati di tipo "partita" dal file
                if (part.save == FALSO)//controllo se la struttura dati contiene una partita salvata o e' vuota
                    printf("VUOTO");
                else printf("%s", part.nomepartita);
                x++;
            }
        }
    }
    fclose(leggi);
    return;
}

void effettuare_salvataggio_partita(FILE *f, int pos, partita *p) {//dichiarazione variabili locali
    int cgx = 12;//indica la riga grafica a video

    f = fopen("salvataggio.dat", "rb+");
    cancella_riga(cgx);
    GotoXY(1, cgx);
    printf("Inserire nome partita: ");
    scanf("%s", p->nomepartita);
    p->save = VERO;//save assume il valore "VERO" per indicare che la partita verr� salvata
    fseek(f, pos * sizeof(partita), SEEK_SET);
    fwrite(p, sizeof(partita), 1, f);
    fclose(f);
    return;
}

boolean controllare_slot_salvataggio(FILE *f, int pos) {//dichiarazione variabili locali
    boolean done = FALSO;//Riceve valore vero se ho inserito un carattere corretto
    partita part;
    int cgx = 12;//indica la riga grafica a video
    int scelta, lettera;// ricevono un carattere ASCII

    boolean test;

    f = fopen("salvataggio.dat", "rb");
    fseek(f, pos * sizeof(partita), SEEK_SET);
    fread(&part, sizeof(partita), 1, f);
    if (part.save == FALSO)
        test = VERO;
    else {
        do {
            GotoXY(1, cgx);
            printf("Stai cercando di salvare nello slot numero %d\n", pos);
            printf(" Sei sicuro di voler sovrascrivere lo slot? (s/n)  ");
            fflush(stdin);
            scelta = getch();
            lettera = islower(scelta);//controllo se il carattere inserito in scelta � minuscolo
            if (lettera != 0) {
                if (scelta == 115)// carattere ASCII 's'
                {
                    test = VERO;
                    done = VERO;
                } else if (scelta == 110) {
                    test = FALSO;// carattere ASCII 'n'
                    done = VERO;
                }
            } else {
                if (scelta == 83)//carattere ASCII 'S'
                {
                    test = VERO;
                    done = VERO;
                } else if (scelta == 78)// carattere ASCII 'N'
                {
                    test = FALSO;
                    done = VERO;
                }
            }
            if (done == FALSO) {
                cancella_riga(cgx);
                cancella_riga(cgx + 1);
                GotoXY(1, cgx);
                printf("Errore! Hai inserito un carattere non accettato: inserire s(si)/n(no)");
                GotoXY(1, cgx + 1);
                system("pause");
                cancella_riga(cgx);
                cancella_riga(cgx + 1);
            }
            cancella_riga(cgx);
            cancella_riga(cgx + 1);
        } while (done != VERO);
    }
    fclose(f);
    return test;
}

boolean controllare_slot_caricamento(FILE *f, int pos) {//dichiarazione variabili locali
    partita part;
    int cgx = 12;//Indica la riga grafica a video
    boolean test;

    f = fopen("salvataggio.dat", "rb");
    fseek(f, pos * sizeof(partita), SEEK_SET);
    fread(&part, sizeof(partita), 1, f);
    if (part.save == VERO)
        test = VERO;
    else {
        cancella_riga(cgx);
        GotoXY(1, cgx);
        printf("Errore! Impossibile caricare la partita: slot vuoto\n");
        GotoXY(1, cgx + 1);
        system("pause");
        cancella_riga(cgx);
        cancella_riga(cgx + 1);
    }
    fclose(f);
    return test;
}

partita estrarre_partita(FILE *f, int pos) {

    partita cpart;
    f = fopen("salvataggio.dat", "rb");
    fseek(f, pos * sizeof(partita), SEEK_SET);
    fread(&cpart, sizeof(partita), 1, f);
    return cpart;
}


void leggere_file_topten(FILE *txt) {//dichiarazione variabili locali
    rec tt;
    int i = 0, cgx = 7;//Contatore, indica la riga grafica a video

    txt = fopen("topten.txt", "r");
    while (!feof(txt)) {//Legge un tempo record dal file e lo stampa a video fino alla fine del file
        fscanf(txt, "%d %d %d %d %s\n", &tt.ora, &tt.minuti, &tt.secondi, &tt.diff_t, tt.nomerecord);
        GotoXY(4, cgx + i);
        printf("%02d:%02d:%02d %s", tt.ora, tt.minuti, tt.secondi, tt.nomerecord);
        i++;
    }
    fclose(txt);
    return;
}

void ordinamento_file_topten(partita *p) { //dichiarazione variabili globali
    int i, j, dim = 0;//"i e j" contatori, "dim" contiene il numero di record su file
    char c;//contiene un carattere letto dal file
    rec vet_tt[maxtt];//vettore di record di dimensione "maxtt" di tipo "rec", "w" variabile di scambio di tipo rec
    rec tt, w;//"w" variabile di scambio di tipo rec
    FILE *txt;

    txt = fopen("topten.txt", "r+");
    if ((c = getc(txt)) == EOF)//Leggo un carattere fino alla fine del file
    {
        fprintf(txt, "%d %d %d %d %s\n", p->recordp.ora, p->recordp.minuti, p->recordp.secondi, p->recordp.diff_t,
                p->recordp.nomerecord);
        fclose(txt);
    } else {
        rewind(txt);//calcolo quanti record ci sono sul file
        while (fscanf(txt, "%d %d %d %d %s\n", &tt.ora, &tt.minuti, &tt.secondi, &tt.diff_t, tt.nomerecord) > 0) {
            if (dim < (maxtt - 1)) {
                vet_tt[dim] = tt;
                dim++;//numero di record sul file
            }
        }
        fclose(txt);
        vet_tt[dim] = p->recordp;//inserisco alla fine del vettore l'ultimo nuovo record conseguito
        dim++;
        //Inzio bubble sort
        j = 1;
        while (j < dim) {
            i = 0;
            while (i < (dim - 1)) {
                if (vet_tt[i].diff_t > vet_tt[i + 1].diff_t) {
                    w = vet_tt[i];
                    vet_tt[i] = vet_tt[i + 1];
                    vet_tt[i + 1] = w;
                }
                i++;
            }
            j++;
        }
        //fine bubble sort
        txt = fopen("topten.txt", "w");//creo un nuovo file con il vettore ordinato
        j = 0;
        if (dim == maxtt)
            dim--;
        while (j < dim) {
            fprintf(txt, "%d %d %d %d %s\n", vet_tt[j].ora, vet_tt[j].minuti, vet_tt[j].secondi, vet_tt[j].diff_t,
                    vet_tt[j].nomerecord);
            j++;
        }
        fclose(txt);
    }
    return;
}
