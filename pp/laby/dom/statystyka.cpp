#include <cstdlib>
#include <ctime>
#include <iostream>
#define SIZE 250

#define SIZE2 100

using namespace std;

int main() {
  const double ratings[] = {2, 3, 3.5, 4, 4.5, 5, 5.5};
  int results[SIZE], stat[7] = {}; // wszystkie elementy = 0
  srand(time(NULL));

  // ---- Losowanie ocen z zadanymi prawdopodobieństwami ----
  // 2 -> 20%, 3 -> 15%, 3.5 -> 25%, 4 -> 20%, 4.5 -> 10%, 5 -> 10%, 5.5 -> 5%
  int probs[] = {20, 15, 25, 20, 10, 10, 5};

  for (int i = 0; i < SIZE; i++) {
    int p = rand() % 100 + 1; // liczba od 1 do 100
    int sum = 0, idx = 0;
    while (p > sum + probs[idx]) {
      sum += probs[idx];
      idx++;
    }
    results[i] = idx;
  }

  // ---- Liczenie statystyk ----
  for (int i = 0; i < SIZE; i++)
    stat[results[i]]++;

  cout << "Statystyka ocen:\n";
  for (int i = 0; i < 7; i++)
    cout << ratings[i] << " - " << stat[i] << endl;

  // ---- Ile ocen powyżej 2 ----
  int above2 = 0;
  double sum = 0;
  for (int i = 0; i < SIZE; i++) {
    sum += ratings[results[i]];
    if (ratings[results[i]] > 2)
      above2++;
  }

  cout << "\nLiczba ocen powyżej 2: " << above2 << endl;

  // ---- Średnia ocen ----
  double avg = sum / SIZE;
  cout << "Średnia ocen: " << avg << endl;

  // ---- Ile ocen powyżej średniej ----
  int aboveAvg = 0;
  for (int i = 0; i < SIZE; i++)
    if (ratings[results[i]] > avg)
      aboveAvg++;
  cout << "Liczba ocen powyżej średniej: " << aboveAvg << endl;

  // ---- Grupy po 25 osób ----
  int groups = SIZE / 25;
  double groupAvg[groups] = {};

  for (int g = 0; g < groups; g++) {
    double s = 0;
    for (int i = g * 25; i < (g + 1) * 25; i++)
      s += ratings[results[i]];
    groupAvg[g] = s / 25.0;
  }

  int best = 0, worst = 0;
  for (int g = 1; g < groups; g++) {
    if (groupAvg[g] > groupAvg[best])
      best = g;
    if (groupAvg[g] < groupAvg[worst])
      worst = g;
  }

  cout << "\nNajlepsza grupa: " << best + 1 << " (średnia " << groupAvg[best]
       << ")" << endl;
  cout << "Najgorsza grupa: " << worst + 1 << " (średnia " << groupAvg[worst]
       << ")" << endl;

  return 0;
}
