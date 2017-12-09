#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h>
#include <ctype.h>
#include "dati_globali.h"
#include <time.h>
#include <ncurses.h>

//colonne, righe
void GotoXY(int x, int y) {
    COORD coord; // coordinate
    coord.X = x;
    coord.Y = y; // X and Y coordinate
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // mouovi dalle coordinate
}

void cancella_riga(int r) {//dichiarazione variabili locali
    int i = 0;//Contatore
    int maxC = 79;//Numero massimo di colonne

    while (i < maxC)//Cancella la riga partendo da destra verso sinistra
    {
        GotoXY(maxC - i, r);
        printf(" ");
        i++;
    }
    return;
}

void creare_matrice_pedine(partita *p, config *imp) {//dichiarazione variabili locali
    int i, j;//Contatori per righe e colonne

    p->pedinag1 = imp->pg1; //associo le impostazioni della pedina del giocatore 1 per l'inizio di una nuova partita
    p->pedinag2 = imp->pg2; //associo le impostazioni della pedina del giocatore 2 per l'inizio di una nuova partita

    i = 0;
    while (i < righe) {
        j = 0;
        while (j < colonne) {
            if (i <= 1)//dalla riga 0 alla riga 1 vengono associate le pedine del giocatere 2
                p->matrice_pedine[i * colonne + j] = p->pedinag2;
            else if (i >= 3)// dalla riga 3 alla riga 4 vengono associate le pedine del giocatore 1
                p->matrice_pedine[i * colonne + j] = p->pedinag1;
            else if (i == 2)// riga centrale
            {
                if (j < 4) {
                    if (j % 2 == 0)//controllo se la colonna e' pari
                        p->matrice_pedine[i * colonne +
                                          j] = p->pedinag2;//associo pedina giocatore 2 nelle colonne pari minore di 4
                    else
                        p->matrice_pedine[i * colonne +
                                          j] = p->pedinag1;//altrimenti associo pedina giocatore 1 nelle colonne dispari minore di 4
                } else if (j > 4) {
                    if (j % 2 == 0)//controllo se la colonna e' pari
                        p->matrice_pedine[i * colonne +
                                          j] = p->pedinag1;//associo pedina giocatore 1 nelle colonne pari maggiori di 4
                    else
                        p->matrice_pedine[i * colonne +
                                          j] = p->pedinag2;//altrimenti associo pedina giocatore 1 nelle colonne dispari maggiori di 4
                } else p->matrice_pedine[i * colonne + j] = 0;
            }
            j++;
        }
        i++;
    }
    return;
}

void creare_matrice_grafica() {//dichiarazione variabili locali
    FILE *fpt;//Puntatore al file "campo.txt"
    char c, d;/* "c" contiene un carattere letto dal file,
  "d" conserva il penultimo carattere letto dal file */
    int ri, co;//righe,colonne della matrice_pedine

    if ((fpt = fopen("campo.txt", "r")) == NULL)//Se il file non esiste, stampo un messaggio di errore
        puts("File campo da gioco non trovato");
    else {
        //leggo il campo da gioco dal file di testo
        c = getc(fpt);//acquisisco carrattere dal file
        d = c;
        ri = 0;
        co = 0;
        while (c != EOF)//Leggo caratteri fino alla fine del file{
            if (d == '\n') {
                co = 0;
                ri++;
            }
        matrice_grafica[ri * colonneg + co] = c;//Associo alla "matrice_grafica" il carattere letto dal file
        co++;
        d = c; //salvo il penultimo carattere
        c = getc(fpt); //leggo carattere successivo
    }
    fclose(fpt);//chiudo il file
}

return;
}

void effettuare_mossa(partita *p) {
    //dichiarazione variabili locali
    pedina *daptr;
    pedina *versoptr;
    pedina da;//indica le coordinate di partenza della pedina
    pedina verso;//indica le coordinate di arrivo della pedina
    daptr = &da;//associo il puntatore all'indirizzo di memoria della variabile
    versoptr = &verso;//associo il puntatore all'indirizzo di memoria della variabile

    check *cordpt;
    check checkspia;
    cordpt = &checkspia;//associo il puntatore all'indirizzo di memoria della variabile

    rec *recptr;
    rec reclocale = {0, 0, 0, 0, ""};//inizializzo la struttura
    recptr = &reclocale;//associo il puntatore all'indirizzo di memoria della variabile

    FILE *ftxt;
    time_t start;

    boolean errore;//
    boolean vittoria;//indica se una partita � stata conclusa o meno
    boolean vicino;//riceve il valore VERO se la modalit� di cattura � per avvicinamento
    boolean lontano;//riceve il valore VERO se la modalit� di  cattura � per allontanamento

    int cgy = 11;//indica la colonna grafica a video
    int cgx = 21;//indica la riga grafica a video
    int esito;
    int scelta;
    int ESC = 0;
    //fine dichiarazione variabili locali

    time(&start);//inizializzo il tempo
    do {
        contatore_tempo(recptr, start, p);
        cancella_riga(cgx);
        cancella_riga(cgx + 1);
        cancella_riga(cgx + 2);
        GotoXY(1, cgx);
        if (p->turn % 2)//In base al valore di turn cambia il turno tra giocatore 1 e giocatore 2
            printf("%s %c muove la pedina:\n", p->nomeg2, p->pedinag2);//se � pari
        else printf("%s %c muove la pedina:\n", p->nomeg1, p->pedinag1);//se � dispari
        do {//Inserimento delle coordinate dal punto in cui devo spostare la pedina
            esito = 0;
            cancella_riga(cgx + 1);
            GotoXY(1, cgx + 1);
            printf("Da: riga ( ) colonna ( )");
            ESC = inserire_coordinate_spostamento_pedina(cgy, cgx + 1, daptr);
            if (ESC !=
                codESC)//se non l'utente non ha premuto il tasto ESC posso procedere con il controllo delle coordinate
                esito = controllare_coordinate_spostamento(daptr);
            if (esito != 0)//l'utente non ha inserito correttamente le coordinate
            {//stampo il messaggio di errore
                GotoXY(26, cgx + 1);
                printf("Coordinate errate!! Premere un tasto...");
                fflush(stdin);
                getch();
            }
            contatore_tempo(recptr, start, p);
        } while (esito != 0);//ripeto l'inserimento delle coordinate finch� non sono state inserite correttamente
        if (ESC != codESC)//se l'utente non ha premuto il tasto ESC
        {
            do {//Inserimento delle coordinate verso il punto in cui devo spostare la pedina
                esito = 0;
                cancella_riga(cgx + 2);
                GotoXY(1, cgx + 2);
                printf("A:  riga ( ) colonna ( )");
                ESC = inserire_coordinate_spostamento_pedina(cgy, cgx + 2, versoptr);
                if (ESC !=
                    codESC)//se non l'utente non ha premuto il tasto ESC posso procedere con il controllo delle coordinate
                    esito = controllare_coordinate_spostamento(versoptr);
                if (esito != 0)//l'utente non ha inserito correttamente le coordinate
                {//stampo il messaggio di errore
                    GotoXY(25, cgx + 2);
                    printf("Coordinate errate! Premere un tasto...");
                    fflush(stdin);
                    getch();
                }
                contatore_tempo(recptr, start, p);
            } while (esito != 0);//ripeto l'inserimento delle coordinate finch� non sono state inserite correttamente
            if (ESC != codESC)//se l'utente non ha premuto il tasto ESC
            {
                if (p->turn % 2)//In base al valore di turn scelgo quale giocatore sta effettuando lo spostamento
                {//giocatore 2 sta effettuando la mossa
                    if (p->matrice_pedine[daptr->cordx * colonne + daptr->cordy] ==
                        p->pedinag2)//se il giocatore sta muovendo la sua pedina verso uno spazio vuoto sul campo
                        errore = controllare_spostamento_pedina(p, daptr, versoptr, cordpt);
                    else errore = VERO;//il giocatore non ha selezionato la sua pedina oppure ha selezionato uno spazio vuoto sul campo
                } else {//giocatore 1 sta effettuando la mossa
                    if (p->matrice_pedine[daptr->cordx * colonne + daptr->cordy] ==
                        p->pedinag1)//se il giocatore sta muovendo la sua pedina verso uno spazio vuoto sul campo
                        errore = controllare_spostamento_pedina(p, daptr, versoptr, cordpt);
                    else errore = VERO;//il giocatore non ha selezionato la sua pedina oppure ha selezionato uno spazio vuoto sul campo
                }
                if (errore == FALSO)//il giocatore ha mosso correttamente la sua pedina sul campo
                {
                    if (p->turn % 2) {//giocatore 2 sta effettuando la mossa
                        controllare_modalita_cattura(p, cordpt, daptr, versoptr, p->pedinag1, &vicino, &lontano);
                        if ((vicino == VERO) &&
                            (lontano == VERO)) {//l'utente deve scegliere quale modalit� di cattura eseguire
                            scelta = scegliere_modalita_cattura();
                            if (scelta == 1) //eseguire la cattura per avvicinamento
                                mangiare_pedina_avvicinamento(p, cordpt, daptr, versoptr, p->pedinag1);
                            else if (scelta == 2)//eseguire la cattura per allontanamento
                                mangiare_pedina_allontanamento(p, cordpt, daptr, versoptr, p->pedinag1);
                        } else if (vicino == VERO)//eseguire la cattura solo per avvicinamento
                            mangiare_pedina_avvicinamento(p, cordpt, daptr, versoptr, p->pedinag1);
                        else if (lontano == VERO)//eseguire la cattura solo per allontanamento
                            mangiare_pedina_allontanamento(p, cordpt, daptr, versoptr, p->pedinag1);
                        vittoria = controllare_pedine_campo(p,
                                                            p->pedinag1);//controllo se giocatore 2 ha mangiato tutte le pedine avversarie
                    } else {//giocatore 1 sta effettuando la mossa
                        controllare_modalita_cattura(p, cordpt, daptr, versoptr, p->pedinag2, &vicino, &lontano);
                        if ((vicino == VERO) &&
                            (lontano == VERO)) {//l'utente deve scegliere quale modalit� di cattura eseguire
                            scelta = scegliere_modalita_cattura();
                            if (scelta == 1)//eseguire la cattura per avvicinamento
                                mangiare_pedina_avvicinamento(p, cordpt, daptr, versoptr, p->pedinag2);
                            else if (scelta == 2)//eseguire la cattura per allontanamento
                                mangiare_pedina_allontanamento(p, cordpt, daptr, versoptr, p->pedinag2);
                        } else if (vicino == VERO)//eseguire la cattura per avvicinamento
                            mangiare_pedina_avvicinamento(p, cordpt, daptr, versoptr, p->pedinag2);
                        else if (lontano == VERO)//eseguire la cattura per allontanamento
                            mangiare_pedina_allontanamento(p, cordpt, daptr, versoptr, p->pedinag2);
                        vittoria = controllare_pedine_campo(p,
                                                            p->pedinag2);//controllo se giocatore 1 ha mangiato tutte le pedine avversarie
                    }
                    p->turn++;//scambio giocatore
                } else {//stampo messaggio di errore
                    cancella_riga(cgx);
                    cancella_riga(cgx + 1);
                    cancella_riga(cgx + 2);
                    GotoXY(1, cgx);
                    printf("Impossibile effettuare mossa");
                    GotoXY(1, cgx + 1);
                    system("pause");
                }
            }
        }
        contatore_tempo(recptr, start, p);
    } while ((ESC != codESC) && (vittoria !=
                                 VERO));//ripeto finch� non � stato premuto il tasto ESC oppure nessuno dei due giocatori ha vinto
    p->recordp = reclocale;//associo il tempo Record realizzato alla partita in corso
    if (vittoria == VERO) {
        cancella_riga(cgx);
        cancella_riga(cgx + 1);
        cancella_riga(cgx + 2);
        GotoXY(0, cgx);
        if ((p->turn - 1) %
            2)//In base al valore di turn scelgo quale giocatore ha vinto la partita
            printf("Complimenti %s ha vinto la partita\n", p->nomeg2);//giocatore 2 ha vinto la partita
        else printf("Complimenti %s ha vinto la partita\n", p->nomeg1);//giocatore 1 ha vinto la partita
        GotoXY(1, cgx + 1);
        system("pause");
        //controllo del tempo Record
        if ((ftxt = fopen("topten.txt", "r")) == NULL) {//se il file non esiste lo creo
            ftxt = fopen("topten.txt", "w");
            fclose(ftxt);
        }
        esito = controllare_nuovo_record(p);//controllo se c'� stato un nuovo tempo Record
        if (esito == VERO) {  //e' stato conseguito un nuovo tempo Record
            cancella_riga(cgx + 1);
            GotoXY(0, cgx + 1);
            printf("Nuovo Record conseguito!\n");
            printf("Inserire nome Record: ");
            scanf("%s", p->recordp.nomerecord);
            ordinamento_file_topten(p); //ordino il file in modo crescente
        }
    }
    return;
}

void contatore_tempo(rec *reclc, time_t s, partita *p) {//dichiarazione di variabili locali
    time_t end;
    int maxtemp = 60;

    time(&end);//Ferma il contatore tempo
    reclc->diff_t = difftime(end, s);//Calcola la differenza da start a end
    reclc->minuti = reclc->diff_t / maxtemp;//Calcola i minuti
    reclc->secondi = reclc->diff_t - (reclc->minuti * maxtemp);//Calcola i secondi

    reclc->minuti = reclc->minuti + p->recordp.minuti;
    reclc->secondi = reclc->secondi + p->recordp.secondi;
    while (reclc->secondi > maxtemp) {
        reclc->minuti++;
        reclc->secondi = reclc->secondi - maxtemp;
    }
    while (reclc->minuti > maxtemp) {
        reclc->ora++;
        reclc->minuti = reclc->minuti - maxtemp;
    }
    reclc->ora = reclc->ora + p->recordp.ora;
    reclc->diff_t = reclc->diff_t + p->recordp.diff_t;
    cancella_riga(0);
    GotoXY(66, 0);
    printf("TIME  %02d:%02d:%02d", reclc->ora, reclc->minuti, reclc->secondi);
    return;
}

int inserire_coordinate_spostamento_pedina(int xx, int yy,
                                           pedina *p) {//xx,yy coordinate grafiche per l'inserimento a video delle coordinate
    int exit;//Restituisce l'ultimo valore letto da tastiera
    GotoXY(xx, yy);
    fflush(stdin);
    p->cordx = getch();
    exit = p->cordx;
    if (exit != codESC)//controllo se � stato premuto il tasto ESC
    {//non e' stato premuto il tasto ESC, l'utente puo' completare l'inserimento delle coordinate
        printf("%c", p->cordx);
        GotoXY(xx + 12, yy);
        fflush(stdin);
        p->cordy = getch();
        exit = p->cordy;
        if (exit != codESC)
            printf("%c", p->cordy);
    }
    return exit;
}

int controllare_coordinate_spostamento(pedina *p)//riga(lettera),colonna(numero)
{//definizione variabili locali
    int lettera, numero;//"lettera" assume caratteri ASCII delle lettere da A,a e E,e

    int ctrl;//Viene decrementato a 0 se le coordinate sono state inserite correttamente
    ctrl = 2;
    lettera = islower(p->cordx);//controlla se la lettera � minuscola o maiuscola
    if (lettera != 0) {//La lettere e' minuscola
        if ((p->cordx >= 97) && (p->cordx <= 101))//codici ASCII dei caratteri a,b,c,d,e
        {
            ctrl--;
            p->cordx = p->cordx - 97;//Converto la lettera in numero da 0 a 4
        }
    } else //La lettera e' maiuscola
    {
        if ((p->cordx >= 65) && (p->cordx <= 69))//codici ASCII dei caratteri A,B,C,D,E
        {
            ctrl--;
            p->cordx = p->cordx - 65;//Ricavo il numero da 0 a 4
        }
    }
    numero = isdigit(p->cordy);//Controllare se la coordinata e' un numero
    if (numero != 0) {//Il carattere digitato e' un numero
        if ((p->cordy >= 49) && (p->cordy <= 57))//codici ASCII delle cifre 1,2,3,4,5,6,7,8,9
        {
            ctrl--;
            p->cordy = p->cordy - 49;
        }
    }
    return ctrl;
}

void cambiare_simbolo_pedine(config *imp)//
{
    stampare_interfaccia_cambiare_simbolo_pedine();
    scegliere_coppia_pedine(imp);
    return;
}

void scegliere_coppia_pedine(config *imp)//
{//dichiarazioni variabili locali
    int scelta;//Puo' assumere valori da 0 a 3
    int cgx = 13;//indica la riga grafica a video

    do {
        cancella_riga(cgx);
        GotoXY(2, cgx);
        printf("%c Inserire scelta:  ", 16);
        scanf("%d", &scelta);
        if ((scelta < 0) || (scelta > 3)) {
            printf("\nERRORE! Inserire un numero compreso tra 0 e 3\n");
            system("pause");
            cancella_riga(cgx + 1);
            cancella_riga(cgx + 2);
        }
            //Associa ad ogni scelta dell'utente una coppia di caratteri ASCII che identificano graficamente le pedine
        else if (scelta == 1) {
            imp->pg1 = 2;
            imp->pg2 = 1;
        } else if (scelta == 2) {
            imp->pg1 = 15;
            imp->pg2 = 14;
        } else if (scelta == 3) {
            imp->pg1 = 36;
            imp->pg2 = 156;
        }
    } while ((scelta < 0) || (scelta > 3));
    return;
}

void cambiare_nome_giocatori(config *imp) {
    int cgx = 5;//indica la riga grafica a video

    stampare_interfaccia_cambiare_nomi_giocatori();
    GotoXY(15, cgx);
    scanf("%s", imp->ng1);//Inserisci nome giocatore 1
    GotoXY(15, cgx + 1);
    scanf("%s", imp->ng2);//Inserisci nome giocatore 2
    return;
}

boolean controllare_spostamento_pedina(partita *game, pedina *dap, pedina *versop,
                                       check *verificaptr) {//dichiarazione variabili locali
    int cgx = 21;//indica la riga grafica a video
    int esitoR = 0, esitoC = 0;//Indicano se ci stiamo spostando correttamente di una posizione sulle righe o/e sulle colonne
    boolean move = FALSO;//Indica se e' possibile effettuare lo spostamento in diagonale


    boolean stop = FALSO;//la variabile assumer� i valori "VERO" e "FALSO" per indicare se il giocatore ha inserito delle coordinate giuste per effettuare una mossa


    if ((dap->cordy == versop->cordy) && (dap->cordx == versop->cordx))
        stop = VERO; //L'utente sta cercando di spostarsi sulla stessa posizione
    else {
        esitoR = controllare_riga(dap, versop, verificaptr);
        if (esitoR != 0)//Ci stiamo spostando correttamente di una posizione sulle righe
        {
            esitoC = controllare_colonna(dap, versop, verificaptr);
            if (esitoC != 0)//Ci stiamo spostando correttamente di una posizione sulle coolonne
            {
                if ((esitoR != 2) && (esitoC != 2))//Ci stiamo spostando lungo una diagonale
                {
                    move = controllare_linea_campo(dap, verificaptr);
                    if (move == VERO)//E' possibile spostarsi in diagonale
                        stop = effettuare_spostamento(game, dap, versop);
                    else stop = VERO;//errore, manca la linea del campo
                } else stop = effettuare_spostamento(game, dap, versop);
            } else stop = VERO;//errore, mi sto muovendo di due o pi� colonne
        } else stop = VERO;//errore, mi sto muovendo di due o pi� righe
    }
    return stop;
}

int controllare_colonna(pedina *dap, pedina *versop, check *ck) {
    int ctrl = 0;//Puo' assumere valori tra 0 e 2

    ck->sx = FALSO;
    ck->dx = FALSO;
    //controllo che lo sostamento avvenga di una colonna a destra o sinistra o rimane sulla stessa
    if (dap->cordy > versop->cordy) {
        if (dap->cordy == versop->cordy + 1)//Indica che mi sto muovendo correttamente di una posizione verso sinistra
        {
            ck->sx = VERO;
            ctrl = 1;
        }
    } else if (versop->cordy > dap->cordy) {
        if (versop->cordy == dap->cordy + 1)//Indica che mi sto muovendo correttamente di una posizione verso sinistra
        {
            ck->dx = VERO;
            ctrl = 1;
        }
    } else ctrl = 2;//il numero della colonna da e verso sono uguali
    return ctrl;
}

int controllare_riga(pedina *dap, pedina *versop, check *ck) {
    int ctrl = 0;
    ck->up = FALSO;
    ck->down = FALSO;
    //controllo che lo sostamento avvenga di una riga in basso o in alto o rimane sulla stessa
    if (dap->cordx > versop->cordx) {
        if (dap->cordx == versop->cordx + 1)//Indica che mi sto muovendo correttamente di una posizione verso l'alto
        {
            ck->up = VERO;
            ctrl = 1;
        }
    } else if (versop->cordx > dap->cordx) {
        if (versop->cordx == dap->cordx + 1)//Indica che mi sto muovendo correttamente di una posizione verso il basso
        {
            ck->down = VERO;
            ctrl = 1;
        }
    } else ctrl = 2;//il numero della riga da e verso sono uguali

    return ctrl;
}

boolean controllare_linea_campo(pedina *dap, check *ck) {//dichiarazione di variabili locali
    int xC = 5, xR = 4;//Moltiplicatori di colonna e riga per posizionarsi correttamente nella "matrice_grafica" linearizzata

    boolean muovi = FALSO;//Autorizza lo spostamento lungo una diagonale

    if ((ck->up == VERO) && (ck->dx == VERO))//Controlla lo spostamento in alto a destra
    {
        if (matrice_grafica[(dap->cordx * xR * colonneg + 2) + (dap->cordy * xC - colonneg)] == '/')
            muovi = VERO;
    } else if ((ck->up == VERO) && (ck->sx == VERO))//Controlla lo spostamento in alto a sinistra
    {
        if (matrice_grafica[(dap->cordx * xR * colonneg - 2) + (dap->cordy * xC - colonneg)] == '\\')
            muovi = VERO;
    } else if ((ck->down == VERO) && (ck->dx == VERO))//Controlla lo spostamento in basso a destra
    {
        if (matrice_grafica[(dap->cordx * xR * colonneg + 1) + (dap->cordy * xC + colonneg)] == '\\')
            muovi = VERO;
    } else if ((ck->down == VERO) && (ck->sx == VERO))//Controlla lo spostamento in basso a sinistra
    {
        if (matrice_grafica[(dap->cordx * xR * colonneg - 1) + (dap->cordy * xC + colonneg)] == '/')
            muovi = VERO;
    }
    return muovi;
}

boolean effettuare_spostamento(partita *g, pedina *dap, pedina *versop) {//dichiarazione di variabili locali
    int xC = 5, xR = 4;//Moltiplicatori di colonna e riga per spostare il cursore sul video
    pedina cordgraf = {5, 3};

    boolean exit = FALSO;//Indica se e' possibile effettuare o meno lo spostamento

    if (g->matrice_pedine[versop->cordx * colonne + versop->cordy] ==
        0)//Controlla se l'utente si sta spostando in una casella vuota del campo
    {
        if (g->turn % 2)
            g->matrice_pedine[versop->cordx * colonne + versop->cordy] = g->pedinag2;
        else g->matrice_pedine[versop->cordx * colonne + versop->cordy] = g->pedinag1;
        g->matrice_pedine[dap->cordx * colonne + dap->cordy] = 0;
        GotoXY((xC * versop->cordy) + cordgraf.cordy, (xR * versop->cordx) + cordgraf.cordx);
        printf("%c", g->matrice_pedine[versop->cordx * colonne + versop->cordy]);
        GotoXY(cordgraf.cordy + (xC * dap->cordy), cordgraf.cordx + (xR * dap->cordx));
        printf("%c", 79);
    } else exit = VERO;
    return exit;
}

int scegliere_slot() {//dichiarazione variabili locali
    int num;
    int cgx = 12;//indica la riga grafica a video
    boolean test = FALSO;//Indica se l'utente ha inserito uno slot corretto o meno

    int pos;//Indica lo slot inserito dall'utente
    do {
        cancella_riga(cgx);
        GotoXY(1, cgx);
        printf("%c Inserire numero slot:", 16);
        GotoXY(25, cgx);
        fflush(stdin);
        pos = getch();
        printf("%c", pos);
        if (pos != codESC) {
            num = isdigit(pos);//Controlla se il carattere inserito e' un numero
            if (num != 0) {//Il carattere inserito e' un numero
                if ((pos >= 48) && (pos <= 52))//Controllo se il carattere ASCII corrisponde ai numeri 0,1,2,3,4
                {
                    pos = pos - 48;//Mi ricavo il numero da 0 a 4
                    test = VERO;
                } else//Il carattere non corrisponde ai numeri 0,1,2,3,4
                {//Stampo messaggio di errore
                    cancella_riga(cgx);
                    GotoXY(1, cgx);
                    printf("Errore! Inserire un numero compreso tra 0 e 4\n");
                    GotoXY(1, cgx + 1);
                    system("pause");
                    cancella_riga(cgx);
                    cancella_riga(cgx + 1);
                }
            } else//Il carattere inserito non e' un numero
            {//Stampo messaggio di errore
                cancella_riga(cgx);
                GotoXY(1, cgx);
                printf("Errore! Carattere non corretto: non hai inserito un numero\n");
                GotoXY(1, cgx + 1);
                system("pause");
                cancella_riga(cgx);
                cancella_riga(cgx + 1);
            }
        } else test = VERO;//L'utente ha premuto il tasto ESC
    } while (test != VERO);//Ripeto finche' l'utente non inserisce uno slot corretto o non ha premuto il tasto ESC
    return pos;
}

void riprendere_partita(partita *p) {
    creare_matrice_grafica();
    stampare_interfaccia_campo(p);
    effettuare_mossa(p);
    return;
}

void mangiare_pedina_avvicinamento(partita *p, check *cordpt, pedina *dap, pedina *versop,
                                   int pedg) {//dichiarazione variabili locali
    int i, j;//Contatori
    int xC = 5, xR = 4;//Moltiplicatori di colonna e riga per spostare il cursore sul video
    boolean stop;//Assume il valore "VERO" quando incontra una pedina non avversaria
    pedina ctrl;//Struttura dati di tipo "pedina" i cui campi assumomo diversi valori in base alla direzione di spostamento della pedina
    pedina cordgraf = {5, 3};//Indica lo spostamento del campo

    if ((cordpt->up == VERO) && (cordpt->sx == VERO))//Controlla se ci stiamo spostando in alto a sinistra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = versop->cordx - j;
        ctrl.cordy = versop->cordy - j;
        while ((ctrl.cordx >= 0) && (ctrl.cordy >= 0) && (stop != VERO)) {
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {//Catturo la pedina dell'avversario
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx--;
            ctrl.cordy--;
        }
    } else if ((cordpt->up == VERO) && (cordpt->dx == VERO))//Controlla se ci stiamo spostando in alto a destra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = versop->cordx - j;
        ctrl.cordy = versop->cordy + j;
        while ((ctrl.cordx >= 0) && (ctrl.cordy <= (colonne - 1)) &&
               (stop != VERO)) {//Catturo la pedina dell'avversario
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx--;
            ctrl.cordy++;
        }
    } else if ((cordpt->down == VERO) && (cordpt->sx == VERO))//Controlla se ci stiamo spostando in bassp a sinistra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = versop->cordx + j;
        ctrl.cordy = versop->cordy - j;
        while ((ctrl.cordx <= (righe - 1)) && (ctrl.cordy >= 0) && (stop != VERO)) {
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {//Catturo la pedina dell'avversario
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx++;
            ctrl.cordy--;
        }
    } else if ((cordpt->down == VERO) && (cordpt->dx == VERO))//Controlla se ci stiamo spostando in basso a destra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = versop->cordx + j;
        ctrl.cordy = versop->cordy + j;
        while ((ctrl.cordx <= (righe - 1)) && (ctrl.cordy <= (colonne - 1)) && (stop != VERO)) {
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {//Catturo la pedina dell'avversario
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � della dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx++;
            ctrl.cordy++;
        }
    } else if ((cordpt->up == FALSO) && (cordpt->down == FALSO)) {
        if (cordpt->sx == VERO)//Ci stiamo spostando solo a sinistra
        {
            j = 1;
            i = 0;
            stop = FALSO;
            while ((i < versop->cordy) && (stop != VERO)) {
                if (p->matrice_pedine[(versop->cordx * colonne + versop->cordy) - j] ==
                    pedg) {//Catturo la pedina dell'avversario
                    p->matrice_pedine[(versop->cordx * colonne + versop->cordy) - j] = 0;
                    GotoXY((xC * versop->cordy - (j * xC)) + cordgraf.cordy, (xR * versop->cordx) + cordgraf.cordx);
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                j++;
                i++;
            }
        }
        if (cordpt->dx == VERO)//Ci stiamo spostando solo a destra
        {
            i = versop->cordy;
            j = 1;
            stop = FALSO;
            while ((i < (colonne - 1)) && (stop != VERO)) {
                if (p->matrice_pedine[(versop->cordx * colonne + versop->cordy) + j] ==
                    pedg) {//Catturo la pedina dell'avversario
                    p->matrice_pedine[(versop->cordx * colonne + versop->cordy) + j] = 0;
                    GotoXY((xC * versop->cordy + (j * xC)) + cordgraf.cordy, (xR * versop->cordx) + cordgraf.cordx);
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                //Mi sposto alla posizione successiva
                i++;
                j++;
            }
        }
    } else if ((cordpt->sx == FALSO) && (cordpt->dx == FALSO)) {
        if (cordpt->up == VERO)//Ci stiamo spostando solo in alto
        {
            i = 0;
            j = 1;
            stop = FALSO;
            while ((i < versop->cordx) && (stop != VERO)) {
                if (p->matrice_pedine[(versop->cordx * colonne + versop->cordy) - (j * colonne)] ==
                    pedg) {//Catturo la pedina dell'avversario
                    p->matrice_pedine[(versop->cordx * colonne + versop->cordy) - (j * colonne)] = 0;
                    GotoXY(((xC * versop->cordy) + cordgraf.cordy), cordgraf.cordx + (versop->cordx * xR) - (j * xR));
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                //Mi sposto alla posizione successiva
                i++;
                j++;
            }
        }
        if (cordpt->down == VERO)//Ci stiamo spostando solo in basso
        {
            i = versop->cordx;
            j = 1;
            stop = FALSO;
            while ((i < righe - 1) && (stop != VERO)) {
                if (p->matrice_pedine[(versop->cordx * colonne + versop->cordy) + (j * colonne)] ==
                    pedg) {//Catturo la pedina dell'avversario
                    p->matrice_pedine[(versop->cordx * colonne + versop->cordy) + (j * colonne)] = 0;
                    GotoXY(((xC * versop->cordy) + cordgraf.cordy), cordgraf.cordx + (versop->cordx * xR) + (j * xR));
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                //Mi sposto alla posizione successiva
                i++;
                j++;
            }
        }
    }
    return;
}

void mangiare_pedina_allontanamento(partita *p, check *cordpt, pedina *dap, pedina *versop,
                                    int pedg) {//dichiarazione variabili locali
    int xC = 5, xR = 4;//Moltiplicatori di colonna e riga per spostare il cursore sul video
    int i, j;//Contatori
    boolean stop;//Assume il valore "VERO" quando incontra una pedina non avversaria
    pedina ctrl;//Struttura dati di tipo "pedina" i cui campi assumomo diversi valori in base alla direzione di spostamento della pedina
    pedina cordgraf = {5, 3};

    if ((cordpt->up == VERO) && (cordpt->sx == VERO))//Ci stiamo spostando in alto a sinistra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = dap->cordx + j;
        ctrl.cordy = dap->cordy + j;
        while ((ctrl.cordx <= (righe - 1)) && (ctrl.cordy <= (colonne - 1)) && (stop != VERO)) {
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx++;
            ctrl.cordy++;
        }
    } else if ((cordpt->up == VERO) && (cordpt->dx == VERO))//Ci spostiamo in alto a destra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = dap->cordx + j;
        ctrl.cordy = dap->cordy - j;
        while ((ctrl.cordx <= (righe - 1)) && (ctrl.cordy >= 0) && (stop != VERO)) {
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx++;
            ctrl.cordy--;
        }
    } else if ((cordpt->down == VERO) && (cordpt->sx == VERO))//Ci spostiamo giu' a sinistra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = dap->cordx - j;
        ctrl.cordy = dap->cordy + j;
        while ((ctrl.cordx >= 0) && (ctrl.cordy <= (colonne - 1)) && (stop != VERO)) {
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx--;
            ctrl.cordy++;
        }
    } else if ((cordpt->down == VERO) && (cordpt->dx == VERO))//Ci stiamo spostando in basso a destra
    {
        stop = FALSO;
        j = 1;
        ctrl.cordx = dap->cordx - j;
        ctrl.cordy = dap->cordy - j;
        while ((ctrl.cordx >= 0) && (ctrl.cordy >= 0) && (stop != VERO)) {
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg) {
                p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] = 0;
                GotoXY(xC * ctrl.cordy + cordgraf.cordy, xR * ctrl.cordx + cordgraf.cordx);
                printf("%c", 79);
            } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
            //Mi sposto alla posizione successiva
            ctrl.cordx--;
            ctrl.cordy--;
        }
    } else if ((cordpt->up == FALSO) && (cordpt->down == FALSO)) {
        if (cordpt->sx == VERO)//Ci stiamo spostando solo a sinistra
        {
            i = dap->cordy;
            j = 1;
            stop = FALSO;
            while ((i < (colonne - 1)) && (stop != VERO)) {
                if (p->matrice_pedine[(dap->cordx * colonne + dap->cordy) + j] == pedg) {
                    p->matrice_pedine[(dap->cordx * colonne + dap->cordy) + j] = 0;
                    GotoXY((xC * dap->cordy + (j * xC)) + cordgraf.cordy, (xR * dap->cordx) + cordgraf.cordx);
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                //Mi sposto alla posizione successiva
                i++;
                j++;
            }
        }
        if (cordpt->dx == VERO)//Ci stiamo spostando solo verso destra
        {
            j = 1;
            i = 0;
            stop = FALSO;
            while ((i < dap->cordy) && (stop != VERO)) {
                if (p->matrice_pedine[(dap->cordx * colonne + dap->cordy) - j] == pedg) {
                    p->matrice_pedine[(dap->cordx * colonne + dap->cordy) - j] = 0;
                    GotoXY((xC * dap->cordy - (j * xC)) + cordgraf.cordy, (xR * dap->cordx) + cordgraf.cordx);
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                //Mi sposto alla posizione successiva
                j++;
                i++;
            }
        }
    } else if ((cordpt->sx == FALSO) && (cordpt->dx == FALSO)) {
        if (cordpt->up == VERO)//Ci stiamo spostando solo vero l'alto
        {
            i = dap->cordx;
            j = 1;
            stop = FALSO;
            while ((i < righe - 1) && (stop != VERO)) {
                if (p->matrice_pedine[(dap->cordx * colonne + dap->cordy) + (j * colonne)] == pedg) {
                    p->matrice_pedine[(dap->cordx * colonne + dap->cordy) + (j * colonne)] = 0;
                    GotoXY(((xC * dap->cordy) + cordgraf.cordy), cordgraf.cordx + (dap->cordx * xR) + (j * xR));
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                //Mi sposto alla posizione successiva
                i++;
                j++;
            }
        }
        if (cordpt->down == VERO)//Ci stiamo spostando solo verso il basso
        {
            i = 0;
            j = 1;
            stop = FALSO;
            while ((i < dap->cordx) && (stop != VERO)) {
                if (p->matrice_pedine[(dap->cordx * colonne + dap->cordy) - (j * colonne)] == pedg) {
                    p->matrice_pedine[(dap->cordx * colonne + dap->cordy) - (j * colonne)] = 0;
                    GotoXY(((xC * dap->cordy) + cordgraf.cordy), cordgraf.cordx + (dap->cordx * xR) - (j * xR));
                    printf("%c", 79);
                } else stop = VERO;//se ho incontrato una pedina che non � dell'avversario
                //Mi sposto alla posizione successiva
                i++;
                j++;
            }
        }
    }
    return;
}

void controllare_modalita_cattura(partita *p, check *cordpt, pedina *dap, pedina *versop, int pedg, boolean *vicinopt,
                                  boolean *lontanopt) {//dichiarazione variabili locali
    int j = 1;
    pedina ctrl;
    //Quando entrambi assumono il valore "VERO" possono essere effettuati entrambi i tipi di cattura
    *lontanopt = FALSO;//Cambia il suo valore a "VERO" se la modalit� di cattura e' per allontanamento
    *vicinopt = FALSO;//Cambia il suo valore a "VERO" se la modalit� di cattura e' per avvicinamento

    if ((cordpt->up == VERO) && (cordpt->sx == VERO))//Se ci muoviamo verso l'alto a sinistra
    {//Controllo le pedine successive in basso a destra partendo dalla coordinata "dap"
        ctrl.cordx = dap->cordx + j;
        ctrl.cordy = dap->cordy + j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)//se la pedina trovata e' una pedina avversaria
            *lontanopt = VERO; //Posso mangiare per allontanamento
        //Controllo pedine successive in alto a sinistra partendo dalla coordinata "versop"
        ctrl.cordx = versop->cordx - j;
        ctrl.cordy = versop->cordy - j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)//se la pedina trovata e' una pedina avversaria
            *vicinopt = VERO; //Posso mangiare per avvicinamento
    } else if ((cordpt->up == VERO) && (cordpt->dx == VERO))//Se ci muoviamo in alto a destra
    {//Controllo le pedine successive in basso a sinistra partendo dalla coordinata "dap"
        ctrl.cordx = dap->cordx + j;
        ctrl.cordy = dap->cordy - j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)//se la pedina trovata e' una pedina avversaria
            *lontanopt = VERO; //Posso mangiare per allontanamento
        //Controllo pedine successive in alto a destra partendo dalla coordinata "versop"
        ctrl.cordx = versop->cordx - j;
        ctrl.cordy = versop->cordy + j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
            *vicinopt = VERO; //Posso mangiare per avvicinamento
    } else if ((cordpt->down == VERO) && (cordpt->sx == VERO))//Se ci muoviamo in basso a sinistra
    {//Controllo le pedine successive in alto a destra a partire dalla coordinata "dap"
        ctrl.cordx = dap->cordx - j;
        ctrl.cordy = dap->cordy + j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
            *lontanopt = VERO; //Posso mangiare per allontanamento
        //Controllo pedine successive in basso a sinistra partendo dalla coordinata "versop"
        ctrl.cordx = versop->cordx + j;
        ctrl.cordy = versop->cordy - j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
            *vicinopt = VERO; //Posso mangiare per avvicinamento
    } else if ((cordpt->down == VERO) && (cordpt->dx == VERO))//Se ci muoviamo in basso a destra
    {//Controllo pedine successive in alto a sinistra partendo dalla coordinata "dap"
        ctrl.cordx = dap->cordx - j;
        ctrl.cordy = dap->cordy - j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
            *lontanopt = VERO; //Posso mangiare per allontanamento
        //Controllo pedine successive in basso a destra partendo dalla coordinata "versop"
        ctrl.cordx = versop->cordx + j;
        ctrl.cordy = versop->cordy + j;
        if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
            *vicinopt = VERO;//Posso mangiare per avvicinamento
    } else if ((cordpt->up == FALSO) && (cordpt->down == FALSO)) {
        if (cordpt->sx == VERO)//Mi sto muovendo solo a sinistra
        {//Controllo le pedine successive verso sinistra a partire dalla coordinata "versop"
            ctrl.cordx = versop->cordx;
            ctrl.cordy = versop->cordy - j;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *vicinopt = VERO; //Posso mangiare per avvicinamento
            //Controllo le pedine successive verso destra a partire dalla coordinata "dap"
            ctrl.cordx = dap->cordx;
            ctrl.cordy = dap->cordy + j;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *lontanopt = VERO;//Posso mangiare per allontanamento
        }
        if (cordpt->dx == VERO)//Mi sto muovendo solo a destra
        {//Controllo le pedine successive verso sinistra a partire dalla coordinata "dap"
            ctrl.cordx = dap->cordx;
            ctrl.cordy = dap->cordy - j;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *lontanopt = VERO; //Posso mangiare per allontanamento
            //Controllo le pedine successive verso destra a partire dalla coordinata  "versop"
            ctrl.cordx = versop->cordx;
            ctrl.cordy = versop->cordy + j;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *vicinopt = VERO;//Posso mangiare per avvicinamento
        }
    } else if ((cordpt->sx == FALSO) && (cordpt->dx == FALSO)) {
        if (cordpt->up == VERO)//Mi sto muovendo solo verso l'alto
        {//Controllo le pedine successive verso il basso a partire dalla coordinata "versop"
            ctrl.cordx = dap->cordx + j;
            ctrl.cordy = dap->cordy;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *lontanopt = VERO; //Posso mangiare per allontanamento
            //Controllo le pedine successive verso l'alto a partire dalla coordinata "dap"
            ctrl.cordx = versop->cordx - j;
            ctrl.cordy = versop->cordy;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *vicinopt = VERO;//Posso mangiare per avvicinamento
        }
        if (cordpt->down == VERO) {//Controllo le pedine successive verso l'alto a partire dalla coordinata "versop"
            ctrl.cordx = dap->cordx - j;
            ctrl.cordy = dap->cordy;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *lontanopt = VERO; //Posso mangiare per allontanamento
            //Controllo le pedine successive verso il basso a partire dalla coordinata "dap"
            ctrl.cordx = versop->cordx + j;
            ctrl.cordy = versop->cordy;
            if (p->matrice_pedine[ctrl.cordx * colonne + ctrl.cordy] == pedg)
                *vicinopt = VERO;//Posso mangiare per avvicinamento
        }
    }
    return;
}

int scegliere_modalita_cattura() {//dichiarazione variabili locali
    int cgx = 21;//indica la riga grafica a video
    int quale;//Puo' assumere valori 1 e 2
    do {
        cancella_riga(cgx);
        cancella_riga(cgx + 1);
        cancella_riga(cgx + 2);
        GotoXY(1, cgx);

        printf("Scegliere modalita' di cattura:\n");
        printf(" 1.Avvicinamento\n");
        printf(" 2.Allontanamento\n");
        GotoXY(33, cgx);
        scanf("%d", &quale);

        if ((quale < 1) || (quale > 2)) {
            cancella_riga(cgx);
            cancella_riga(cgx + 1);
            cancella_riga(cgx + 2);
            GotoXY(0, cgx);
            printf("Errore! Numeri accettati: 1 e 2\n");
            system("pause");
            cancella_riga(cgx);
            cancella_riga(cgx + 1);

        }
    } while ((quale < 1) || (quale > 2));


    return quale;
}

boolean controllare_pedine_campo(partita *p, int pedg) {//dichiarazione variabili locali
    int r = 0, c;

    boolean win = VERO;//Quando assume valore "VERO" uno dei due giocatori ha vinto la partita

    while ((r < righe) && (win != FALSO)) {
        c = 0;
        while ((c < colonne) && (win != FALSO)) {
            if (p->matrice_pedine[r * colonne + c] == pedg)//Se e' presente una pedina del giocatore avversario
                win = FALSO;//"win"="FALSO" quindi non ho vinto
            c++;
        }
        r++;
    }
    return win;
}

boolean controllare_nuovo_record(partita *p) {//dichiarazione variabili locali
    int i = 0, dim = 0;//"i" contatore, "dim" indica quanti record sono presenti su file
    char c;//Riceve un carattere letto dal file
    rec tt;//Riceve un tempo "Record" letto dal file
    boolean stop = FALSO;//Cambia il suo valore in vero se c'� stato un nuovo tempo record
    FILE *txt;//Puntatore a file

    txt = fopen("topten.txt", "r");
    if ((c = getc(txt)) == EOF) // legge un carattere dal file fino alla fine del file
        stop = VERO;
    else {
        while (fscanf(txt, "%d %d %d %d %s\n", &tt.ora, &tt.minuti, &tt.secondi, &tt.diff_t, tt.nomerecord) > 0)
            //e' stato trovato un tempo Record su file
            dim++;
        if (dim < (maxtt -
                   1))//Se il numero dei tempi record su file e' minore del numero massimo di tempi record conseguibili
            stop = VERO;//e' stato conseguito un nuovo tempo record
        else {
            while (fscanf(txt, "%d %d %d %d %s\n", &tt.ora, &tt.minuti, &tt.secondi, &tt.diff_t, tt.nomerecord) > 0) {
                if (p->recordp.diff_t <
                    tt.diff_t)//Confronto i tempi record presenti su file con quello della partita in corso
                    stop = VERO;//Il tempo della partita e' minore di uno dei tempi record presenti sul file: � stato conseguito un nuovo tempo record
            }
        }
    }
    fclose(txt);
    return stop;
}
