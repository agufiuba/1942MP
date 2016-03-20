#ifndef MENU_H
#define MENU_H
#include <string>
#include <map>

class Menu {
  private:
    std::string title;
    std::map<int, std::string> options;
    std::map<int, std::string> actions;
    int optionNumber;

  public:
    Menu(std::string title);
    void addOption(std::string optionName, std::string action); 
    void removeOption(std::string optionName);
    void display();
};

#endif
