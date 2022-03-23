// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <iostream>
#include <sstream>
#include <algorithm>

#include "category.h"

// Default constuctor
Category::Category() {}

// Constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
Category::Category(const std::string &name) : name(name) {}

// Function, size, that takes no parameters and returns an unsigned
//  int of the number of Items in the Category contains.
size_t Category::size() const {
	return items.size();
}

// Function, empty, that takes no parameters and returns true
//  if the number of Items in the Category is zero, false otherwise.
bool Category::empty() const {
	if (items.size() == 0) {
		return true;
	}
	return false;
}

// Function, getIdent, that returns the identifier for the
//  Category.
std::string Category::getIdent() const {
	return name;
}

// Function, setIdent, that takes one parameter, a string for a new
//  Category identifier, and updates the member variable. It returns nothing.
void Category::getIdent(const std::string& newIdent) {
	name = newIdent;
}

// Function, newItem, that takes one parameter, an Item identifier,
//  (a string) and returns the Item object as a reference. If an object with the
//  same identifier already exists, then the existing object should be returned.
//  Throw a std::runtime_error if the Item object cannot be inserted into the
//  container for whatever reason.
Item& Category::newItem(const std::string& itemIdent) {
	// Check if such element exist
	std::vector<Item>::iterator it;
	for (it = items.begin(); it != items.end()
		&& !(it->getIdent() == itemIdent); it++);

	if (it != items.end()) {
		return *it;
	}

	// If not, create an object and return it
	Item* item = new Item(itemIdent);
	items.push_back(*item);
	// return *item;
	return getItem(itemIdent);
}

// Function, addItem, that takes one parameter, an Item object,
//  and returns true if the object was successfully inserted. If an object with
//  the same identifier already exists, then the contents should be merged and
//  return false.
bool Category::addItem(const Item& newItem) {
	// If such element exists merge it and return false
	std::string itemIdent = newItem.getIdent();
	for (Item item : items) {
		if (item.getIdent() == itemIdent) {
			item.mergeEntries(newItem);
			return false;
		}
	}

	// If not, push a new element and return true
	items.push_back(newItem);
	return true;
}

// Function, getItem, that takes one parameter, an Item
//  identifier (a string) and returns the Item as a reference. If no Item
//  exists, throw an appropriate exception.
Item& Category::getItem(const std::string& itemIdent) {
	std::vector<Item>::iterator it;
	for (it = items.begin(); it != items.end()
		&& !(it->getIdent() == itemIdent); it++);

	if (it != items.end()) {
		return *it;
	}

	// Throw an error and return an empty item
	throw std::out_of_range("item with such a name was not found to get");
	Item* item = new Item();
	return *item;
}

// Function, deleteItem, that takes one parameter, an Item
//  identifier (a string), deletes it from the container, and returns true if
//  the Item was deleted. If no Item exists, throw an appropriate exception.
bool Category::deleteItem(const std::string& itemIdent) {
	for (unsigned int i = 0; i < items.size(); i++) {
		if (items.at(i).getIdent() == itemIdent) {
			items.erase(items.begin() + i);
			return true;
		}
	}

	throw std::out_of_range("item with such a name was not found to delete");
	return false;
}

// Allows to merge two categories with the same name
bool Category::megreItems(const Category& mergeCategory)
{
	// First check the name
	if (getIdent() != mergeCategory.getIdent()) {
		return false;
	}
	
	// Parse every element and add it to the category
	for (Item item : mergeCategory.items) {
		addItem(item);
	}

	return true;
}

// An == operator overload for the Category class, such that two
//  Category objects are equal only if they have the same identifier and same
//  Items.
bool operator==(const Category& lhs, const Category& rhs) {
	if (lhs.getIdent() != rhs.getIdent()) {
		return false;
	}

	// Check every item in the class
	for (Item item : lhs.items) {
		if (std::find(rhs.items.begin(), rhs.items.end(),
			item) != rhs.items.end()) {
			return false;
		}
	}

	return true;
}

// Custom compare operator to parse through the set
bool operator<(const Category& lhs, const Category& rhs) {
	if (lhs.getIdent() < rhs.getIdent()) {
		return true;
	}
	return false;
}

// Function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Category.
std::string Category::str() const {
	std::stringstream ss;

	// Nice output
	//ss << "  \"" << getIdent() << "\": {" << std::endl;

	//unsigned int i = 1;
	//for (Item item : items) {
	//	ss << item.str();

	//	if (i++ != items.size()) {
	//		ss << ",";
	//	}

	//	ss << std::endl;
	//}

	//ss << "  }";

	// Required output
	ss << "{";
	unsigned int i = 1;
	for (Item item : items) {
		ss << "\"" << item.getIdent() << "\":";
		ss << item.str();

		if (i++ != items.size()) {
			ss << ",";
		}
	}
	ss << "}";

	return ss.str();
}
