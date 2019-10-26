#ifndef MAP_H_
#define MAP_H_
#include "Node.h"
#include <iostream>
#include <list>
#include <queue>
#include <vector>
// Copyright 2019 Samoilescu Sebastian

// structura in care salvam harta
template <typename keyType, typename itemType> class Map {
 private:
    int maxHeight;
    int (*hash)(keyType, int);

 public:
std::list<Node<keyType, itemType>> *graph;
Map(int, int (*hash)(keyType, int));
Map(const Map &other);
Map(){}
Map<keyType, itemType> operator=(const Map &other);
void resize(int);
int isEqual(keyType, std::vector<keyType> &);
void getCosts(keyType, std::vector<keyType> &);
int distance(keyType, std::vector<keyType> &);
void addVertex(keyType, itemType);
void addEdge(keyType, keyType);
void listMap();
int getCollisions();
void deleteWay(keyType, keyType);
void deleteWays(keyType, keyType);
void biDirectional(keyType, keyType);
bool isWay(keyType, keyType);
void reverseWay(keyType, keyType);
Node<keyType, itemType> *getItem(keyType);
~Map();
int getHeight();
};
template <typename keyType, typename itemType>
Map<keyType, itemType>::Map(int height, int (*hash)(keyType, int)) {
  maxHeight = height;
  this->hash = hash;
  graph = new std::list<Node<keyType, itemType>>[maxHeight];
}
template <typename keyType, typename itemType>
bool Map<keyType, itemType>::isWay(keyType source, keyType dest) {
  Node<keyType, itemType> *node = getItem(source);
  typename std::list<Node<keyType, itemType> *>::iterator it =
      node->neigh_out.begin();
  for (; it != node->neigh_out.end(); ++it) {
    if ((*it)->getKey() == dest) {
      return true;
    }
  }
  return false;
}
template <typename keyType, typename itemType>
Map<keyType, itemType>::Map(const Map &other) {
  maxHeight = other.maxHeight;
  this->hash = other.hash;
  graph = new std::list<Node<keyType, itemType>>[maxHeight];
  for (int i = 0; i < maxHeight; i++) {
    typename std::list<Node<keyType, itemType>>::iterator it =
        other.graph[i].begin();
    for (; it != other.graph[i].end(); it++) {
      Node<keyType, itemType> node(it->getKey(), it->getId());
      typename std::list<Node<keyType, itemType> *>::iterator jt =
          it->neigh_out.begin();
      for (; jt != it->neigh_out.end(); jt++) {
        node.neigh_out.push_back((*jt));
      }
      jt = it->neigh_in.begin();
      for (; jt != it->neigh_in.end(); jt++) {
        node.neigh_in.push_back((*jt));
      }
      graph[i].push_back(node);
    }
  }
}
template <typename keyType, typename itemType>
Map<keyType, itemType> Map<keyType, itemType>::
operator=(const Map &other) {
  maxHeight = other.maxHeight;
  this->hash = other.hash;
  graph = new std::list<Node<keyType, itemType>>[maxHeight];
  for (int i = 0; i < maxHeight; i++) {
    typename std::list<Node<keyType, itemType>>::iterator it =
        other.graph[i].begin();
    for (; it != other.graph[i].end(); it++) {
      Node<keyType, itemType> node(it->getKey(), it->getId());
      typename std::list<Node<keyType, itemType> *>::iterator jt =
          it->neigh_out.begin();
      for (; jt != it->neigh_out.end(); jt++) {
        node.neigh_out.push_back((*jt));
      }
      jt = it->neigh_in.begin();
      for (; jt != it->neigh_in.end(); jt++) {
        node.neigh_in.push_back((*jt));
      }
      graph[i].push_back(node);
    }
  }
  return *this;
}
// returnare referinta intersectie
template <typename keyType, typename itemType>
Node<keyType, itemType> *Map<keyType, itemType>::getItem(keyType source) {
  int index = hash(source, maxHeight);
  typename std::list<Node<keyType, itemType>>::iterator it =
      graph[index].begin();
  for (; it != graph[index].end(); ++it) {
    if (it->getKey() == source) {
      return &(*it);
    }
  }
  return NULL;
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::resize(int height) {
  maxHeight = height;
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::addVertex(keyType key, itemType id) {
  int index = hash(key, maxHeight);
  Node<keyType, itemType> newNode(key, id);
  graph[index].push_back(newNode);
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::addEdge(keyType source, keyType dest) {
  if (isWay(source, dest)) {
    return;
  }
  Node<keyType, itemType> *sourceNode = getItem(source);
  int indexDest = hash(dest, maxHeight);
  typename std::list<Node<keyType, itemType>>::iterator jt =
      graph[indexDest].begin();
  for (; jt != graph[indexDest].end(); ++jt) {
    if (jt->getKey() == dest) {
      // adaugare muchie exteriora la intersectia de plecare
      sourceNode->neigh_out.push_back(&(*jt));
      // adaugarea muchie interioara la intersectia destinatie
      jt->neigh_in.push_back(sourceNode);
      return;
    }
  }
}
template <typename keyType, typename itemType>
Map<keyType, itemType>::~Map() {
  for (int i = 0; i < maxHeight; i++) {
    while (!graph[i].empty()) {
      graph[i].pop_front();
    }
  }
  delete[] graph;
}
template <typename keyType, typename itemType>
int Map<keyType, itemType>::getCollisions() {
  int collisions = 0;
  for (int i = 0; i < maxHeight; i++) {
    if (graph[i].size() > 1) {
      collisions++;
    }
  }
  return collisions;
}

template <typename keyType, typename itemType>
int Map<keyType, itemType>::distance(keyType source,
                                       std::vector<keyType> &dest) {
  int distanceToNeigh = -1;
  // in dest[0] salvam destinatia propriu-zisa, iar in restul componentelor
  // salvam vecinii destinatiei
  // Vom marca in numele intersectiei cu 1 - atingerea destinatiei,
  // iar cu 2 atingerea veciniilor destinatiei
  // Urmeaza DFS putin modificat
  if (source == dest[0]) {
    dest[0] = dest[0] + "1";
    return 0;
  } else {
    for (unsigned int i = 1; i < dest.size(); i++) {
      if (source == dest[i]) {
        distanceToNeigh = 0;
        dest[i] = dest[i] + "2";
        break;
      }
    }
  }
  std::vector<int> visited(maxHeight, 0);
  std::queue<Node<keyType, itemType> *> queue;
  Node<keyType, itemType> *node = getItem(source);
  queue.push(node);
  visited[node->getId()] = 1;
  while (!queue.empty()) {
    Node<keyType, itemType> *currNode = queue.front();
    queue.pop();
    typename std::list<Node<keyType, itemType> *>::iterator it =
        currNode->neigh_out.begin();
    for (; it != currNode->neigh_out.end(); ++it) {
      if (visited[(*it)->getId()] == 0) {
        if ((*it)->getKey() == dest[0]) {
          dest[0] = dest[0] + "1";
          return visited[currNode->getId()];
        } else if (distanceToNeigh == -1) {
          for (unsigned int i = 1; i < dest.size(); i++) {
              if ((*it)->getKey() == dest[i]) {
                distanceToNeigh = visited[currNode->getId()];
                dest[i] = dest[i] + "2";
                break;
              }
            }
        }
        visited[(*it)->getId()] = visited[currNode->getId()] + 1;
        queue.push(*it);
      }
    }
  }
  return distanceToNeigh;
}
template <typename keyType, typename itemType>
int Map<keyType, itemType>::isEqual(keyType source,
                                      std::vector<keyType> &dest) {
  for (unsigned int i = 0; i < dest.size(); i++) {
    if (source == dest[i]) {
      return i;
    }
  }
  return -1;
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::getCosts(keyType source,
                                        std::vector<keyType> &dest) {
  // introducem la inceputul numelelor intersectiilor destinatie
  // distanta dintre locul de plecare si ele
  int index = isEqual(source, dest);
  if (index != -1) {
    dest[index] = "0" + dest[index];
  }
  std::vector<int> visited(maxHeight, 0);
  std::queue<Node<keyType, itemType> *> queue;
  Node<keyType, itemType> *node = getItem(source);
  queue.push(node);
  visited[node->getId()] = 1;
  while (!queue.empty()) {
    Node<keyType, itemType> *currNode = queue.front();
    queue.pop();
    typename std::list<Node<keyType, itemType> *>::iterator it =
        currNode->neigh_out.begin();
    for (; it != currNode->neigh_out.end(); ++it) {
      if (visited[(*it)->getId()] == 0) {
        index = isEqual((*it)->getKey(), dest);
        if (index != -1) {
          dest[index] =
              std::to_string(visited[currNode->getId()]) + dest[index];
        }
        visited[(*it)->getId()] = visited[currNode->getId()] + 1;
        queue.push(*it);
      }
    }
  }
  // celor ce nu le-a fost atribuita nici-o distanta le vom atribui -1
  for (unsigned int i = 0; i < dest.size(); i++) {
    if (!(dest[i][0] >= '0' && dest[i][0] <= '9')) {
      dest[i] = "-1" + dest[i];
    }
  }
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::deleteWay(keyType source, keyType dest) {
  Node<keyType, itemType> *node = getItem(source);
  typename std::list<Node<keyType, itemType> *>::iterator it =
      node->neigh_out.begin();
  for (; it != node->neigh_out.end(); ++it) {
    if ((*it)->getKey() == dest) {
      node->neigh_out.erase(it);
      break;
    }
  }
  node = getItem(dest);
  it = node->neigh_in.begin();
  for (; it != node->neigh_in.end(); ++it) {
    if ((*it)->getKey() == source) {
      node->neigh_in.erase(it);
      break;
    }
  }
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::deleteWays(keyType source, keyType dest) {
  deleteWay(source, dest);
  deleteWay(dest, source);
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::biDirectional(keyType source, keyType dest) {
  addEdge(dest, source);
  addEdge(source, dest);
}
template <typename keyType, typename itemType>
void Map<keyType, itemType>::reverseWay(keyType source, keyType dest) {
  if (isWay(source, dest) && isWay(dest, source)) {
    return;
  }
  if (isWay(source, dest)) {
    deleteWay(source, dest);
    addEdge(dest, source);
  } else if (isWay(dest, source)) {
    deleteWay(dest, source);
    addEdge(source, dest);
  }
}
template <typename keyType, typename itemType>
int Map<keyType, itemType>::getHeight() {
  return maxHeight;
}
#endif  //  MAP_H_
