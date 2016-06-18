#include "Player.h"

using namespace std;

Player::Player( string name, string color, int x, int y, int team, int health ) {
	cout << "CRENADO PLAYER 1 " << endl;
  this->name = name;
  this->color = color;
  this->x = x;
  this->y = y;
  this->xEstacionamiento = x;
  this->yEstacionamiento = y;
  this->active = false;
  this->health = health;
  this->score = 0;
  this->team = team;
  this->ready = false;
}

Player::Player( string name, string color, int x, int y, int xEst, int yEst, int team, int health ) {
	cout << "CRENADO PLAYER 2 " << endl;
  this->name = name;
  this->color = color;
  this->x = x;
  this->y = y;
  this->xEstacionamiento = xEst;
  this->yEstacionamiento = yEst;
  this->active = false;
  this->health = health;
  this->score = 0;
  this->team = team;
  this->ready = false;
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

int Player::getEstacionamientoX() {
	return this->xEstacionamiento;
}

int Player::getEstacionamientoY() {
	return this->yEstacionamiento;
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

void Player::changeReady(){
	this->ready = !this->ready;
}

bool Player::isReady(){
	return this->ready;
}

void Player::setTeam(int team){
	this->team = team;
}
