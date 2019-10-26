#ifndef DRIVER_H_
#define DRIVER_H_
#include <string>
#include <fstream>
#include <iostream>
// Copyright 2019 Samoilescu Sebastian

class Driver {
 private:
  int id, ridesFinished, distance, points;
  float rating;
  bool status;
  std::string name, position;

 public:
  Driver(int, std::string);
  Driver();
  Driver(const Driver &);
  Driver &operator=(const Driver &);
  std::string getName();
  void setName(std::string);
  void setStatus(bool status);
  void setId(int);
  void addDistance(int);
  void addRating(int);
  void setPosition(std::string);
  void printInfos(std::ofstream &);
  int getId();
  bool getStatus();
  int getDistance();
  float getRating();
  int getRides();
  std::string getPosition();
};
Driver::Driver(int id, std::string name) {
  this->id = id;
  this->name = name;
  distance = 0;
  points = 0;
  rating = 0;
  ridesFinished = 0;
  position = "NULL";
  status = true;
}
Driver::Driver() {
  distance = 0;
  rating = 0;
  points = 0;
  ridesFinished = 0;
  position = "NULL";
  status = true;
}
Driver::Driver(const Driver &other) {
  distance = other.distance;
  points = other.points;
  rating = other.rating;
  ridesFinished = other.ridesFinished;
  position = other.position;
  status = other.status;
  name = other.name;
  id = other.id;
}
Driver &Driver::operator=(const Driver &other) {
  distance = other.distance;
  points = other.points;
  rating = other.rating;
  ridesFinished = other.ridesFinished;
  position = other.position;
  status = other.status;
  name = other.name;
  id = other.id;
  return *this;
}
std::string Driver::getName() { return name; }
void Driver::setName(std::string name) { this->name = name; }
void Driver::setId(int id) { this->id = id; }
int Driver::getId() { return id; }
int Driver::getDistance() { return distance; }
float Driver::getRating() { return rating; }
std::string Driver::getPosition() { return position; }
bool Driver::getStatus() { return status; }
void Driver::addDistance(int dist) { distance += dist; }
void Driver::addRating(int rate) {
  points += rate;
  ridesFinished++;
  rating = (float) points / ridesFinished;
}
void Driver::setPosition(std::string pos) { position = pos; }
void Driver::setStatus(bool status) { this->status = status; }
int Driver::getRides() { return ridesFinished; }
void Driver::printInfos(std::ofstream &fout) {
  fout << name << ": " << position << " " << rating << " " << ridesFinished
       << " " << distance << " ";
  if (status) {
    fout << "online\n";
  } else {
    fout << "offline\n";
  }
}
#endif  //  DRIVER_H_
