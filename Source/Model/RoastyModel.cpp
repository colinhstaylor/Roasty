// RoastyModel.cpp
// Roasty cw - OODP
// Colin Taylor
// last edited 26/11/21

// Data Model for Roasty web app //

// Class function definitions //

#include <iostream>
#include "RoastyModel.hpp"

using namespace std;

Ingredient& Ingredient::operator=(Ingredient const& ingredient)  {
    this->bean = ingredient.getBean();
    this->beanAmount = ingredient.getAmount();
    return *this;
}

Event::Event(std::string type, long timestamp, EventValue const* eventValue) : type(type), timestamp(timestamp) {
    if (eventValue != nullptr) {
        this->eventValue = new EventValue(*eventValue);
    }
}

Event& Event::operator=(Event const& event) {
    this->type = event.getType();
    this->timestamp = event.getTimestamp();
    if (this->hasValue()) { delete this->eventValue; }
    if (event.hasValue()) {
        this->eventValue = new EventValue(event.getValue()->getValue());
    }
    return *this;
}

Event::~Event() {
    if (this->hasValue()) {
        delete eventValue;
    }
}

bool Event::hasValue() const {
    if (this->eventValue == nullptr) return false;
    else return true;
}

Roast::IngredientNode::IngredientNode(IngredientNode const& ingredientNode) {
    this->ingredient = ingredientNode.ingredient;
    if (ingredientNode.next != nullptr) {
        // recursively deep copy the list from this node
        this->next = new IngredientNode(*(ingredientNode.next));
    }
    else { this->next = nullptr; }
};

Roast::EventNode::EventNode(EventNode const& eventNode) {
    this->event = eventNode.event;
    if (eventNode.next != nullptr) {
        this->next = new EventNode(*(eventNode.next));
    }
    else { this->next = nullptr; }
}

// linkedListNode constructors implement recursive deep copy of rest of list
Roast::Roast(Roast const& roast) {
        this->timestamp = roast.getTimestamp();
        if (roast.ingredients != nullptr) {
           this->ingredients = new IngredientNode(*roast.ingredients); 
        }
        if (roast.events != nullptr) {
            this->events = new EventNode(*roast.events);
        }
    }

// linkedListNode constructors implement recursive deep copy of rest of list
Roast& Roast::operator=(Roast const& roast) {
    this->timestamp = roast.getTimestamp();
    if (roast.ingredients != nullptr) {
        this->deleteIngredients();
        this->ingredients = new IngredientNode(*roast.ingredients); 
    }
    else { this->ingredients = nullptr; }
    if (roast.events != nullptr) {
        this->deleteEvents();
        this->events = new EventNode(*roast.events);
    }
    else { this->events = nullptr; }
    return *this;
}

Roast::~Roast() {
    this->deleteIngredients();
    this->deleteEvents();
}

void Roast::deleteIngredients() {
    for (auto iterator = this->ingredients; iterator != nullptr;) {
        auto hold = iterator->next;
        delete iterator;
        iterator = hold; 
    }
}

void Roast::deleteEvents() {
    for (auto iterator = this->events; iterator != nullptr;) {
        auto hold = iterator->next;
        delete iterator;
        iterator = hold;
    }
}

int Roast::getEventCount() const {
    int count = 0;
    auto iterator = this->events;
    while (iterator != nullptr) {
        count++;
        iterator = iterator->next;
    }
    return count;
}

int Roast::getIngredientsCount() const {
    int count = 0;
    auto iterator = this->ingredients;
    while (iterator != nullptr) {
        count++;
        iterator = iterator->next;
    }
    return count;
}

Event const& Roast::getEvent(int i) const {
    auto iterator = events;
    for (int j = 1; j<i; j++) {
        iterator = iterator->next;
    }
    return iterator->event;
}

Ingredient const& Roast::getIngredient(int i) const {
    auto iterator = ingredients;
    for (int j = 1; j<i; j++) {
        iterator = iterator->next;
    }
    return iterator->ingredient;
}

Ingredient& Roast::getIngredient(int i) {
    auto iterator = ingredients;
    for (int j = 1; j<i; j++) {
        iterator = iterator->next;
    }
    return iterator->ingredient;
}

void Roast::addIngredient(Ingredient const& ingredient) {
    this->ingredients = new IngredientNode(ingredient, this->ingredients);
}

void Roast::addIngredient(Bean const& bean, int newAmount) {
    this->ingredients = new IngredientNode(bean, newAmount, this->ingredients);
}

void Roast::addEvent(Event const& event) {
    this->events = new EventNode(event, this->events);
}

// empty function not implemented
//void Roast::removeIngredientByBeanName(std::string const& beanName) {}

//buggy attempt with seg fault:11 :

void Roast::removeIngredientByBeanName(std::string const& beanName) {
    auto nodePointer = &(this->ingredients); // pointer to 
    if ((*nodePointer) != nullptr) {
        // if there are any ingredients
        // if the first ingredient is the bean
        if ((*nodePointer)->ingredient.getBean().getName() == beanName) {
            auto hold = (*nodePointer)->next;
            delete (*nodePointer);
            ingredients = hold;
        }
        while(true) {
            // check whether next ingredient is the bean
            if ((*nodePointer)->next->ingredient.getBean().getName() == beanName) {
                // nodePointer->next is the bean //
                // hold on to the next next ingredient in the list
                auto hold = (*nodePointer)->next->next;
                (*nodePointer) = hold;
                // then delete the next node because it is the bean
                delete (*nodePointer)->next;
            }
            // else if not the bean and 
            else if ((*nodePointer)->next == nullptr) {
                // bean is not in list
                return;
            }
            // else try the next ingredient
            else { nodePointer = &((*nodePointer)->next); }
        }
    }
}


// empty function not implemented
void Roast::removeEventByTimestamp(long eventTimestamp) {}

// buggy attempt with seg fault:
/*
void Roast::removeEventByTimestamp(long eventTimestamp) {
    auto nodePointer = &(this->events); // pointer to the pointer
    if (*nodePointer != nullptr) {
        // if there are any events
        while(true) {
            // check current event has this timestamp
            if ((*nodePointer)->event.getTimestamp() == eventTimestamp) {
                // FOUND THE EVENT //
                // hold on to the node
                auto hold = *nodePointer;
                // reassign the pointer in the list (behind nodePointer) to the next element in the list
                *nodePointer = (*nodePointer)->next;
                // then delete the node held
                delete hold;
            }
            // else if not the event and 
            else if ((*nodePointer)->next == nullptr) {
                // event is not in list
                return;
            }
            // else try the next event
            else { nodePointer = &((*nodePointer)->next); }
        }
    }
}
*/

Event const& Roast::getEventByTimestamp(long eventTimestamp) const {
    auto nodePointer = this->events;
    if (nodePointer != nullptr) {
        // if there are any events
        while(true) {
            // check current event has this timestamp
            if (nodePointer->event.getTimestamp() == eventTimestamp) {
                // FOUND THE EVENT // so return the event object
                return nodePointer->event;
            }
            // else this is not the event we were looking for
            else if (nodePointer->next == nullptr) {
                // if at the end the event is not in list so throw error
                cerr << "Event is not in the list. Cannot return a valid reference. Exiting program." << endl;
                exit(1);
            }
            // or else try the next event
            else { nodePointer = nodePointer->next; }
        }
    }
    // if events == nullptr throw the error
    else {
        cerr << "Event is not in the list. Cannot return a valid reference. Exiting program." << endl;
        exit(1);
    }
}