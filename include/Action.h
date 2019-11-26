
#pragma once
#ifndef UNTITLED_ACTION_H
#define UNTITLED_ACTION_H

#include <string>
#include <iostream>
#include "Customer.h"
#include <vector>

using namespace std;


enum ActionStatus {
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction {
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant) = 0;
    virtual std::string toString() const = 0;
    virtual BaseAction* clone() = 0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    ActionStatus getStatus();
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    OpenTable* clone();
    Customer* getCusto(int i) const;
    ~OpenTable(); //destructor
    OpenTable(const  OpenTable& other);// copy constructor
    OpenTable(OpenTable && other);//move copy constructor

private:
    const int tableId;
    std::vector<Customer *> customers;
    string toStr;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~Order();
    Order* clone();
private:
    const int tableId;
    string toStr;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~MoveCustomer();
    MoveCustomer* clone();
private:
    const int srcTable;
    const int dstTable;
    const int id;
    string toStr;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    int getBill() const;
    ~Close();
    Close* clone();
private:
    const int tableId;
    int tableBill;
    string toStr;

};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~CloseAll();
    CloseAll* clone();
private:
    string toStr;
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~PrintMenu();
    PrintMenu* clone();
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~PrintTableStatus();
    PrintTableStatus* clone();
private:
    const int tableId;
    string toStr;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~PrintActionsLog();
    PrintActionsLog* clone();
private:
    string toStr;

};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~BackupRestaurant();
    BackupRestaurant* clone();
private:
    string toStr;
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    ~RestoreResturant();
    RestoreResturant* clone();
private:
    string toStr;
};


#endif //UNTITLED_ACTION_H