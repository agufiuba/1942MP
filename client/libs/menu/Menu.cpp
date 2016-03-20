#ifndef MENU_H
#include "Menu.h"
#endif
#include <iostream>

using namespace std;

Menu::Menu(string title) {
  this->title = title;
  this->optionNumber = 0;
}

void Menu::addOption(string optionName, string action) {
 this->optionNumber++;
 this->options.insert(pair<int, string>(this->optionNumber, optionName)); 
 this->actions.insert(pair<int, string>(this->optionNumber, action));
}

void Menu::removeOption(string optionName) {
  
}

void Menu::display() {
  cout << "----\t" << this->title << "\t----" << endl;
  for(map<int, string>::iterator it = this->options.begin(); it!=this->options.end(); it++) {
    cout << it->first << ". " << it->second << " with action: " << this->actions[it->first] << endl;
  }

  string input = "";
  
  cout << endl << "Ingrese opcion: " << endl;
  cin >> input;
  
}
