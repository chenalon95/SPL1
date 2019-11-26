
#include "../include/Customer.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

string Customer::getName() const {
    return name;
};
int Customer::getId() const {
    return id;
};
Customer:: ~Customer() {

}


//VegetarianCustomer:
VegetarianCustomer::VegetarianCustomer(std::string name, int id) :Customer::Customer(name, id) ,justOrdered() {};

std::vector<int>  VegetarianCustomer::order(const std::vector<Dish> &menu) {
    vector<int> ret;
    int index = 0;
    //finding a VEG dish
    while (index < (int)menu.size() && menu.at(index).getType() != VEG )
        index += 1;
    if (index < (int)menu.size()) {//if VEG dish found
        int vIndex = index;
        while (vIndex < (int)menu.size()) {//find VEG with minimal ID
            if (menu.at(vIndex).getType() == VEG && menu.at(vIndex).getId() < menu.at(index).getId())
                index = vIndex;
            vIndex++;
        }
        ret.push_back(menu.at(index).getId());
        justOrdered.push_back(menu.at(index));
        index = (int)menu.size() - 1;
        //finding most expansive BVG dish
        while (index > 0 && menu.at(index).getType() != BVG)
            index--;
        int index1=0;
        if (index >= 0) {//if there is a BVG in the menu
            while (index1 < (int) menu.size() &&
                   (menu.at(index1).getType() != BVG || menu.at(index1).getPrice() < menu.at(index).getPrice())) {
                index1++;
            }
            if (index1 < (int) menu.size())
                index=index1;
            ret.push_back(menu.at(index).getId());
            justOrdered.push_back(menu.at(index));
        }
        else {
            ret.pop_back();
            justOrdered.pop_back();
        }
    }
    return ret;
};
string  VegetarianCustomer::toString() const {
    string ret = "";
    if ((int)this->justOrdered.size() > 1)
        ret = this->getName() + " ordered " + justOrdered[0].getName() + "\n" + this->getName()
              + " ordered " + justOrdered[1].getName() + "\n";
    return ret;
};


std::vector <Dish> VegetarianCustomer::getOrders() const {
    return justOrdered;
}
VegetarianCustomer::~VegetarianCustomer() {

}


VegetarianCustomer* VegetarianCustomer::clone() {
    return new VegetarianCustomer(*this);
}

string VegetarianCustomer::customerType() const {
    return "veg";
}



//CheapCustomer:
CheapCustomer::CheapCustomer(std::string name, int id) : Customer::Customer(name, id), isFirstOrder(true),flag(false), toStr(""), justOrdered()  {
};

vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    vector<int> ret;
    if (isFirstOrder) {
        if ((int)menu.size() > 0) {
            ret.push_back(menu.at(0).getId());//menu is sorted by price
            justOrdered.push_back(menu.at(0));
            flag=true;
        }
        else {
            flag = false;
            toStr="";
        }
    }
    if ((int)this->justOrdered.size() == 1 && isFirstOrder == true && flag==true) {
        toStr = this->getName() + " ordered " + justOrdered.at(0).getName() + "\n";
        isFirstOrder = false;
    }
    else
        toStr = "";

    return ret;
};

string CheapCustomer::toString() const {
    return toStr;
};


std::vector <Dish> CheapCustomer::getOrders() const {
    return justOrdered;
}

CheapCustomer::	~CheapCustomer() {}

CheapCustomer* CheapCustomer::clone() {
    return new CheapCustomer(*this);
}

string CheapCustomer::customerType() const {
    return "chp";
}



//SpicyCustomer
SpicyCustomer::SpicyCustomer(std::string name, int id) :Customer::Customer(name, id), firstOrder(true), justOrdered() {};

vector<int>  SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> ret;
    if ((int)menu.size() > 0) {
        int index = (int)menu.size() - 1, orderSpc = -1, orderBvg = -1;
        //finding most expansive SPC in menu
        while (index >= 0 && menu.at(index).getType() != SPC)
            index--;
        int index1=0;
        if (index >= 0) {//there is an SPC in the menu
            while (index1 < (int) menu.size() &&
                   (menu.at(index1).getType() != SPC || menu.at(index1).getPrice() < menu.at(index).getPrice())) {
                index1++;
            }
            if (index1 < (int) menu.size())
                index=index1;
            orderSpc = menu.at(index).getId();
            //order cheapest BVG
            index = 0;
            while (index < (int) menu.size() && menu.at(index).getType() != BVG)
                index++;
            if (index < (int) menu.size()) {//if there is BVG in menu
                orderBvg = menu.at(index).getId();
            }
            else {//no BVG so he won't order at all
                ret.pop_back();
            }


        }
        if (orderSpc != -1 ) {//there is SPC
            if (firstOrder) {//order SPC
                firstOrder = false;
                ret.push_back(orderSpc);
                for (int i = 0; i < (int)menu.size(); i++) {
                    if (menu.at(i).getId() == orderSpc)
                        justOrdered.push_back(menu.at(i));
                }

            }
            else {//order BVG
                ret.push_back(orderBvg);
                for (int i = 0; i < (int)menu.size(); i++) {
                    if (menu.at(i).getId() == orderBvg)
                        justOrdered.push_back(menu.at(i));
                }
            }
        }
    }
    return ret;
}

SpicyCustomer::~SpicyCustomer() {}


string  SpicyCustomer::toString() const {
    string ret = "";
    if ((int)this->justOrdered.size() > 0)
        ret = this->getName() + " ordered " + justOrdered.at((int)justOrdered.size() - 1).getName() + "\n";
    return ret;
};


std::vector <Dish> SpicyCustomer::getOrders() const {
    return justOrdered;
}


SpicyCustomer* SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}

string SpicyCustomer::customerType() const {
    return "spc";
}




//AlchoholicCustomer:
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer::Customer(name, id), lastDish(-1), flag(false), justOrdered() {
};
vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    vector<int> ret;
    if (lastDish == -1) {//first order
        int i = 0;

        //find ALC dish
        while (i < (int)menu.size() && menu.at(i).getType() != ALC)
            i++;
        if (i < (int)menu.size()) {//there's ALC in the menu
            ret.push_back(menu.at(i).getId());
            lastDish = i;
            justOrdered.push_back(menu.at(i));
            flag=true;
        }
        else
            flag=false;
    }
    else {//not the first order
        if ((int)justOrdered.size() != 0) {//there is ALC in the menu
            int i;
            //find next ALC, starting from the last ALC found
            for (i = lastDish +1; i < (int)menu.size() && menu.at(i).getType() != ALC; i++) {}
            if (i<(int)menu.size()) {
                lastDish = i;
                ret.push_back(menu.at(lastDish).getId());
                justOrdered.push_back(menu.at(lastDish));
                flag=true;
            }
            else
                flag=false;

        }
    }
    return ret;
}


string AlchoholicCustomer::toString() const {
    string ret = "";
    if ((int)this->justOrdered.size() > 0 && flag==true)
        ret = this->getName() + " ordered " + justOrdered.at((int)justOrdered.size() - 1).getName() + "\n";
    return ret;
};


AlchoholicCustomer::~AlchoholicCustomer() {}

std::vector <Dish> AlchoholicCustomer::getOrders() const {
    return justOrdered;
}


AlchoholicCustomer* AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}

string AlchoholicCustomer::customerType() const {
    return "alc";
}
