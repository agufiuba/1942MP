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
    std::map<int, std::map<std::string, std::function<void(std::string)>> > iactions;
    int optionNumber;
    bool validInput(std::string input);
    int requireOption();
    void triggerOption(int optionNumber);

  public:
    Menu(std::string title);
    void addOption(std::string optionName, std::function<void()>);
    void addOption(std::string optionName, std::function<void(std::string)>, std::string);
    void removeOption(std::string optionName);
    void display();
};

#endif
