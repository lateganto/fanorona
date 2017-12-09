#include <stdlib.h>

#define righe 5 // Indica le righe della "matrice_pedine" linearizzata
#define colonne 9 // Indica le colonne della "matrice_pedine" linearizzata
#define righeg 16 //Indica le righe della "matrice_grafica" linearizzata
#define colonneg 40 // Indica le colonne della "matrice_grafica" linearizzata

//La matrice grafica linearizzata serve per i controlli sullo spostamento della pedina sul campo da gioco
#define maxtt 11 //Dimensione massima del vettore di record "vet_tt" 
#define dimax 5//Dimensione massima di schede di tipo partita salvabili sul file "salvataggio.dat"
#define nmax 20//Dimensione massima nome giocatore,nome partita e nome record 
#define codESC 27// Codice ASCII del tasto ESC

//Crea un nuovo tipo di variabile che pu� assumeri i valori "FALSO" e "VERO"
typedef enum {
    FALSO, VERO
} boolean;

//Crea un nuovo tipo di variabile per indicare una sequenza di "nmax" caratteri
typedef char stringa[nmax];

//Crea una nuova struttura dati utilizzata per il salvataggio dei tempi Record
typedef struct t_record {
    int ora;
    int minuti;
    int secondi;
    int diff_t;// Indica il totale dei secondi trascorsi dall'inizio della partita
    stringa nomerecord;//Indica il nome del record
} rec;

//Crea una struttura dati contenente tutte le informazioni relative ad una partita
typedef struct t_partita {
    boolean save;//Indica se la partita e' stata salvata o meno
    rec recordp;// Indica il tempo Record conseguito durante partita
    stringa nomepartita;//Indica il nome della partita
    int turn;//indica il turno del giocatore
    int pedinag1;//simbolo pedina del giocatore1
    int pedinag2;//simbolo pedina del giocatore2
    stringa nomeg1;//nome del Giocatore1
    stringa nomeg2;//nome del Giocatore2
    int matrice_pedine[righe * colonne];//matrice linearizzata contenente la posizione delle pedine sul campo da gioco
} partita;

//Crea una struttura dati contenente tutte le informazioni relative alle impostazioni di gioco
typedef struct t_config {
    int pg1;//simbolo pedina del giocatore1; di default: 2(Codice ASCII della pedina)
    int pg2;//simbolo pedina del giocatore2; di default: 1(Codice ASCII della pedina)
    stringa ng1;//nome del Giocatore1, di default: Giocatore1
    stringa ng2;//nome del Giocatore2, di default: Giocatore2
} config;

//Crea una struttura dati relativa alle coordinate di spostamento della pedina
typedef struct t_pedina {
    int cordy;//indica lo spostamento della pedina sulle colonne
    int cordx;//indica lo spostamento della pedina sulle righe
} pedina;

//Crea una struttura dati contenente informazioni riguardo i possibili spostamenti da un punto sul campo
typedef struct t_check {
    boolean dx;//Indica se la pedina si sta muovendo verso destra
    boolean sx;//Indica se la pedina si sta muovendo verso sinistra
    boolean up;//Indica se la pedina si sta muovendo verso verso l'alto
    boolean down;//Indica se la pedina si sta muovendo verso il basso
} check;

char matrice_grafica[righeg * colonneg];//Matrice linearizzata contenente la scacchiera di base del campo da gioco

/*------------------------------------------------------------*/
//prototipi funzioni
int main();

/*------------------------------------------------------------*/
//interfacce.c prototipi


void stampare_titolo(); //Stampa a video il titolo del gioco leggendolo da un file di testo
void stampare_interfaccia_menu();//Stampa a video l'interfaccia del menu' principale
void stampare_interfaccia_campo(
        partita *);//Stampa a video il campo secondo i parametri contenuti nella struttura dati "partita"
void stampare_interfaccia_impostazioni();//Stampa a video il menu' relativo alle impostazioni del gioco
void stampare_interfaccia_carica_partita();//Stampa a video il menu' relativo al caricamento delle partite salvate
void stampare_interfaccia_salva_partita();//Stampa a video il menu' relativo al salvataggio delle partite
void stampare_interfaccia_topten();//Stampa a video l'intefaccia relativa alla TopTen dei tempi di Record
void
stampare_interfaccia_cambiare_simbolo_pedine();//Stampa a video l'interfaccia relativa al menu' cambiare simbolo pedine
void
stampare_interfaccia_cambiare_nomi_giocatori();//Stampa a video l'interfaccia relativa al menu' per cambiare il nome ad entrambi i giocatori

//generale.c prototipi
partita nuova_partita(rec *, config *);//Funzione che permette all'utente di iniziare una nuova partita
partita carica_partita(FILE *);//Funzione che permette all'utente di caricare una partita precedentemente salvata
void salva_partita(FILE *, partita *);//Funzione che permette all'utente di salvare una partita in corso
void impostazioni(config *);//Funzione relativa che permette all'utente di cambiare le impostazioni del gioco
void topten();//Funzione che permette all'utente di visualizzare la lista dei tempi di Record conseguiti

//funzioni.c prototipi
void GotoXY(int, int);//Funzione che permette di spostare il cursore a video
void cancella_riga(int);//Funzione che permette di cancellare il contenuto di una sola riga a video
void creare_matrice_pedine(partita *,
                           config *);//Funzione che permette di creare una nuova matrice relativa ad una nuova partita
void creare_matrice_grafica();//Funzione che permette di implementare la "matrice_grafica"
void effettuare_mossa(partita *);//Funzione che permette ai giocatori di effettuare una mossa
void contatore_tempo(rec *, time_t, partita *);//Funzione che permette di calcolare la durata della partita
int inserire_coordinate_spostamento_pedina(int, int,
                                           pedina *);//Funzione che permette di inserire coordinate per spostare la pedina
int
controllare_coordinate_spostamento(pedina *);//Funzione che controllo se l'utente ha inserito coordinate valide o meno
boolean controllare_spostamento_pedina(partita *, pedina *, pedina *,
                                       check *);//Funzione che permette di controllare se e' possibile spostare la pedina
int controllare_colonna(pedina *, pedina *,
                        check *);//Funzione che permette di stabilire la direzione della pedina lungo la colonna
int controllare_riga(pedina *, pedina *,
                     check *);//Funzione che permette di stabilire la direzione della pedina lungo la riga
boolean controllare_linea_campo(pedina *,
                                check *);//Funzione che permette di controllare lo spostamento della pedina lungo una diagonale
boolean effettuare_spostamento(partita *, pedina *, pedina *);//Funzione che permette di spostare la pedina sul campo
void mangiare_pedina_avvicinamento(partita *, check *, pedina *, pedina *,
                                   int);//Funzione che permette di catturare le pedine con la modalita' per avvicinamento
void mangiare_pedina_allontanamento(partita *, check *, pedina *, pedina *,
                                    int);//Funzione che permette di catturare le pedine con la modalita' per allontanamento
void controllare_modalita_cattura(partita *, check *, pedina *, pedina *, int, boolean *,
                                  boolean *);//Funzione che permette di controllare quale modalit� di cattura e' possibile effetturare
int
scegliere_modalita_cattura();//Funzione che permette all'utente di scegliere la modalita' di cattura, nel caso in cui si verifichino entrambe le possibilita' di cattura
boolean controllare_pedine_campo(partita *, int);//Funzione che controlla se uno dei due giocatori ha vinto la partita
void cambiare_simbolo_pedine(config *);//Funzione che permette all'utente di cambiare il simbolo delle pedine
void scegliere_coppia_pedine(config *);//Funzione che permette all'utente di scegliere una coppia di pedine grafica
void cambiare_nome_giocatori(config *);//Funzione che permette all'utente di cambiare il nome dei giocatori
int scegliere_slot();//Funzione che permette all'utente di scegliere uno slot di salvataggio o di caricamento
void riprendere_partita(partita *);//Funzione che permette di riprendere una partita correttamente caricata
boolean controllare_nuovo_record(
        partita *);//Funzione che controlla, quando la partita e' terminata, se c'� stato un nuovo tempo record

//funzioni_file.c prototipi
void leggere_file_salvataggio(FILE *);//Funzione che permette di leggere il file di salvataggio delle partite
void creare_file_salvataggio(FILE *);//Funzione che permette di creare il file di salvataggio delle partite
void effettuare_salvataggio_partita(FILE *, int,
                                    partita *);//Funzione che permette di effettuare il salvataggio di una partita su file
boolean controllare_slot_salvataggio(FILE *,
                                     int);//Funzione che controlla se lo slot selezionato per il salvataggio della partita e' libero o meno
boolean controllare_slot_caricamento(FILE *,
                                     int);//Funzione che controlla se lo slot selezionato per il caricamento della partita contiene effettivamente una partita salvata
partita estrarre_partita(FILE *, int);//Funzione che estrae la partita selezionata dal file di salvataggio delle partite
void leggere_file_topten(FILE *);//Funzione che legge il file relativo alla TopTen dei tempi di record
void ordinamento_file_topten(partita *);//Funzione che ordina i tempi di record in ordine crescente




