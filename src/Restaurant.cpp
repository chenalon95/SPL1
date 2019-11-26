

#include "../include/Restaurant.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/Dish.h"



using namespace std;

Restaurant::Restaurant() : numOfCus(0), open(false), isFirstBackup(true), tables(),actionsLog(),menu() {

};
Restaurant::Restaurant(const std::string &configFilePath) : numOfCus(0), open(false) ,isFirstBackup(true),  tables(),actionsLog(),menu() {
    init(configFilePath);
};


//initialization of tables
void Restaurant::initTables(std::string str, int numOfTables) {
    int lastComma = -1;
    int thisComma = str.find(',');
    for (int i = 0; i < numOfTables; i++) {
        int capacity = stoi(str.substr(lastComma + 1, thisComma - lastComma - 1));
        tables.push_back(new Table(capacity));
        lastComma = thisComma;
        thisComma = str.find(',', lastComma + 1);
    }

};

//initialization
void Restaurant::init(const std::string &configFilePath) {
    numOfCus = 0;
    string line = "";
    if (configFilePath.length() > 1) {
        std::ifstream myfile(configFilePath);
        if (myfile.is_open()) {
            getline(myfile, line);
            while (line == "" || line.at(0) == '#') {
                getline(myfile, line);
            }
            int numOfTables = stoi(line);
            getline(myfile, line);
            while (line == "" || line.at(0) == '#') {
                getline(myfile, line);
            }
            string temp = line;
            temp += ",";

            initTables(temp, numOfTables);

            getline(myfile, line);

            while (line == "" || line.at(0) == '#') {
                getline(myfile, line);

            }


            //init menu
            int dishIndex = 0;//dish ID
            while (myfile.is_open() && (int)line.size()>0 && line.at((int)line.size() - 1) != ',') {
                if (line != "") {
                    line += ",";
                    int index = line.find(',');
                    string dishName = line.substr(0, index);
                    string dishType = line.substr(index + 1, 3);
                    DishType dish; // convert string to enum
                    if (dishType.compare("VEG") == 0)
                        dish = VEG;
                    else if (dishType.compare("SPC") == 0)
                        dish = SPC;
                    else if (dishType.compare("ALC") == 0)
                        dish = ALC;
                    else if (dishType.compare("BVG") == 0)
                        dish = BVG;

                    index += 4;
                    index = line.find(',', index);
                    int dishPrice = stoi(line.substr(index + 1, line.length() - index - 2));
                    menu.push_back(Dish(dishIndex, dishName, dishPrice, dish));
                }

                dishIndex++;
                getline(myfile, line);
                while ((line == "")) {
                    getline(myfile, line);

                }

            }
            sortByPrice();

        }
        myfile.close();
    }
};

//start of actions
void Restaurant::start() {
    std::cout << "Restaurant is now open!\n";
    open = true;
    string line;
    getline(cin, line);

    while (!line.compare("CloseAll") == false && !line.compare("closeall") == false) {
        getAction(line);
        getline(cin, line);
    }
    CloseAll action = CloseAll();
    action.act(*this);

};

//seperate actions
void Restaurant::getAction(string line) {
    string s = line.substr(0, line.find(" "));
    if (s.compare("menu") == 0) {
        PrintMenu action = PrintMenu();
        action.act(*this);
    }
    else if (s.compare("open") == 0)
        openAct(line.substr((int)s.size() + 1, (int)line.size() - (int)s.size() + 1));
    else if (s.compare("order") == 0) {
        int tableNum = stoi(line.substr((int)s.size() + 1, (int)line.size() - (int)s.size() + 1));
        Order action = Order(tableNum);
        action.act(*this);
    }
    else if (s.compare("move") == 0)
        moveAct(line.substr((int)s.size() + 1, (int)line.size() - (int)s.size() + 1));
    else if (s.compare("status") == 0) {
        int tableNum = stoi(line.substr((int)s.size() + 1, (int)line.size() - ((int)s.size() + 1)));
        PrintTableStatus action = PrintTableStatus(tableNum);
        action.act(*this);
    }
    else if (s.compare("log") == 0) {
        PrintActionsLog action = PrintActionsLog();
        action.act(*this);
    }
    else if (s.compare("close") == 0) {
        int tableNum = stoi(line.substr((int)s.size() + 1, (int)line.size() - (int)s.size() + 1));
        Close action = Close(tableNum);
        action.act(*this);
    }
    else if (s.compare("backup") == 0) {
        BackupRestaurant action = BackupRestaurant();
        action.act(*this);
    }
    else if (s.compare("restore") == 0) {
        RestoreResturant action = RestoreResturant();
        action.act(*this);
    }

};

//call move act
void Restaurant::moveAct(string line) {
    int lastComma = 0;
    int thisComma = line.find(' ');
    int src = stoi(line.substr(lastComma, thisComma - lastComma));
    lastComma = thisComma;
    thisComma = line.find(' ', lastComma + 1);
    int dst = stoi(line.substr(lastComma, thisComma - lastComma));
    lastComma = thisComma;
    thisComma = line.find(' ', lastComma + 1);
    int customerId = stoi(line.substr(lastComma, thisComma - lastComma));
    MoveCustomer action = MoveCustomer(src, dst, customerId);
    action.act(*this);

};

//call open act
void Restaurant::openAct(string line) {
    int lastComma = 0;
    int thisComma = line.find(' ');
    string name = "", type = "";
    int tableNum = stoi(line.substr(0, thisComma - lastComma));
    line = line.substr(thisComma + 1, (int)line.size() - thisComma);
    vector<Customer*> customers;
    while ((int)line.size() > 0 && (int)line.size() > 3) { //more then one word
        name = line.substr(0, line.find(','));
        line = line.substr(line.find(',') + 1);
        type = line.substr(0, line.find(' '));
        line = line.substr(line.find(' ') + 1);
        if (type.compare("VEG") == 0 || type.compare("veg") == 0)
            customers.push_back(new VegetarianCustomer(name, numOfCus));
        else if (type.compare("ALC") == 0 || type.compare("alc") == 0)
            customers.push_back(new AlchoholicCustomer(name, numOfCus));
        else if (type.compare("SPC") == 0 || type.compare("spc") == 0)
            customers.push_back(new SpicyCustomer(name, numOfCus));
        else if (type.compare("CHP") == 0 || type.compare("chp") == 0)
            customers.push_back(new CheapCustomer(name, numOfCus));
        numOfCus++;
    }
    OpenTable action = OpenTable(tableNum, customers);
    action.act(*this);


};


void Restaurant::sortByPrice() {
    vector <Dish> newMenu;
    int min, jIndex = 0;
    min = 0;


    for (vector<Dish>::iterator i = menu.begin(); i != menu.end(); ++i) {
        for (vector<Dish>::iterator j = menu.begin(); j != menu.end(); ++j) {
            if (j->getPrice() < menu.at(min).getPrice() && !menu.at(jIndex).getSorted()) {
                min = jIndex;
            }
            jIndex++;
        }
        menu.at(min).setSorted(true);
        newMenu.push_back(menu.at(min));
        jIndex = 0;
        int h = 0;
        while (menu.at(h).getSorted() && h < (int) menu.size() - 1)
            h++;
        min = h;
    }
  menu.clear();
    for (vector<Dish>::iterator i = newMenu.begin(); i != newMenu.end(); ++i) {
        menu.push_back(*i);
    }

};

//getters&setters

int Restaurant::getNumOfTables() const {
    return (int)this->tables.size();
};

int Restaurant::getNumOfOpenTab(){
    int num;
    for(int i=0;i<(int)tables.size();i++)
        if(tables.at(i)->isOpen())
            num++;
    return num;
}

std::vector<Table*>  Restaurant::getTables() const {
    return this->tables;
};


int Restaurant::getNumOfCus() const {
    return numOfCus;
};

Table* Restaurant::getTable(int ind) {
    if ((int)tables.size() > ind)
        return tables.at(ind);
    return nullptr;
};
const vector<BaseAction*>& Restaurant::getActionsLog() const {
    return actionsLog;
};
// Return a reference to the history of actions

vector<Dish>& Restaurant::getMenu() {
    return menu;
};

void Restaurant::setActionsLog(BaseAction* action) {
    actionsLog.push_back((*action).clone());
};

int Restaurant::getActionsLogSize() const {
    return (int)actionsLog.size();
};



void Restaurant::setFirstBackup(bool b) {
    isFirstBackup = b;
};
bool Restaurant::getFirstBackup() const {
    return isFirstBackup;

};

//rule of five



Restaurant::~Restaurant() {//destructor
    clear();
};


//copy constructor
Restaurant::Restaurant(const  Restaurant& other) : numOfCus(other.getNumOfCus()), open(other.open) ,isFirstBackup(other.getFirstBackup()),  tables(),actionsLog(),menu()  {
    copy(other); //deep copy
};

//move constructor
Restaurant::Restaurant(Restaurant&& other): numOfCus(other.getNumOfCus()), open(other.open) ,isFirstBackup(other.getFirstBackup()),  tables(),actionsLog(),menu()  {
    if (this != &other) {
        //shallow copy
        shallowCopy(other);

        for (vector<Table*>::iterator i = other.getTables().begin(); i != other.getTables().end(); ++i) {
            *i = nullptr;
        }
        for (vector<BaseAction*>::iterator i = other.actionsLog.begin(); i != other.actionsLog.end(); ++i) {
            *i = nullptr;

        }
        other.tables.clear();
        other.actionsLog.clear();
    }

};


Restaurant& Restaurant:: operator=(const  Restaurant& other) {//copy assignment
    if (this != &other) {

        for (std::vector<Table*>::iterator i = tables.begin(); i != tables.end(); ++i) {
            delete *i;

        }

        for (std::vector<BaseAction*> ::iterator i = actionsLog.begin(); i != actionsLog.end(); ++i) {
            delete *i;
        }
        tables.clear();
        actionsLog.clear();
        copy(other);

    }
    return *this;

}



Restaurant& Restaurant:: operator=(Restaurant&& other) {//move assignment
    if (this != &other) {


        clear(); //deleting this previous data
        shallowCopy(other);

    }
    return *this;

}


void Restaurant::shallowCopy(const Restaurant& other) {
    for (vector<Table*>::iterator i = other.getTables().begin(); i != other.getTables().end(); ++i) {
        this->tables.push_back(*i);
    }

    for (vector<BaseAction*>::const_iterator i = other.actionsLog.begin(); i != other.actionsLog.end(); ++i) {
        this->actionsLog.push_back(*i);
    }

    open=other.open;
    numOfCus=other.getNumOfCus();
    isFirstBackup = other.getFirstBackup();
}


//deep copy function
void Restaurant::copy(const Restaurant& other) {
    //no need to copy menu
    open = other.open;
    isFirstBackup = other.getFirstBackup();
    numOfCus = other.getNumOfCus();
    for (int i = 0; i < (int)other.getTables().size(); i++) {
        tables.push_back(new Table(*other.getTables().at(i)));
    }
    for (std::vector<BaseAction*> ::const_iterator it = other.getActionsLog().begin(); it != other.getActionsLog().end(); ++it) {
        actionsLog.push_back((*it)->clone());
    }
};

void Restaurant::clear() {
    for (std::vector<Table*>::iterator i = tables.begin(); i != tables.end(); ++i) {
        delete *i;

    }

    for (std::vector<BaseAction*> ::iterator i = actionsLog.begin(); i != actionsLog.end(); ++i) {
        delete *i;
    }

    tables.clear();
    actionsLog.clear();
    menu.clear();

};
