
#ifndef UNTITLED_DISH_H
#define UNTITLED_DISH_H


#include <string>

enum DishType {
    VEG, SPC, BVG, ALC
};

class Dish {
public:
    Dish(int d_id, std::string d_name, int d_price, DishType d_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    DishType getType() const;
    bool getSorted() const;
    void setSorted(bool sort);
private:
    const int id;
    const std::string name;
    const int price;
    const DishType type;
    bool isSorted;
};


#endif //UNTITLED_DISH_H
