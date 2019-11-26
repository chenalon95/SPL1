

#ifndef UNTITLED_CUSTOMER_H
#define UNTITLED_CUSTOMER_H

#include <vector>
#include <string>
#include "Dish.h"
using namespace std;
class Customer {
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu) = 0;
    virtual std::string toString() const = 0;
    virtual std::string customerType() const = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer();//Destructor
    virtual std::vector <Dish> getOrders() const = 0;
    virtual Customer* clone() = 0;

private:
    const std::string name;
    const int id;

};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::vector <Dish> getOrders() const;
    ~VegetarianCustomer();
    VegetarianCustomer* clone();
    std::string customerType() const;

private:
    std::vector <Dish> justOrdered;//last order -for toString printings

};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::vector <Dish> getOrders() const;
    ~CheapCustomer();
    CheapCustomer* clone();
    std::string customerType() const;

private:
    bool isFirstOrder;
    bool flag;
    string toStr;
    std::vector <Dish> justOrdered;//last order -for toString printings

};

class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::vector <Dish> getOrders() const;
    ~SpicyCustomer();
    SpicyCustomer* clone();
    std::string customerType() const;

private:
    bool firstOrder;
    std::vector<Dish> justOrdered;//last order -for toString printings

};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::vector <Dish> getOrders() const;
    ~AlchoholicCustomer();
    AlchoholicCustomer* clone();
    std::string customerType() const;

private:
    int lastDish;//index of last dish in the menu that was ordered
    bool flag;//justOrdered
    std::vector<Dish> justOrdered;//last order -for toString printings

};



#endif //UNTITLED_CUSTOMER_H
