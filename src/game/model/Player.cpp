#include "Player.h"

Player::Player( string name, string color, int x, int y, int team, int health ) {
  this->name = name;
  this->color = color;
  this->x = x;
  this->y = y;
  this->active = false;
  this->health = health;
  this->score = 0;
  this->team = team;
}

Player::~Player() {}

string Player::getName() {
  return this->name;
}

string Player::getColor() {
  return this->color;
}

int Player::getScore() {
  return this->score;
}

void Player::addScore( int score ) {
  this->score += score;
}

void Player::resetScore() {
  this->score = 0;
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

int Player::getHealth() {
  return this->health;
}

void Player::takeHit() {
  if ( this->health > 0 )
    this->health--;
}

bool Player::isAlive() {
  return this->health > 0;
}

void Player::die() {
  this->health = 0;
}

int Player::getTeam(){
	return this->team;
}
