
#pragma once
#ifndef UNTITLED_TABLE_H
#define UNTITLED_TABLE_H



#include <string>
#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table {
public:
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void setOrders(Dish d, int id);//adds an order
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    int isExistCustomer(int id);//returns 0 if customer exist, 1 otherwise
    ~Table(); //destructor
    Table& operator=(const Table& other);//assignment operator
    Table(const Table& other);//copy constructor
    Table(Table && other);//move copy constructor
    Table& operator=(Table&& other);//move assignment operator


private:
    void clear();//sub function for ass operator and destructor
    void copy(const Table& other);//sub function for ass operator and constructor
    bool open;
    int capacity;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif //UNTITLED_TABLE_H