#include <stdio.h>
#include <stdlib.h>
#include "dati_globali.h"

void stampare_interfaccia_menu() {//dichiarazione variabili locali
    int i = 0;//Contatore

    printf("\n");
    printf("%22c%c", 32, 201);
    while (i < 34) { //Scrivo il bordo superiore
        printf("%c", 205);
        i++;
    }
    printf("%c\n", 187);
    printf("%22c%c%34c%c\n", 32, 186, 32, 186);
    printf("%22c%c  1. Nuova Partita%16c%c\n", 32, 186, 32, 186);
    printf("%22c%c  2. Carica Partita%15c%c\n", 32, 186, 32, 186);
    printf("%22c%c  3. Salva Partita%16c%c\n", 32, 186, 32, 186);
    printf("%22c%c  4. Impostazioni%17c%c\n", 32, 186, 32, 186);
    printf("%22c%c  5. Top Ten%22c%c\n", 32, 186, 32, 186);
    printf("%22c%c%34c%c\n", 32, 186, 32, 186);
    printf("%22c%c  0. Fine%25c%c\n", 32, 186, 32, 186);
    printf("%22c%c%34c%c\n", 32, 186, 32, 186);
    printf("%22c%c", 32, 200);
    i = 0;
    while (i < 34) {//Scrivo il bordo inferiore
        printf("%c", 205);
        i++;
    }
    printf("%c\n\n", 188);
    return;
}

void stampare_titolo() {
    //dichiarazione variabili locali
    FILE *fpt;
    char c, d;//"c" contiene un carattere letto dal file, "d" conserva il penultimo carattere letto dal file

    if ((fpt = fopen("titolo.txt", "r")) == NULL)//Se il file non esiste, stampo un messaggio di errore
        puts("File titolo non trovato");
    else {
        c = getc(fpt);
        d = c;
        printf("%21c", 32);
        while (c != EOF)//Leggo caratteri fino alla fine del file
        {
            if (d == '\n')
                printf("%21c", 32);
            printf("%c", c);
            d = c; //salvo il penultimo carattere
            c = getc(fpt); //leggo carattere successivo
        }
    }
    fclose(fpt);//chiudo il file
    printf("\n");
    return;
}

void stampare_interfaccia_campo(partita *p) {
    //dichiarazione varibili locali
    int i, j, num;//Contatori
    int ri, co;//righe,colonne della matrice_pedine
    FILE *fpt;//Puntatore al file "campo.txt"
    char c, d;//"c" contiene un carattere letto dal file, "d" conserva il penultimo carattere letto dal file
    int lettera;//contiene i caratteri ASCII delle lettere da stampare sul bordo sinistro del campo

    system("cls");
    printf("\n");
    if ((fpt = fopen("campo.txt", "r")) == NULL)//Se il file non esiste, stampo un messaggio di errore
        puts("File campo da gioco non trovato");
    else {
        printf("%5c", 32);
        i = 0;
        j = 0;
        num = 1;
        //Stampa i numeri da 1 a 9 in orizzontale sul campo
        while (i < 9) {
            printf("%d", num);
            num++;
            j = 0;
            while (j < 4) {
                printf(" ");
                j++;
            }
            i++;
        } //fine stampa i numeri
        printf("\n");
        i = 0;
        j = 0;
        printf("%3c%c", 32, 201);
        //stampa il bordo superiore
        while (i < 43) {
            printf("%c", 205);
            i++;
        }
        printf("%c", 187);
        //fine stampa bordo superiore
        printf("\n");
        //leggo il campo da gioco dal file di testo
        c = getc(fpt);
        d = c;
        lettera = 65;//lettera A in ASCII
        printf(" %c %c ", lettera, 186);
        lettera++;
        i = 0;
        ri = 0;
        co = 0;
        while (c != EOF)//Leggo caratteri fino alla fine del file
        {
            if (d == '\n') {
                i++;
                if (i == 4)//Ogni quattro righe lette dal file scrivo la lettera relativa alla riga corrispondente
                {
                    printf(" %c %c ", lettera, 186);
                    lettera++;
                    i = 0;
                    ri++;
                    co = 0;
                } else {
                    if ((lettera == 70) && (i == 1))//sono alla fine del campo
                    {
                        printf("   %c", 200);
                        c = 205;
                    } else printf("   %c ", 186);
                }
            }
            if (c ==
                'x')//In questo caso analizzo la "matrice_pedine" e in base al suo contenuto stampo il carattere ASCII corrispondente
            {
                if (p->matrice_pedine[ri * colonne + co] == 0)
                    c = 'O';
                else c = p->matrice_pedine[ri * colonne + co];
                co++;
            } else if (c == '\n')
                printf(" %c", 186);
            printf("%c", c);
            d = c; //salvo il penultimo carattere
            c = getc(fpt); //leggo carattere successivo
        }
        fclose(fpt);//chiudo il file
        i = 0;
        while (i < 42)//stampa bordo inferiore
        {
            printf("%c", 205);
            i++;
        }
        printf("%c", 188);
        //fine stampa bordo inferiore
    }
    return;
}

void stampare_interfaccia_carica_partita() {//dichiarazione variabili locali
    int i;//Contatore

    system("cls");
    printf("\n");
    printf("%33cCARICA PARTITA%c\n\n", 174, 175);
    i = 0;
    while (i < 80)//stampo a video una riga divisoria
    {
        printf("%c", 196);
        i++;
    }
    printf("\n");
    i = 0;
    while (i < 5)//Stampo a video il numero degli slot da 0 a 4
    {
        printf(" %d:\n", i);
        i++;
    }
    return;
}

void stampare_interfaccia_salva_partita() {//dichiarazione variabili locali
    int i;//Contatore

    system("cls");
    printf("\n");
    printf("%34cSALVA PARTITA%c\n\n", 174, 175);
    i = 0;
    while (i < 80)//Stampo a video linea divisoria
    {
        printf("%c", 196);
        i++;
    }
    printf("\n");
    i = 0;
    while (i < 5)//Stampo a video il numero degli slot da 0 a 4
    {
        printf(" %d:\n", i);
        i++;
    }
    return;
}

void stampare_interfaccia_impostazioni() {//dichiarazione variabili locali
    int scelta, i;//"scelta" potra' assumere valori tra 0 e 2 per muoversi del menu' impostazioni, "i" contatore

    system("cls");
    printf("\n");
    printf("%34cIMPOSTAZIONI%c\n\n", 174, 175);
    i = 0;
    while (i < 80)//Stampo a video linea divisoria
    {
        printf("%c", 196);
        i++;
    }
    printf("\n");
    printf("  1. Cambia simbolo delle pedine\n");
    printf("  2. Cambia nome Giocatori\n\n");
    printf("  0. Torna al menu' principale");
    return;
}

void stampare_interfaccia_topten() {//dichiarazione variabili locali
    int i, cgx = 5;// "i" contatore,indica la riga grafica a video

    system("cls");
    printf("\n");
    printf("%37cTOP TEN%c\n\n", 174, 175);
    i = 0;
    while (i < 80)//Stampo a video la linea divisoria
    {
        printf("%c", 196);
        i++;
    }
    GotoXY(4, cgx);
    printf("hh:mm:ss <nomerecord>\n\n");
    i = 1;
    while (i < maxtt)//Stampo a video numeri da 1 a 10
    {
        printf("%d.\n", i);
        i++;
    }
    return;
}

void stampare_interfaccia_cambiare_simbolo_pedine() {//dichiarazione variabili locali
    int i, scelta;//"i" contatore, "scelta" puo' assumere valori da 0 a 3

    system("cls");
    printf("\n%19cIMPOSTAZIONI-Cambia simbolo delle pedine%c\n\n", 174, 175);
    i = 0;
    while (i < 80)//Stampa a video linea divisoria
    {
        printf("%c", 196);
        i++;
    }
    printf("\n    Giocatore 1    Giocatore 2\n");
    printf("  1.     %c%13c%c\n", 2, 32, 1);
    printf("  2.     %c%13c%c\n", 15, 32, 14);
    printf("  3.     %c%13c%c\n\n", 36, 32, 156);
    printf("  0. Torna al menu' Impostazioni");
    return;
}

void stampare_interfaccia_cambiare_nomi_giocatori() {//dichiarazione variabili locali
    int i;

    system("cls");
    printf("\n");
    printf("%19cIMPOSTAZIONI-Cambia nome Giocatori%c\n\n", 174, 175);
    i = 0;
    while (i < 80)//Stampa a video linea divisoria
    {
        printf("%c", 196);
        i++;
    }
    printf("\n");
    printf("  Giocatore 1:\n");
    printf("  Giocatore 2:\n");
    return;
}


