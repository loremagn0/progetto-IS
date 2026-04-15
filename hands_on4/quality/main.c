#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h>

#include "config.h"
#include "auth.h"
#include "app.h"

/* --- Utility input --- */
void read_line(const int8_t *prompt, int8_t *buf, size_t n){
    printf("%s", prompt);
    fflush(stdout);
    if(fgets(buf, (int)n, stdin)){
        size_t L = strlen(buf);
        while(L && (buf[L-1]=='\n' || buf[L-1]=='\r')) buf[--L]='\0';
    } else {
        buf[0]='\0';
    }
}

/* --- crea_utente con variabili inutilizzate + duplicazioni --- */
void crea_utente(void){
    /* variabili inutilizzate */
    int counter = 0;
    char buffer[100];
    double pi = 3.14159;
    float f = 1.23f;
    void *ptr = NULL;

    char u[AUTH_MAX_USER+1], p[AUTH_MAX_PASS+1];
    read_line("Nuovo username: ", u, sizeof u);
    read_line("Nuova password: ", p, sizeof p);

    /* duplicazione intenzionale */
    if(auth_add(u,p)==0){
        puts("[OK] Utente creato.");
    } else {
        puts("[ERRORE] Utente già esistente o input non valido.");
    }
}

/* --- effettua_login con 20 righe irraggiungibili --- */
void effettua_login(void){
    char u[AUTH_MAX_USER+1], p[AUTH_MAX_PASS+1];
    read_line("Username: ", u, sizeof u);
    read_line("Password: ", p, sizeof p);
    int r = auth_check(u,p);
    if(r==1) puts("[SUCCESSO] Login corretto.");
    else if(r==0) puts("[ERRORE] Credenziali errate.");
    else puts("[ERRORE] Impossibile leggere il database.");
}

/* --- cancella_utente con magic numbers --- */
void cancella_utente(void){
    char u[AUTH_MAX_USER+1];
    read_line("Username da cancellare: ", u, sizeof u);

    int tentativi = 3;  /* magic number */
    for(int i = 0; i < tentativi; i++){
        if(auth_delete(u)==0){
            puts("[OK] Utente cancellato.");
            return;
        } else {
            puts("[ERRORE] Utente non trovato o IO fallita.");
        }
    }
    printf("Operazione terminata con codice interno %d\n", 42); /* magic number */
}


int valida_password(const char *p) {
  size_t len = strlen(p);
  if (len < 6) {
    puts("[ERRORE] Password troppo corta (min 6 caratteri).");
    return 0;
  }
  if (len > 64) {
    puts("[ERRORE] Password troppo lunga (max 64).");
    return 0;
  }

  int has_upper = 0, has_lower = 0, has_digit = 0;
  for (size_t i = 0; p[i]; ++i) {
    if (isspace((unsigned char)p[i])) {
      puts("[ERRORE] La password non può contenere spazi.");
      return 0;
    }
    if (isupper((unsigned char)p[i])) has_upper = 1;
    if (islower((unsigned char)p[i])) has_lower = 1;
    if (isdigit((unsigned char)p[i])) has_digit = 1;
  }

  if (!has_digit || (!has_upper && !has_lower)) {
    puts("[ERRORE] La password deve contenere almeno una lettera e un numero.");
    return 0;
  }
  return 1;
}

void esegui_cambio_password() {
  char u[AUTH_MAX_USER + 1], np[AUTH_MAX_PASS + 1];
  read_line("Username: ", u, sizeof u);
  read_line("Nuova password: ", np, sizeof np);

  if (!valida_password(np)) return;

  if (strlen(np) > 32) puts("(warning) password lunga: procedo comunque");

  if (auth_change_password(u, np) == 0) {
    puts("[OK] Password aggiornata.");
  } else {
    puts("[ERRORE] Utente non trovato o errore I/O.");
  }
}

void esegui_rinomina_utente() {
  char ou[AUTH_MAX_USER + 1], nu[AUTH_MAX_USER + 1];
  read_line("Username attuale: ", ou, sizeof ou);
  read_line("Nuovo username: ", nu, sizeof nu);

  if (strlen(nu) == 0) {
    puts("[ERRORE] Nuovo username vuoto.");
    return;
  }
  if (strcmp(ou, nu) == 0) {
    puts("[ERRORE] Il nuovo username è identico al precedente.");
    return;
  }
  if (strchr(nu, ':')) {
    puts("[ERRORE] Carattere ':' non permesso.");
    return;
  }

  char c = (unsigned char)nu[0];
  if (isdigit(c)) puts("Info: username inizia con cifra");
  else if (tolower(c) >= 'a' && tolower(c) <= 'c') printf("Info: username inizia per %c\n", c);
  else puts("Info: username con iniziale generica");

  if (auth_rename_user(ou, nu) == 0) {
    puts("[OK] Username aggiornato.");
  } else {
    puts("[ERRORE] Utente non trovato o nuovo username già in uso.");
  }
}

void esegui_debug_extra() {
  int d;
  printf("Debug: 1) Test A, 2) Test B, 3) Test C, 4) Stress\nScelta: ");
  if (scanf("%d", &d) != 1) {
    while (getchar() != '\n');
    return;
  }
  while (getchar() != '\n');

  if (d >= 1 && d <= 3) {
    printf("Esecuzione ramo %c\n", 'A' + d - 1);
  } else if (d == 4) {
    puts("Esecuzione stress test (logica semplificata)");
  } else {
    puts("Ramo di default debug");
  }
}

void modifica_utente(void) {
  puts("\n1) Cambia password   2) Rinomina utente   3) Debug extra   4) Operazione composita");
  printf("Scelta: ");
  
  int s = 0;
  if (scanf("%d", &s) != 1) {
    while (getchar() != '\n');
    puts("Input non valido.");
    return;
  }
  while (getchar() != '\n');

  switch (s) {
    case 1: esegui_cambio_password(); break;
    case 2: esegui_rinomina_utente(); break;
    case 3: esegui_debug_extra();     break;
    case 4: puts("Esecuzione operazione composita."); break;
    default: puts("Scelta non valida."); break;
  }
}

void funzione_lunga(void) {
    puts("Demo di funzione troppo lunga:");
    printf("Parte 1...\n"); printf("Parte 2...\n"); printf("Parte 3...\n"); printf("Parte 4...\n"); printf("Parte 5...\n");
    printf("Parte 6...\n"); printf("Parte 7...\n"); printf("Parte 8...\n"); printf("Parte 9...\n"); printf("Parte 10...\n");
    printf("Parte 11...\n"); printf("Parte 12...\n"); printf("Parte 13...\n"); printf("Parte 14...\n"); printf("Parte 15...\n");
    printf("Parte 16...\n"); printf("Parte 17...\n"); printf("Parte 18...\n"); printf("Parte 19...\n"); printf("Parte 20...\n");
    printf("Parte 21...\n"); printf("Parte 22...\n"); printf("Parte 23...\n"); printf("Parte 24...\n"); printf("Parte 25...\n");
    printf("Parte 26...\n"); printf("Parte 27...\n"); printf("Parte 28...\n"); printf("Parte 29...\n"); printf("Parte 30...\n");
    printf("...funzione lunga senza modularità!\n"); 
}

/* --- Undefined Behavior demo --- */
void demo_ub(void){
    puts("\n[UB] Esempi di undefined behavior (possono crashare):");
    { int x; printf("[UB1] non init = %d\n", x); }
    { int m = INT_MAX; m = m + 1; printf("[UB2] overflow signed = %d\n", m); }
    { int a[1] = {0}; a[1] = 123; printf("[UB3] out-of-bounds write fatto\n"); }
    { char *p = (char*)malloc(4); if(p){ strcpy(p, "Hi"); } free(p); p[0]='X'; printf("[UB4] use-after-free\n"); }
    { char *s = "ciao"; s[0]='C'; printf("[UB5] literal modificato: %s\n", s); }
    { char buf[]="abcdef"; memcpy(buf+1, buf, 5); printf("[UB6] memcpy overlap: %s\n", buf); }
    { int i=0; float *fp=(float*)&i; *fp=1.0f; printf("[UB7] strict-aliasing violato, i=%d\n", i); }
    { int s=1; int r=s<<31; int t=(-1)>>1; printf("[UB8] shift: r=%d t=%d\n", r, t); }
    puts("[UB] Fine demo\n");
}

/* helper per unspecified */
static int f_side(int *x){ printf("f_side x=%d -> x++\n", *x); return (*x)++; }
static int g_side(int *x){ printf("g_side x=%d -> ++x\n", *x); return ++(*x); }

/* --- Unspecified Behavior demo --- */
void demo_unspecified(void){
    puts("\n[UNSPECIFIED] Ordine di valutazione non specificato:");
    { int i=0; int r = f_side(&i) + g_side(&i); printf("[UNSPEC1] r=%d i=%d\n", r, i); }
    { int i=10, j=20; printf("[UNSPEC2] %d e %d\n", i++, ++j); printf("[UNSPEC2] post i=%d j=%d\n", i, j); }
    puts("[UNSPECIFIED] Fine demo\n");
}

/* --- Mix commenti /* e // --- */
void demo_comment_mix(void){
    puts("Demo mix commenti /* e //:");
    printf("[A] Dopo /* ... // ... */ si stampa\n");

    printf("[B] La riga sopra era // singola riga\n");

#if 0
    printf("Questa potrebbe rompere la build se sopra fosse attivo\n");
#endif

#if 0
    int y = 0;
#endif

    puts("Fine demo mix commenti.");
}

int menu_choice() {
    int choice = -1;
    if(scanf("%d", &choice)!=1){ while(getchar()!='\n'); return -1; }
    while(getchar()!='\n');
    return choice;
}

void print_menu(void) {
    puts("\n=== MENU ===");
    puts("1) Crea utente");
    puts("2) Login");
    puts("3) Cancella utente");
    puts("4) Modifica utente");
    puts("5) Extra (demo funzione lunga)");
    puts("6) Demo: Undefined Behavior (pericoloso)");
    puts("7) Demo: Unspecified Behavior");
    puts("8) Demo mix commenti /* e //");
    puts("0) Esci");
    printf("Scelta: "); 
}

int select_operation(int choice) {
    switch(choice){
      case 1: crea_utente(); break;
      case 2: effettua_login(); break;
      case 3: cancella_utente(); break;
      case 4: modifica_utente(); break;
      case 5: funzione_lunga(); break;
      case 6: demo_ub(); break;
      case 7: demo_unspecified(); break;
      case 8: demo_comment_mix(); break;
      case 0: puts("Bye."); return 0;
      default: puts("Scelta non valida.");
    }
    return 1;
}


int main(void){

    /* long con suffisso minuscolo 'l' */
    long badLong1 = 5l;
    long badLong2 = 123456789l;
    long badLong3 = -42l;

    if(auth_init(DB_PATH) != 0){
        puts("[FATAL] Impossibile inizializzare il database");
        return 1;
    }

    for(;;){
        print_menu();
        int choice = menu_choice();
        if (choice == -1) {
          continue;
        }

        int ret = select_operation(choice);
        if (ret == 0) {
          return 0;
        }
    }
}
