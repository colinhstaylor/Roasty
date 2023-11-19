// RoastyModel.hpp
// Roasty cw - OODP
// Colin Taylor
// last edited 26/11/21

/* sources:
overloading string() for implicit casting: https://www.learncpp.com/cpp-tutorial/overloading-typecasts/
*/

// Data Model for Roasty web app //

#ifndef ROASTY_MODEL_H
#define ROASTY_MODEL_H

// Class definitions //

#include <string>

class Bean {
    std::string name = {};
public:
    Bean() {};
    Bean(std::string name) : name(name){};
    operator std::string() { return name; }
    std::string getName() const { return name; }
};

class Ingredient {
    Bean bean;
    int beanAmount = 0;
public:
    Ingredient() {};
    Ingredient(Ingredient const& ingredient) { *this = ingredient; }
    Ingredient(Bean const& bean, int beanAmount) : bean(bean), beanAmount(beanAmount) {};
    Ingredient& operator=(Ingredient const& ingredient);
    Bean const& getBean() const { return bean; }
    int getAmount() const { return beanAmount; }
    void setAmount(int beanAmount) { this-> beanAmount = beanAmount; }
};

class EventValue {
    long value = 0;
public:
    EventValue() {};
    EventValue(EventValue const& eventValue) { value = eventValue.getValue(); } 
    EventValue(long value) : value(value) {};
    long getValue() const { return value; }
    void setValue(long value) { this->value = value; }
};

class Event {
    std::string type = {};
    long timestamp = 0;
    EventValue* eventValue = nullptr;
public:
    Event() {};
    Event(Event const& event) { *this = event; }
    Event(std::string type, long timestamp) : type(type), timestamp(timestamp) {};
    // returns ownership of new EventValue object assigned to member pointer
    Event(std::string type, long timestamp, EventValue const* eventValue);
    // returns ownership of new EventValue object assigned to member pointer
    Event& operator=(Event const& event);
    ~Event();
    long getTimestamp() const { return timestamp; }
    std::string const& getType() const { return type; }
    bool hasValue() const;
    // const function returns pointer to modifiable object so that getValue()->setValue(...) can be called on const Event object
    EventValue* getValue() const { return eventValue; }
};

class Roast {
    long timestamp = 0;
    struct IngredientNode {
        Ingredient ingredient;
        IngredientNode* next = nullptr;
        IngredientNode();
        // returns ownership of copies of rest of list starting from parameter node ingredientNode
        IngredientNode(IngredientNode const& ingredientNode);
        IngredientNode(Ingredient const& ingredient) : ingredient(ingredient){};
        // ownership stays with the owner of 'next' parameter
        IngredientNode(Ingredient const& ingredient, IngredientNode* next) : ingredient(ingredient), next(next) {};
        // ownership stays with the owner of 'next' parameter
        IngredientNode(Bean const& bean, int newAmount, IngredientNode* next) : IngredientNode(Ingredient(bean, newAmount), next) {};
    }* ingredients = nullptr;
    struct EventNode {
        Event event;
        EventNode* next = nullptr;
        EventNode();
        // returns ownership of copies of rest of list starting from parameter node eventNode
        EventNode(EventNode const& eventNode);
        // ownership stays with the owner of 'next' parameter
        EventNode(Event const& event, EventNode* next) : event(event), next(next) {};
    }* events = nullptr;
public:
    Roast() {};
    // returns ownership of new linked lists copied from parameter 'roast' object member pointers
    Roast(Roast const& roast);
    Roast(long timestamp) : timestamp(timestamp) {};
    // returns ownership of new linked lists copied from parameter 'roast' object member pointers
    Roast& operator=(Roast const& roast);
    ~Roast();
    void deleteIngredients();
    void deleteEvents();
    long getTimestamp() const { return timestamp; }
    int getEventCount() const;
    int getIngredientsCount() const;
    // returns the 'ith' element in the list
    Event const& getEvent(int i) const;
    // returns the 'ith' element in the list
    Ingredient const& getIngredient(int i) const;
    // need const overloading to call setAmount() on return object
    Ingredient& getIngredient(int i);
    // creates a node now owned by the Roast object
    void addIngredient(Ingredient const& ingredient);
    // creates a node now owned by the Roast object
    void addIngredient(Bean const& bean, int newAmount);
    // failed to implement this. function does nothing...
    void removeIngredientByBeanName(std::string const& beanName); 
    // creates a node now owned by the Roast object
    void addEvent(Event const& event);
    // failed to implement this. function does nothing...
    void removeEventByTimestamp(long eventTimestamp);
    Event const& getEventByTimestamp(long eventTimestamp) const;
};

#endif