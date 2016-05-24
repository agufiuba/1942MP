#include "Player.h"

Player::Player( string name, string color, int x, int y ) {
  this->name = name;
  this->color = color;
  this->x = x;
  this->y = y;
  this->active = false;
}

Player::~Player() {}

string Player::getName() {
  return this->name;
}

string Player::getColor() {
  return this->color;
}

int Player::getX() {
  return this->x;
}

int Player::getY() {
  return this->y;
}

void Player::setX(int x){
	this->x = x;
}

void Player::setY(int y){
	this->y = y;
}

bool Player::isActive() {
  return this->active;
}

void Player::activate() {
  this->active = true;
}

void Player::deactivate() {
  this->active = false;
}
