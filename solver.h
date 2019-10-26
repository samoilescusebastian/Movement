// Copyright 2019 SD_Homework_Team
// Copyright 2019 Samoilescu Sebastian
#ifndef SOLVER_H_
#define SOLVER_H_
#include <fstream>
#include <string>
#include <vector>
#include "./Map.h"
#include "./uber.h"
int compute_hash(std::string s, int m) {
  const int p = 31;
  int hash_value = 0;
  int power = 1;
  for (char c : s) {
    hash_value = (hash_value + (c - 'a' + 1) * power) % m;
    power = (power * p) % m;
  }
  return hash_value;
}
class solver {
 public:
  int gas;
  Driver lastDriver;
  Map<std::string, int> *map;
  int streetsNumber;
  int **distances;
  int getPivot(std::vector<std::string> &array, int left, int right) {
    int leftIndex = left, rightIndex = right, moveLeft = 0, moveRight = 1;
    while (leftIndex < rightIndex) {
      if (array[leftIndex] > array[rightIndex]) {
        std::string aux = array[leftIndex];
        array[leftIndex] = array[rightIndex];
        array[rightIndex] = aux;
        moveLeft = 1 - moveLeft;
        moveRight = 1 - moveRight;
      }
      leftIndex += moveLeft;
      rightIndex -= moveRight;
    }
    return leftIndex;
  }
  void QuickSort(std::vector<std::string> &array, int left, int right) {
    if (left < right) {
      int pivotPos = getPivot(array, left, right);
      QuickSort(array, left, pivotPos - 1);
      QuickSort(array, pivotPos + 1, right);
    }
  }
  void sortArray(std::vector<std::string> &array) {
    int left = 0, right = array.size() - 1;
    QuickSort(array, left, right);
  }
  void task1_solver(std::ifstream &fin, std::ofstream &fout) {
    int verticesNumber, edgesNumber;
    fin >> verticesNumber >> edgesNumber;
    map = new Map<std::string, int>(verticesNumber, compute_hash);
    std::string streetName;
    int id = 0, i = 1;
    // adaugare intersectie in mapa
    while (i <= verticesNumber) {
      fin >> streetName;
      map -> addVertex(streetName, id);
      i++;
      id++;
    }
    streetsNumber = verticesNumber;
    std::string streetName1;
    i = 1;
    // adaugare drum intre doua intersectii
    while (i <= edgesNumber) {
      fin >> streetName >> streetName1;
      i++;
      map->addEdge(streetName, streetName1);
    }
    int queries;
    fin >> queries;
    std::vector<std::string> dest(1);
    // verificare existenta drum intre interectii
    for (int i = 1; i <= queries; i++) {
      fin >> streetName >> dest[0];
      if (map->distance(streetName, dest) == -1) {
        fout << "n\n";
      } else {
        fout << "y\n";
      }
    }
  }
  void task2_solver(std::ifstream &fin, std::ofstream &fout) {
    int queries;
    fin >> queries;
    std::string streetName;
    std::vector<std::string> dest(1);
    // calcul de distante intre intersectii
    for (int i = 1; i <= queries; i++) {
      fin >> streetName >> dest[0];
      fout << map->distance(streetName, dest) << "\n";
    }
  }

  void task3_solver(std::ifstream &fin, std::ofstream &fout) {
    int queries;
    char type;
    int number;
    std::string streets[3];
    fin >> queries;
    for (int i = 1; i <= queries; i++) {
      fin >> type >> streets[0] >> streets[1] >> number;
      if (type == 'q' && number == 2) {
        fin >> streets[2];
      }
      // modificare harta
      if (type == 'c') {
        number++;
        switch (number) {
        case 1:
          map->addEdge(streets[0], streets[1]);
          break;
        case 2:
          map->deleteWays(streets[0], streets[1]);
          break;
        case 3:
          map->biDirectional(streets[0], streets[1]);
          break;
        case 4:
          map->reverseWay(streets[0], streets[1]);
          break;
        }
        // interogare harta
      } else if (type == 'q') {
        number++;
        std::vector<std::string> dest(1);
        switch (number) {
        case 1:
          dest[0] = streets[1];
          if (map->distance(streets[0], dest) == -1) {
            fout << "n\n";
          } else {
            fout << "y\n";
          }
          break;
        case 2:
          dest[0] = streets[1];
          fout << map->distance(streets[0], dest) << "\n";
          break;
        case 3:
          dest[0] = streets[2];
          int distance = map->distance(streets[0], dest);
          dest[0] = streets[1];
          int distance2 = map->distance(streets[2], dest);
          if (distance == -1 || distance2 == -1) {
            fout << -1 << "\n";
          } else {
            fout << distance + distance2 << "\n";
          }
          break;
        }
      }
    }
  }

  void task4_solver(std::ifstream &fin, std::ofstream &fout) {
    int queries;
    fin >> queries;
    std::string param[4];
    Driver driver;
    Driver *driverRef;
    Driver *driverInList;
    // hash pentru a retine soferii
    DriversList<std::string, int> driversList(compute_hash);
    Node<std::string, int> *intersection;
    // folosim cate o structura de tip skipList pentru a avea sortati soferii
    // dupa un anumit criteriu
    Ranking *dist_ranking = new Ranking(5, 0.6, 1);
    Ranking *rating_ranking = new Ranking(5, 0.6, 2);
    Ranking *rides_ranking = new Ranking(5, 0.6, 3);
    for (int i = 0; i < queries; ++i) {
      fin >> param[0];
      if (param[0] == "d") {
        fin >> param[1] >> param[2];
        // initializare sofer
        driver.setId(0);
        driver.setName(param[1]);
        intersection = map->getItem(param[2]);
        driver.setPosition(param[2]);
        driverInList = driversList.getDriver(param[1]);
        // adaugare in lista in cazul in care nu exista
        if (driverInList == NULL) {
          driverRef = driversList.addDriver(driver);
          dist_ranking->addItem(driverRef);
          rating_ranking->addItem(driverRef);
          rides_ranking->addItem(driverRef);
          // adaugare sofer pe harta
          intersection->addDriver(driverRef);
        // actualizare a soferului existent
        } else {
          driverInList->setStatus(true);
          intersection = map->getItem(param[2]);
          driverInList->setPosition(param[2]);
          intersection->addDriver(driverInList);
        }
        // retragerea soferului de pe harta
      } else if (param[0] == "b") {
        fin >> param[1];
        driverRef = driversList.getDriver(param[1]);
        driverRef->setStatus(false);
        intersection = map->getItem(driverRef->getPosition());
        intersection->deleteDriver(driverRef);
        // primirea unei comenzi
      } else if (param[0] == "r") {
        fin >> param[1] >> param[2] >> param[3];
        int distance = 0;
        std::vector<std::string> dest(1);
        // cautarea soferului ideal pentru a realiza cursa
        Driver *driver = findDriver(*map, param[1], distance);
        if (driver != NULL) {
          std::string finalDest;
          // verificare daca cursa poate fi indeplinita
          finalDest = takeRide(*map, param[1], param[2], distance);
          if (finalDest != "NULL") {
            // stergerea soferului din toate clasamentele,
            // actualizarea informatiilor acestuia si reasezarea acestuia pe
            // harta
            dist_ranking->deleteItem(driver);
            driver->addDistance(distance);
            dist_ranking->addItem(driver);
            intersection = map->getItem(driver->getPosition());
            intersection->deleteDriver(driver);
            rating_ranking->deleteItem(driver);
            rides_ranking->deleteItem(driver);
            driver->addRating(stoi(param[3]));
            rating_ranking->addItem(driver);
            rides_ranking->addItem(driver);
            driver->setPosition(finalDest);
            intersection = map->getItem(driver->getPosition());
            intersection->addDriver(driver);
          } else {
            fout << "Destinatie inaccesibila\n";
          }
        } else {
          fout << "Soferi indisponibili\n";
        }

      } else if (param[0] == "top_rating") {
        fin >> param[1];
        rating_ranking->printRatings(fout, stoi(param[1]));
      } else if (param[0] == "top_dist") {
        fin >> param[1];

        dist_ranking->printDistances(fout, stoi(param[1]));
      } else if (param[0] == "top_rides") {
        fin >> param[1];
        rides_ranking->printRides(fout, stoi(param[1]));
      } else {
        fin >> param[1];
        driversList.getDriver(param[1])->printInfos(fout);
      }
    }
    std::string driverName;
    // citirea datelor de la task5 pentru a nu mai pastra lista de soferi
    // in mod inutil
    fin >> gas;
    fin >> driverName;
    // determinarea soferului responsabil pentru task5
    lastDriver = *(driversList.getDriver(driverName));
    delete rides_ranking;
    delete dist_ranking;
    delete rating_ranking;
  }

  void task5_solver(std::ifstream &fin, std::ofstream &fout) {
    int intersectionsNumber;
    fin >> intersectionsNumber;
    std::vector<std::string> intersections(intersectionsNumber);
    for (int  i = 0; i < intersectionsNumber; i++) {
      fin >> intersections[i];
    }
    // determinarea costului pentru ca soferul sa se deplaseze la
    // fiecare intersectie
    map->getCosts(lastDriver.getPosition(), intersections);
    // sortam rezultatul
    sortArray(intersections);
    unsigned int i = 0;
    // sar peste intersectiile la care nu s-a putut ajunge
    while (intersections[i][0] == '-' && i < intersections.size()) {
      i++;
    }
    // afisarea in ordine a intersectiilor ce pot fi atinse
    while (i < intersections.size() && intersections[i][0] - '0' <= gas) {
      for (unsigned int j = 1; j < intersections[i].size(); j++) {
        fout << intersections[i][j];
      }
      fout << " ";
      i++;
    }
    delete map;
  }
};
#endif  //  SOLVER_H_

