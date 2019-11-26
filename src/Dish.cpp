

#include "../include/Dish.h"
#include "../include/Table.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(d_name), price(d_price), type(d_type), isSorted(false) {}
int Dish::getId() const {
    return id;
};
string Dish::getName() const {
    return name;
};
int Dish::getPrice() const {
    return price;
};
DishType Dish::getType() const {
    return type;
};
bool Dish::getSorted() const {
    return isSorted;
};

void Dish::setSorted(bool sort) {
    isSorted = sort;
};
