#include "Player.h"

Player::Player( string name, string color ) {
  this->name = name;
  this->color = color;
}

Player::~Player() {}

string Player::getName() {
  return this->name;
}

string Player::getColor() {
  return this->color;
}
