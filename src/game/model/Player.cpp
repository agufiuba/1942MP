#include "Player.h"

Player::Player( string name, string color ) {
  this->name = name;
  this->color = color;
  this->active = true;
}

Player::~Player() {}

string Player::getName() {
  return this->name;
}

string Player::getColor() {
  return this->color;
}

bool Player::isActive() {
  return this->active;
}

void Player::deactivate() {
  this->active = false;
}
