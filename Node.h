#ifndef NODE_H_
#define NODE_H_
#include <list>
#include "./driver.h"
#include "./ranking.h"
// Copyright 2019 Samoilescu Sebastian
// clasa in care se retine informatiile despre o intersectie
template <typename keyType, typename itemType> class Node {
 private:
  keyType key;
  // id unei intersectii este folosit in parcurgere pentru ca
  // intersectiile sa fie usor marcate drept vizitate
  itemType id;

 public:
  // retine soferii aflati in intersectie
  Ranking drivers = Ranking(3, 0.5, 2);
  // retine muchiile exterioare
  std::list<Node<keyType, itemType> *> neigh_out;
  // retine muchiile interioare
  std::list<Node<keyType, itemType> *> neigh_in;
  Node(keyType, itemType);
  void addDriver(Driver *);
  void deleteDriver(Driver *);
  void setKey(keyType);
  void setId(itemType);
  void listDrivers(std::ofstream &);
  int getSize();
  // intrucat retinem soferii in skipList dupa rating
  // functia returneaza primul sofer din lista,adica pe cel cu rating mai mare
  Driver *getBestDriver();
  keyType getKey();
  itemType getId();
  ~Node();
};
template <typename keyType, typename itemType>
Node<keyType, itemType>::Node(keyType nKey, itemType nId) {
  key = nKey;
  id = nId;
}
template <typename keyType, typename itemType>
void Node<keyType, itemType>::setKey(keyType nKey) {
  key = nKey;
}
template <typename keyType, typename itemType>
void Node<keyType, itemType>::setId(itemType nId) {
  id = nId;
}
template <typename keyType, typename itemType>
keyType Node<keyType, itemType>::getKey() {
  return key;
}
template <typename keyType, typename itemType>
itemType Node<keyType, itemType>::getId() {
  return id;
}
template <typename keyType, typename itemType>
Node<keyType, itemType>::~Node() {
  neigh_out.clear();
  neigh_in.clear();
}
template <typename keyType, typename itemType>
void Node<keyType, itemType>::addDriver(Driver *driver) {
  drivers.addItem(driver);
}
template <typename keyType, typename itemType>
void Node<keyType, itemType>::deleteDriver(Driver *driver) {
  drivers.deleteItem(driver);
}
template <typename keyType, typename itemType>
int Node<keyType, itemType>::getSize() {
  return drivers.getLength();
}
template <typename keyType, typename itemType>
Driver *Node<keyType, itemType>::getBestDriver() {
  return drivers.getFirstItem();
}

#endif  //  NODE_H_
