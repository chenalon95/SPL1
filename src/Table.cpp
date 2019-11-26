
#include "../include/Table.h"
#include <iostream>
#include <string>
#include <vector>
#include "../include/Dish.h"

using namespace std;

Table::Table(int t_capacity) : open(false), capacity(t_capacity), customersList(), orderList() {
}


void Table::openTable() {
    open = true;

}

bool Table::isOpen() {
    return open;
}

int Table::getCapacity() const {
    return capacity;
};

void Table::addCustomer(Customer* customer) {
    //add to customers list
    customersList.push_back(customer->clone());
    //add customers prev orders to \order list (if exist)
    for (int i = 0; i < (int)customer->getOrders().size(); i++)
        setOrders(customer->getOrders().at(i), customer->getId());
    openTable();//in case its not yet open
};


void Table::removeCustomer(int id) {
    //remove customer from list
    int index=-1;
    if ((int)customersList.size() > 0) {
        vector<OrderPair> newList;
        vector<Customer*> newCust;
        for (int i = 0; i < (int)customersList.size(); i++)
            if (customersList.at(i)->getId() != id)
                newCust.push_back(customersList.at(i));
            else
                index=i;

       delete customersList.at(index);
       customersList.clear();


        for (int i = 0; i < (int)newCust.size(); i++)
            customersList.push_back(newCust.at(i));
        for (int i = 0; i < (int)newCust.size(); i++)
            newCust.at(i) = nullptr;
        newCust.clear();
        //remove orders from table's orderlist
        for (int i = 0; i < (int)orderList.size(); i++)
            if (orderList.at(i).first != id)
                newList.push_back(orderList.at(i));

      orderList.clear();
        for (int i = 0; i < (int)newList.size(); i++)
            orderList.push_back(newList.at(i));
        newList.clear();

    }

};
Customer* Table::getCustomer(int id) {
    for (int i = 0; i < (int)customersList.size(); i++) {
        if (customersList.at(i)->getId() == id)
            return customersList.at(i);
    }
    return nullptr;

};

int Table::isExistCustomer(int id) {//returns 0 if customer exist, -1 otherwise
    for (int i = 0; i < (int)customersList.size(); i++)
        if (customersList.at(i)->getId() == id)
            return 0;
    return -1;

};
std::vector<Customer*> & Table::getCustomers() {
    return customersList;
};
std::vector<OrderPair>& Table::getOrders() {
    return orderList;
};
void Table::setOrders(Dish d, int id) {
    OrderPair pair(id, d);
    orderList.push_back(pair);

}


void Table::order(const std::vector<Dish> &menu) {
    vector<int> customerOrder;
    for (int i = 0; i < (int)customersList.size(); i++) {
        customerOrder = customersList.at(i)->order(menu); // get each customer's order
        //add customer i's orders to the orderList
        for (int j = 0; j < (int)customerOrder.size(); j++) {
            for (int h = 0; h < (int)menu.size(); h++)
                if (menu.at(h).getId() == customerOrder.at(j))
                    this->setOrders(menu.at(h), customersList.at(i)->getId());
        }
    }
    customerOrder.clear();

};

void Table::closeTable(){
    clear();
    open=false;
}

int Table::getBill() {
    int bill = 0;
    for (int i = 0; i < (int)orderList.size(); i++) {
        bill = bill + orderList.at(i).second.getPrice();
    }
    return bill;
};



// Rule of five


Table::~Table() { //destructor
    clear();

}
//copy constructor
Table::Table(const Table& other):open(other.open),capacity(other.capacity),customersList(),orderList() {
    copy(other); //deep copy
}

//move constructor
Table::Table(Table && other):open(other.open),capacity(other.capacity),customersList(),orderList() {
    if (this != &other) {
        //shallow copy
        for (vector<Customer*>::iterator i = other.customersList.begin(); i != other.customersList.end(); ++i) {
            customersList.push_back(*i);
        }
        for (vector<OrderPair>::iterator i = other.orderList.begin(); i != other.orderList.end(); ++i)
            orderList.push_back(*i);

        //delete addresses in other's pointers
        for (vector<Customer*>::iterator i = other.customersList.begin(); i != other.customersList.end(); ++i) {
            *i = nullptr;
        }
    }
}

//assig operator
Table& Table::operator=(const Table& other) {
    if (this != &other) {
        clear();//clear this table's prev data
        copy(other);//deap copy
    }
    return *this;
}

//move assig operator
Table& Table::operator=(Table&& other) {
    if (this != &other) {
        clear();//clear this table's prev data
        //shallow copy
        capacity = other.getCapacity();
        open = other.open;
        for (vector<Customer*>::iterator i = other.customersList.begin(); i != other.customersList.end(); ++i) {
            customersList.push_back(*i);
        }
        for (vector<OrderPair>::iterator i = other.orderList.begin(); i != other.orderList.end(); ++i)
            orderList.push_back(*i);
        //delete other's pointers' addresses
        for (vector<Customer*>::iterator i = other.getCustomers().begin(); i != other.getCustomers().end(); ++i) {
            *i = nullptr;
        }
    }
    return *this;

}

//sub function to help rul of five
void Table::clear() {
    for (vector<Customer*>::iterator i = customersList.begin(); i != customersList.end(); ++i) {
        delete *i;
    }

    customersList.clear();
    orderList.clear();

}


//deap copy
void Table::copy(const Table& other) {
    capacity = other.getCapacity();
    open = other.open;
    for (vector<OrderPair>::const_iterator i = other.orderList.begin(); i != other.orderList.end(); ++i)
        orderList.push_back((*i));
    for (int i = 0; i < (int)other.customersList.size(); i++)
        customersList.push_back(other.customersList.at(i)->clone());
};

