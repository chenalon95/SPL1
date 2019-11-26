

#include "../include/Action.h"
#include <iostream>
#include "../include/Restaurant.h"
#include <string>
#include <vector>
#include "../include/Dish.h"


extern Restaurant* backup;
using namespace std;

//check
BaseAction::BaseAction() :errorMsg(""),status(PENDING) {

}

//getters&setters
ActionStatus BaseAction::getStatus() const {
    return status;
};
void BaseAction::complete() {
    status = COMPLETED;
};
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " << this->errorMsg << endl;
};
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
};

ActionStatus BaseAction::getStatus() {
    return status;
}

BaseAction::~BaseAction() {

}




//open Table
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) :tableId(id), customers(customersList), toStr("") {
};
void OpenTable::act(Restaurant &restaurant) {
    toStr = "open " + to_string(tableId) + ' ';//creating string for logger
    for (int i = 0; i < (int)customers.size(); i++)
        toStr += customers.at(i)->getName() + "," + customers.at(i)->customerType() + ' ';
    if ((bool)(restaurant.getTable(tableId)->isOpen())) {
        toStr += "Error: Table does not exist or is already open\n";
        error("Table does not exist or is already open");
    }
    else {
        for (const auto& item : customers)// adding customers
            restaurant.getTable(tableId)->addCustomer(item);
        if ((int)customers.size()==0)
            restaurant.getTable(tableId)->openTable();
        complete();


        toStr += "Completed\n";
    }
    restaurant.setActionsLog(this);


};
string OpenTable::toString() const {
    return toStr;
};

OpenTable* OpenTable::clone() {
    return new OpenTable(*this);
};

Customer* OpenTable::getCusto(int i) const {
    return customers.at(i);
}

//rule of 5



//destructor
OpenTable:: ~OpenTable() {
    for (vector<Customer*>::iterator i = customers.begin(); i != customers.end(); ++i) {
        delete *i;
    }
    customers.clear();

}

//copy constructor
OpenTable::OpenTable(const  OpenTable& other) : tableId(other.tableId),customers(), toStr(other.toStr) {
    for (int i = 0; i < (int)other.customers.size(); i++)
        this->customers.push_back(other.getCusto(i)->clone());
}


//move constructor
OpenTable::OpenTable(OpenTable && other) : tableId(other.tableId),customers(), toStr(other.toStr) {
    for (int i = 0; i < (int)other.customers.size(); i++) {
        customers.push_back(other.getCusto(i)); //shallow copy
        other.customers.at(i) = nullptr;
    }
    other.customers.clear();
}


//Order
Order::Order(int id) : tableId(id), toStr("") {
};
void Order::act(Restaurant &restaurant) {
    string toPrint;
    if (restaurant.getTable(tableId) == nullptr || !(restaurant.getTable(tableId)->isOpen())) {
        error("Table does not exist or is not open");
        toStr="order "+to_string(tableId) +" Error: Table does not exist or is not open\n";
    }
    else {

        restaurant.getTable(tableId)->order(restaurant.getMenu());
        for (std::vector<Customer*>::iterator it = restaurant.getTable(tableId)->getCustomers().begin(); it != restaurant.getTable(tableId)->getCustomers().end(); ++it) {
            toPrint += (*it)->toString();
        }
        toStr = "order " + to_string(tableId) + " Completed\n";
        complete();
    }
    restaurant.setActionsLog(this);
    cout << toPrint;
};
string Order::toString() const {
    return toStr;

};

Order* Order::clone() {
    return new Order(*this);
};

Order::~Order(){};


//MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) :  srcTable(src), dstTable(dst), id(customerId), toStr("") {
};

void MoveCustomer::act(Restaurant &restaurant) {
    if (restaurant.getTable(srcTable) == nullptr || restaurant.getTable(dstTable) == nullptr || !restaurant.getTable(dstTable)->isOpen() ||
            (int)restaurant.getTable(dstTable)->getCapacity() < (int)restaurant.getTable(dstTable)->getCustomers().size() + 1 ||
        restaurant.getTable(srcTable)->isExistCustomer(id) == -1) {
        error("Cannot move customer");
        toStr = "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " Error: Cannot move customer\n";
    }
    else {
        //adding customer to destination table
        restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));

        //removing customer from source table
        restaurant.getTable(srcTable)->removeCustomer(id);
        if (restaurant.getTable(srcTable)->getCustomers().size() == 0) { // in case of empty table
            restaurant.getTable(srcTable)->closeTable();
        }
        toStr = "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " Completed\n";
        complete();
    }
    restaurant.setActionsLog(this);
};

MoveCustomer::~MoveCustomer() {};


string MoveCustomer::toString() const {
    return toStr;
};

MoveCustomer* MoveCustomer::clone() {
    return new MoveCustomer(*this);
};

//Close
Close::Close(int id) : tableId(id), tableBill(0) ,toStr(""){

};

void Close::act(Restaurant &restaurant) {

    if (!restaurant.getTable(tableId)->isOpen()) { // if the table is empty
        error("Table does not exist or is not open");
        toStr="close "+to_string(tableId)+" Error: Table does not exist or is not open\n";
    }
    else {
        tableBill = restaurant.getTable(tableId)->getBill();
        restaurant.getTable(tableId)->closeTable();
        cout << "Table " + to_string(tableId) + " was closed. Bill " + to_string(tableBill) + "NIS" << endl;
        toStr="close "+to_string(tableId)+" Completed\n";
        complete();
    }
    restaurant.setActionsLog(this);
};

int Close::getBill() const {
    return tableBill;
}

string Close::toString() const {
    return toStr;
};

Close* Close::clone() {
    return new Close(*this);
};

Close::~Close(){};

//CloseAll
CloseAll::CloseAll() :toStr("") {

};
void CloseAll::act(Restaurant &restaurant) {
    string str = "";
    for (int i = 0; i < (int)restaurant.getTables().size(); i++)
        if (restaurant.getTable(i)->isOpen()) {
                Close close(i);
                close.act(restaurant);
                str = str + close.toString();

        }
    toStr = str;

};

string CloseAll::toString() const {
    return toStr;
};

CloseAll* CloseAll::clone() {
    return new CloseAll(*this);
};

CloseAll::~CloseAll(){};


//PrintMenu
PrintMenu::PrintMenu() {

};
void PrintMenu::act(Restaurant &restaurant) {
    string dishType="",toString="";
    for (vector<Dish>::iterator i = restaurant.getMenu().begin(); i != restaurant.getMenu().end(); ++i) {
        if ((*i).getType() == VEG)
            dishType = "VEG";
        else if ((*i).getType() == ALC)
            dishType = "ALC";
        else if ((*i).getType() == SPC)
            dishType = "SPC";
        else if ((*i).getType() == BVG)
            dishType = "BVG";
        toString += (*i).getName() + " " + dishType + " " + to_string((*i).getPrice()) + "NIS\n";
    }
    cout << toString;
    restaurant.setActionsLog(this);
};
string PrintMenu::toString() const {
    return "menu Completed\n";
};

PrintMenu* PrintMenu::clone() {
    return new PrintMenu(*this);
};

PrintMenu::~PrintMenu(){};

// PrintTableStatus

PrintTableStatus::PrintTableStatus(int id) : tableId(id), toStr("") {

};
void PrintTableStatus::act(Restaurant &restaurant) {
    toStr="";
    string toPrint;
    if (restaurant.getTable(tableId)->isOpen()) {
        toPrint = "Table " + to_string(tableId) + " status: open\n";
        toPrint += "Customers:\n";
        for (vector<Customer*>::iterator i = restaurant.getTable(tableId)->getCustomers().begin(); i != restaurant.getTable(tableId)->getCustomers().end(); ++i) {
            toPrint += to_string((*i)->getId()) + " " + (*i)->getName() + "\n";
        }
        toPrint += "Orders:\n";
        for (vector<OrderPair>::iterator i = restaurant.getTable(tableId)->getOrders().begin(); i != restaurant.getTable(tableId)->getOrders().end(); ++i) {
            toPrint += (*i).second.getName() + " " + to_string((*i).second.getPrice()) + "NIS " + to_string((*i).first) + "\n";
        }
        toPrint += "Current Bill: " + to_string(restaurant.getTable(tableId)->getBill()) + "NIS\n";

    }
    else {
        toPrint = "Table " + to_string(tableId) + " status: closed\n";
    }
    cout << toPrint;
    toStr += "status " + to_string(tableId) + " Completed\n";
    restaurant.setActionsLog(this);


};
string PrintTableStatus::toString() const {
    return toStr;
};

PrintTableStatus* PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
};

PrintTableStatus::~PrintTableStatus(){};


//PrintActionsLog
PrintActionsLog::PrintActionsLog() :toStr("") {

};
void PrintActionsLog::act(Restaurant &restaurant) {
    string toString="";
    for (int i = 0; i < (int)restaurant.getActionsLogSize(); i++)
        toString += restaurant.getActionsLog().at(i)->toString();
    cout << toString;
    toStr="log Completed\n";
    restaurant.setActionsLog(this);
};


string PrintActionsLog::toString() const {
    return toStr;
};

PrintActionsLog* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
};

PrintActionsLog::~PrintActionsLog(){};


// BackupRestaurant
BackupRestaurant::BackupRestaurant() :toStr("") {
};
void BackupRestaurant::act(Restaurant &restaurant) {
    if (restaurant.getFirstBackup()) {
        restaurant.setFirstBackup(false);
        backup = new Restaurant(restaurant);
    }
    else {
        *backup = restaurant;

    }
    toStr = "backup Completed\n";
    restaurant.setActionsLog(this);
};
string BackupRestaurant::toString() const {
    return toStr;
};

BackupRestaurant* BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
};

BackupRestaurant::~BackupRestaurant(){};


//RestoreResturant
RestoreResturant::RestoreResturant():toStr(""){

};
void RestoreResturant::act(Restaurant &restaurant) {
    if (restaurant.getFirstBackup()) {
        error("No backup available");
        toStr="restore Error: No backup available\n";
    }
    else {
        restaurant = *backup;
        toStr = "restore Completed\n";
    }
    restaurant.setActionsLog(this);

};
string RestoreResturant::toString() const {
    return toStr;
};

RestoreResturant* RestoreResturant::clone() {
    return new RestoreResturant(*this);
};

RestoreResturant::~RestoreResturant(){};
