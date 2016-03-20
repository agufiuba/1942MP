#ifndef MENU_H
#define MENU_H
#include <string>
#include <map>
#include <functional>

class Menu {
  private:
    std::string title;
    std::map<int, std::string> options;
    std::map<int, std::function<void()>> actions;
    int optionNumber;
    bool validInput(std::string input);
    int requireOption();
    void triggerOption(int input);

  public:
    Menu(std::string title);
    void addOption(std::string optionName, std::function<void()>); 
    void removeOption(std::string optionName);
    void display();
};

#endif
