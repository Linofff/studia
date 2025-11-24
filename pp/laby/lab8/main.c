#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define name_max_length 10000

typedef struct {
  char imie[name_max_length + 1];
  int liczba_kluczy;
} Osoba;

typedef struct {
  int numer_pokoju;
  Osoba *wlasciciel_klucza;
} Pokoj;

void przypisz_klucz(Pokoj *pokoj, Osoba *nowy_wlasciciel) {
  if (pokoj->wlasciciel_klucza != NULL) {
    Osoba *stary_wlasciciel = pokoj->wlasciciel_klucza;
    stary_wlasciciel->liczba_kluczy--;

    if (stary_wlasciciel->liczba_kluczy == 0) {
      free(stary_wlasciciel);
    }
  }

  pokoj->wlasciciel_klucza = nowy_wlasciciel;

  if (nowy_wlasciciel != NULL) {
    nowy_wlasciciel->liczba_kluczy++;
  }
}

void komenda_D(Pokoj *pokoje, int n) {
  int indeks;
  char bufor_imie[10001];

  scanf("%d %s", &indeks, bufor_imie);

  if (indeks < 0 || indeks >= n)
    return;

  Osoba *nowa_osoba = (Osoba *)malloc(sizeof(Osoba));
  if (nowa_osoba == NULL) {
    fprintf(stderr, "Blad alokacji pamieci!\n");
    exit(1);
  }

  strcpy(nowa_osoba->imie, bufor_imie);
  nowa_osoba->liczba_kluczy = 0;

  przypisz_klucz(&pokoje[indeks], nowa_osoba);
}

void komenda_B(Pokoj *pokoje, int n) {
  int indeks_zrodlowy;
  int szukany_numer_pokoju;
  scanf("%d %d", &indeks_zrodlowy, &szukany_numer_pokoju);

  if (indeks_zrodlowy < 0 || indeks_zrodlowy >= n ||
      pokoje[indeks_zrodlowy].wlasciciel_klucza == NULL) {
    printf("!\n");
    return;
  }

  Osoba *dawca = pokoje[indeks_zrodlowy].wlasciciel_klucza;

  int indeks_celu = -1;
  for (int i = 0; i < n; i++) {
    if (pokoje[i].numer_pokoju == szukany_numer_pokoju) {
      indeks_celu = i;
      break;
    }
  }

  if (indeks_celu == -1) {
    printf("!\n");
    return;
  }

  przypisz_klucz(&pokoje[indeks_celu], dawca);
}

void komenda_P(const Pokoj *pokoje, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d", pokoje[i].numer_pokoju);
    if (pokoje[i].wlasciciel_klucza != NULL) {
      printf(" %s", pokoje[i].wlasciciel_klucza->imie);
    }
    printf("\n");
  }
}

void wyczysc_pamiec(Pokoj *pokoje, int n) {
  for (int i = 0; i < n; i++) {
    if (pokoje[i].wlasciciel_klucza != NULL) {
      Osoba *osoba = pokoje[i].wlasciciel_klucza;
      osoba->liczba_kluczy--;

      if (osoba->liczba_kluczy == 0) {
        free(osoba);
      }
      pokoje[i].wlasciciel_klucza = NULL;
    }
  }
  free(pokoje);
}

int main(int argc, char *argv[]) {
  int n;
  if (scanf("%d", &n) != 1) {
    printf("error");
    return 0;
  }
  Pokoj *pokoje = (Pokoj *)malloc(n * sizeof(Pokoj));
  if (pokoje == NULL) {
    printf("error");
    return 1;
  }

  for (int i = 0; i < n; i++) {
    scanf("%d", &pokoje[i].numer_pokoju);
    pokoje->wlasciciel_klucza = NULL;
  }

  char komenda;
  while (scanf(" %c", &komenda) != EOF) {
    if (komenda == 'Q') {
      break;
    } else if (komenda == 'D') {
      komenda_D(pokoje, n);
    } else if (komenda == 'B') {
      komenda_B(pokoje, n);
    } else if (komenda == 'P') {
      komenda_P(pokoje, n);
    }
  }

  wyczysc_pamiec(pokoje, n);

  return 0;
}
