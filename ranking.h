// Copyright 2019 Samoilescu Sebastian
#ifndef RANKING_H_
#define RANKING_H_
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include "./driver.h"
class Item {
 private:
  Driver *info;

 public:
  Item(Driver *, int);
  int getRating();
  int getDistance();
  Driver *getInfo();
  ~Item();
  Item **forward;
};
Item::Item(Driver *info, int height) {
  this->info = info;
  forward = new Item *[height + 1];
  // memset(forward, 0, sizeof(Item *) * (height + 1));
  for (int i = 0; i <= height; i++) {
    forward[i] = NULL;
  }
}
Item::~Item() { delete[] forward; }
int Item::getRating() { return info->getRating(); }
int Item::getDistance() { return info->getDistance(); }
Driver *Item::getInfo() { return info; }
class Ranking {
 private:
  // 1 = distance  2 = rating 3 = rides
  int type;
  int maxLevels;
  int levels;
  Item *firstItem;
  float proc;
  int length = 0;

 public:
  Ranking(int, float, int);
  Ranking(const Ranking &other);
  Ranking();
  Ranking &operator=(const Ranking &other);
  int randomLevel();
  void addItem(Driver *);
  void updateItem(Driver *, Item *);
  void insertItem(Driver *, Item *);
  void insertItem1(Driver *, Item *);
  Item *deleteItem(Driver *);
  void printRanking();
  void printDistances(std::ofstream &, int);
  void printRatings(std::ofstream &, int);
  void printRides(std::ofstream &, int);
  int getLength();
  Driver *getFirstItem();
  ~Ranking();
};
Ranking::Ranking(int maxLevels, float proc, int type) {
  levels = 0;
  length = 0;
  this->maxLevels = maxLevels;
  this->proc = proc;
  this->type = type;
  firstItem = new Item(NULL, maxLevels);
}
Ranking::Ranking(const Ranking &other) {
  this->maxLevels = other.maxLevels;
  this->levels = other.levels;
  this->proc = other.proc;
  this->length = other.length;
  this->type = other.type;
  firstItem = new Item(NULL, this->maxLevels);
  Item *current = other.firstItem;
  Driver *info;
  while (current->forward[0] != NULL) {
    info = current->forward[0]->getInfo();
    this->addItem(info);
    current = current->forward[0];
  }
}
Ranking &Ranking::operator=(const Ranking &other) {
  this->levels = other.levels;
  this->proc = other.proc;
  this->length = other.length;
  this->type = other.type;
  firstItem = new Item(NULL, maxLevels);
  Item *current = other.firstItem;
  Driver *info;
  while (current->forward[0] != NULL) {
    info = current->forward[0]->getInfo();
    this->addItem(info);
    current = current->forward[0];
  }
  return *this;
}
int Ranking::randomLevel() {
  float random = (float)rand() / RAND_MAX;
  int rLevel = 0;
  while (rLevel < maxLevels && random <= proc) {
    rLevel++;
    random = (float)rand() / RAND_MAX;
  }
  return rLevel;
}
void Ranking::addItem(Driver *info) { insertItem1(info, firstItem); }
void Ranking::updateItem(Driver *info, Item *item) { insertItem1(info, item); }
void Ranking::insertItem1(Driver *info, Item *startItem) {
  Item *currItem = startItem;
  Item *newEl[maxLevels + 1];
  memset(newEl, 0, sizeof(Item *) * (maxLevels + 1));
  float firstOperand = 0, secondOperand = 0;
  for (int i = levels; i >= 0; i--) {
    if (currItem->forward[i] != NULL) {
      // selectam informatia necesara in functie de tipul stucturii
      if (type == 1) {
        firstOperand = currItem->forward[i]->getInfo()->getDistance();
        secondOperand = info->getDistance();
      } else if (type == 2) {
        firstOperand = currItem->forward[i]->getInfo()->getRating();
        secondOperand = info->getRating();
      } else if (type) {
        firstOperand = currItem->forward[i]->getInfo()->getRides();
        secondOperand = info->getRides();
      }

    } else {
      firstOperand = -2;
    }
    // cautam locul pentru noul element
    while (firstOperand > secondOperand) {
      currItem = currItem->forward[i];
      if (currItem->forward[i] != NULL) {
        if (type == 1) {
          firstOperand = currItem->forward[i]->getInfo()->getDistance();
        } else if (type == 2) {
          firstOperand = currItem->forward[i]->getInfo()->getRating();
        } else {
          firstOperand = currItem->forward[i]->getInfo()->getRides();
        }
      } else {
        firstOperand = -2;
      }
    }
    // sortam dupa numele soferilor
    while (firstOperand == secondOperand &&
           currItem->forward[i]->getInfo()->getName().compare(info->getName()) <
               0) {
      currItem = currItem->forward[i];
      if (currItem->forward[i] != NULL) {
        if (type == 1) {
          firstOperand = currItem->forward[i]->getInfo()->getDistance();
        } else if (type == 2) {
          firstOperand = currItem->forward[i]->getInfo()->getRating();
        } else {
          firstOperand = currItem->forward[i]->getInfo()->getRides();
        }
      } else {
        firstOperand = -2;
      }
    }

    newEl[i] = currItem;
  }
  currItem = currItem->forward[0];
  int ranLvl = randomLevel();
  if (ranLvl > levels) {
    for (int i = levels + 1; i <= ranLvl; i++) {
      newEl[i] = firstItem;
    }
    levels = ranLvl;
  }
  Item *item = new Item(info, maxLevels);

  length++;
  for (int i = 0; i <= ranLvl; i++) {
    item->forward[i] = newEl[i]->forward[i];
    newEl[i]->forward[i] = item;
  }
}
Item *Ranking::deleteItem(Driver *info) {
  Item *leftItems[levels + 1];
  Item *currItem = firstItem;
  float firstOperand = 0, secondOperand = 0;

  for (int i = levels; i >= 0; i--) {
    if (currItem->forward[i] != NULL) {
      if (type == 1) {
        firstOperand = currItem->forward[i]->getInfo()->getDistance();
        secondOperand = info->getDistance();
      } else if (type == 2) {
        firstOperand = currItem->forward[i]->getInfo()->getRating();
        secondOperand = info->getRating();
      } else {
        firstOperand = currItem->forward[i]->getInfo()->getRides();
        secondOperand = info->getRides();
      }
    } else {
      firstOperand = -2;
    }
    while (firstOperand > secondOperand) {
      currItem = currItem->forward[i];
      if (currItem->forward[i] != NULL) {
        if (type == 1) {
          firstOperand = currItem->forward[i]->getInfo()->getDistance();
        } else if (type == 2) {
          firstOperand = currItem->forward[i]->getInfo()->getRating();
        } else {
          firstOperand = currItem->forward[i]->getInfo()->getRides();
        }
      } else {
        firstOperand = -2;
      }
    }
    while (firstOperand == secondOperand &&
           currItem->forward[i]->getInfo()->getName().compare(info->getName()) <
               0) {
      currItem = currItem->forward[i];
      if (currItem->forward[i] != NULL) {
        if (type == 1) {
          firstOperand = currItem->forward[i]->getInfo()->getDistance();
        } else if (type == 2) {
          firstOperand = currItem->forward[i]->getInfo()->getRating();
        } else {
          firstOperand = currItem->forward[i]->getInfo()->getRides();
        }
      } else {
        firstOperand = -2;
      }
    }
    leftItems[i] = currItem;
  }

  if (leftItems[0]->forward[0] == NULL ||
      leftItems[0]->forward[0]->getInfo()->getName().compare(info->getName())) {
    return NULL;
  }
  int i = levels;
  while (leftItems[i]->forward[i] == NULL) {
    i--;
  }
  if (type == 1) {
    firstOperand = leftItems[i]->forward[i]->getInfo()->getDistance();
  } else if (type == 2) {
    firstOperand = leftItems[i]->forward[i]->getInfo()->getRating();
  } else {
    firstOperand = leftItems[i]->forward[i]->getInfo()->getRides();
  }
  while (
      firstOperand != secondOperand &&
      leftItems[i]->forward[i]->getInfo()->getName().compare(info->getName())) {
    i--;
    if (type == 1) {
      firstOperand = leftItems[i]->forward[i]->getInfo()->getDistance();
    } else if (type == 2) {
      firstOperand = leftItems[i]->forward[i]->getInfo()->getRating();
    } else {
      firstOperand = leftItems[i]->forward[i]->getInfo()->getRides();
    }
  }
  Item *curr = leftItems[0]->forward[0];
  while (i >= 0) {
    leftItems[i]->forward[i] = leftItems[i]->forward[i]->forward[i];
    i--;
  }
  while (levels > 0 && firstItem->forward[levels] == NULL) {
    levels--;
  }
  length--;
  delete curr;
  // returnam pozitia de pe care a fost sters soferul
  // pentru a fi introdus ulterior mai rapid
  return leftItems[0];
}
// functii de afisare in functie de tipul structurii
void Ranking::printDistances(std::ofstream &fout, int driversToList) {
  int index = 1;
  Item *current = firstItem;
  while (current->forward[0] != NULL && index <= driversToList) {
    fout << current->forward[0]->getInfo()->getName() << ":";
    fout << current->forward[0]->getInfo()->getDistance() << " ";
    current = current->forward[0];
    index++;
  }
  fout << "\n";
}
void Ranking::printRatings(std::ofstream &fout, int driversToList) {
  int index = 1;
  Item *current = firstItem;
  while (current->forward[0] != NULL && index <= driversToList) {
    fout << current->forward[0]->getInfo()->getName() << ":";
    fout << std::fixed << std::setprecision(3)
         << current->forward[0]->getInfo()->getRating() << " ";
    current = current->forward[0];
    index++;
  }
  fout << "\n";
}
void Ranking::printRides(std::ofstream &fout, int driversToList) {
  int index = 1;
  Item *current = firstItem;
  while (current->forward[0] != NULL && index <= driversToList) {
    fout << current->forward[0]->getInfo()->getName() << ":";
    fout << current->forward[0]->getInfo()->getRides() << " ";
    current = current->forward[0];
    index++;
  }
  fout << "\n";
}
Ranking::~Ranking() {
  Item *item = firstItem;
  Item *aux;
  while (item->forward[0] != NULL) {
    aux = item;
    item = item->forward[0];
    delete aux;
  }
  delete item;
}
Driver *Ranking::getFirstItem() { return firstItem->forward[0]->getInfo(); }
int Ranking::getLength() { return length; }
#endif  //  RANKING_H_
