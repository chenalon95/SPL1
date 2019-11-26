


#pragma once
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant {
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*> &getActionsLog() const; //Return a reference to the history of actions
    void setActionsLog(BaseAction *action);
    std::vector<Dish>& getMenu();
    int getNumOfCus() const;
    std::vector<Table*> getTables() const;
    void setFirstBackup(bool b);
    bool getFirstBackup() const;
    int getActionsLogSize() const;
    int getNumOfOpenTab();
    virtual ~Restaurant();//destructor
    Restaurant(const Restaurant& other);//copy constructor
    Restaurant(Restaurant&& other);//move constructor
    Restaurant& operator=(const Restaurant& other);//copy assignment
    Restaurant& operator=(Restaurant&& other);//move assignment


private:
    void getAction(string line);
    void moveAct(string line);
    void openAct(string line);
    void initTables(std::string str, int numOfTables);
    void init(const std::string &configFilePath);
    void sortByPrice();
    void copy(const Restaurant& other);
    void clear();
    void shallowCopy(const Restaurant& other);


    int numOfCus;//for IDs
    bool open;
    bool isFirstBackup;
    std::vector<Table*> tables;
    std::vector<BaseAction*> actionsLog;
    std::vector<Dish> menu;
};


#endif