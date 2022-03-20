// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <sstream>

#include "item.h"

// Default constructor
Item::Item() {}

// Constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
Item::Item(const std::string& name) : name(name) {}

// Function, size, that takes no parameters and returns an unsigned
//  int of the number of entries in the Item contains.
size_t Item::size() const {
	return data.size();
}

// Function, empty, that takes no parameters and returns true
//  if the number of entries in the Item is zero, false otherwise.
bool Item::empty() const {
	if (data.size() == 0) {
		return true;
	}
	return false;
}

// Function, setIdent, that takes one parameter, a string for a new
//  Item identifier, and updates the member variable. It returns nothing.
void Item::setIdent(const std::string& newIdent) {
	name = newIdent;
}

// Function, getIdent, that returns the identifier for the Item.
std::string Item::getIdent() const{
	return name;
}

// Function, addEntry, that takes two parameters, an entry
//  key and value and returns true if the entry was inserted into the
//  container or false if the entry already existed and was replaced.
bool Item::addEntry(const std::string& key, const std::string& value) {
	if (data.find(key) != data.end()) {
		return false;
	}

	data.insert(std::make_pair(key, value));
	return true;
}

// Adds a new entry by using pair data structure
bool Item::addEntry(const std::pair<std::string, std::string>& newEntry) {
	if (data.find(newEntry.first) != data.end()) {
		return false;
	}

	data.insert(newEntry);
	return true;
}

// Function, getEntry, that takes one parameter, an entry
//  key and returns it's value. If no entry exists, throw an appropriate
//  exception.
std::string Item::getEntry(const std::string& key) const {
	auto it = data.find(key);
	if (it == data.end()) {
		throw std::invalid_argument("no such a key in the map");
		return "";
	}

	return it->second;
}

// Function, deleteEntry, that takes one parameter, an entry
//  key, deletes it from the container, and returns true if the Item was
//  deleted. If no entry exists, throw an appropriate exception.
bool Item::deleteEntry(const std::string& key) {
	auto it = data.find(key);
	if (it == data.end()) {
		throw std::invalid_argument("no such key in the map");
		return false;
	}

	data.erase(key);
	return true;
}

// Function that merges two items with the same identifier
bool Item::mergeEntries(const Item& mergeItem) {
	if (getIdent() != mergeItem.getIdent()) {
		return false;
	}

	// Add every entry from the item to be merged
	for (auto& it : mergeItem.data) {
		addEntry(it);
	}
	return true;
}

// An == operator overload for the Item class, such that two
//  Item objects are equal only if they have the same identifier and same
//  entries.
bool operator==(const Item& lhs, const Item& rhs)
{
	if (lhs.getIdent() != rhs.getIdent()) {
		return false;
	}
	if (lhs.size() != rhs.size()) {
		return false;
	}

	// Parse every element in maps and compare them
	std::string key;
	std::string value;
	std::map<std::string, std::string> rMap = rhs.data;
	for (auto& it : lhs.data) {
		key = it.first;
		value = it.second;

		auto it2 = rMap.find(key);
		if (it2 == rhs.data.end() || value != it2->second) {
			return false;
		}
	}

	return true;
}

// Custom compare operator to parse through the set
bool operator<(const Item& lhs, const Item& rhs) {
	if (lhs.getIdent() < rhs.getIdent()) {
		return true;
	}
	return false;
}

// Function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Item.
std::string Item::str() const {
	std::stringstream ss;
	ss << "    \"" << getIdent() <<  "\":  {" << std::endl;

	// Comma at the end of each line
	unsigned int i = 1;
	for (auto& it : data) {
		ss << "      \"" << it.first << "\":  \""
			<< it.second << "\"";
		if (i++ != data.size()) {
			ss << ",";
		}
		ss << std::endl;
	}

	ss << "    }";

	return ss.str();
}
