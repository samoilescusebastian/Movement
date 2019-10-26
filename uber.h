// Copyright 2019 Samoilescu Sebastian
#ifndef UBER_H_
#define UBER_H_
#include <string>
#include <vector>
#include <queue>
#include <list>
#include "./Map.h"
#include "./Node.h"
#include "./driver.h"
#define MAX_DRIVERS 150
template <typename keyType, typename itemType> class DriversList {
 private:
  int maxHeight;
  int drivers;
  int (*hash)(keyType, int);

 public:
  std::list<Driver> *driversList;
  explicit DriversList(int (*hash)(keyType, int));
  DriversList();
  Driver *getDriver(keyType);
  Driver *addDriver(Driver);
  void listDrivers();
  ~DriversList();
};
template <typename keyType, typename itemType>
DriversList<keyType, itemType>::DriversList(int (*hash)(keyType, int)) {
  maxHeight = MAX_DRIVERS;
  drivers = 0;
  driversList = new std::list<Driver>[maxHeight];
  this->hash = hash;
}
template <typename keyType, typename itemType>
DriversList<keyType, itemType>::DriversList() {}
template <typename keyType, typename itemType>
Driver *DriversList<keyType, itemType>::addDriver(Driver driver) {
  int index = hash(driver.getName(), maxHeight);
  driversList[index].push_back(driver);
  return &(driversList[index].back());
}
template <typename keyType, typename itemType>
Driver *DriversList<keyType, itemType>::getDriver(keyType name) {
  int index = hash(name, maxHeight);
  typename std::list<Driver>::iterator it = driversList[index].begin();
  for (; it != driversList[index].end(); ++it) {
    if (it->getName() == name) {
      return &(*it);
    }
  }
  return NULL;
}
template <typename keyType, typename itemType>
DriversList<keyType, itemType>::~DriversList() {
  for (int i = 0; i < maxHeight; i++) {
    while (!driversList[i].empty()) {
      driversList[i].pop_back();
    }
  }
  delete[] driversList;
}
// functia pentru a cauta pe harta soferul optim pentru a raspunde la o comanda
template <typename keyType, typename itemType>
Driver *findDriver(Map<keyType, itemType> &map, std::string start,
                   int &distance) {
  // pornim un DFS de la locatia clientului si ne oprim pe nivelul unde am
  // gasit un sofer, de pe acest nivel selectam soferul optim
  int maxHeight = map.getHeight();
  std::vector<int> visited(maxHeight, 0);
  std::queue<Node<keyType, itemType> *> queue;
  Node<keyType, itemType> *node = map.getItem(start);
  if (node->getSize() != 0) {
    return node->getBestDriver();
  }
  Driver *driver = NULL, *tmpDriver = NULL;
  int dist = 0;
  queue.push(node);
  visited[node->getId()] = 1;
  while (!queue.empty()) {
    Node<keyType, itemType> *currNode = queue.front();
    queue.pop();
    if (dist != visited[currNode->getId()]) {
      if (driver != NULL) {
        distance += dist;
        return driver;
      }
    }
    dist = visited[currNode->getId()];
    typename std::list<Node<keyType, itemType> *>::iterator it =
        currNode->neigh_in.begin();
    for (; it != currNode->neigh_in.end(); ++it) {
      if (visited[(*it)->getId()] == 0) {
        if ((*it)->getSize() != 0) {
          if (driver == NULL) {
            driver = (*it)->getBestDriver();
          } else {
            tmpDriver = (*it)->getBestDriver();
            if (tmpDriver->getRating() > driver->getRating()) {
              driver = tmpDriver;
            } else if (tmpDriver->getRating() == driver->getRating()) {
              if (tmpDriver->getName() < driver->getName()) {
                driver = tmpDriver;
              }
            }
          }
        }
        visited[(*it)->getId()] = visited[currNode->getId()] + 1;
        queue.push(*it);
      }
    }
  }
  return NULL;
}
// functia care realizeaza comanda in sine
template <typename keyType, typename itemType>
std::string takeRide(Map<keyType, itemType> &map, std::string start,
                     std::string end, int &distance) {
  int neigh = (map.getItem(end))->neigh_out.size();
  std::vector<std::string> dest(neigh + 1);
  dest[0] = end;
  // retinem vecinii destinatiei pentru ca in cazul in care
  // destinatia propriu-zisa nu poate fi atinsa, aceasta sa fie
  // inlocuita cu un vecin
  typename std::list<Node<keyType, itemType> *>::iterator it =
      (map.getItem(end))->neigh_out.begin();
  for (int i = 1; i <= neigh; i++) {
    dest[i] = (*it)->getKey();
    it++;
  }
  distance += map.distance(start, dest);
  std::string reachedDest = "NULL";
  std::string possibleDest = "NULL";
  for (int i = 0; i <= neigh && reachedDest == "NULL"; i++) {
    if (dest[i][dest[i].size() - 1] == '1') {
      reachedDest = dest[i].substr(0, dest[i].size() - 1);
    } else if (dest[i][dest[i].size() - 1] == '2') {
      possibleDest = dest[i].substr(0, dest[i].size() - 1);
    }
  }
  // daca distanta nu a fost atinsa, returnam unul dintre vecinii atinsi
  if (reachedDest == "NULL") {
    return possibleDest;
  }
  return reachedDest;
}
#endif  //  UBER_H_
