// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "wallet.h"
#include "lib_json.hpp"

using json = nlohmann::json;

// Wallet constructor that takes no parameters and constructs an empty wallet.
Wallet::Wallet() {}

// Function, size, that takes no parameters and returns an unsigned
//  int of the number of categories in the Wallet contains.
size_t Wallet::size() {
	return categories.size();
}

// Function, empty, that takes no parameters and returns true
//  if the number of categories in the Wallet is zero, false otherwise.
bool Wallet::empty() {
	if (categories.size() == 0) {
		return true;
	}
	return false;
}

// Function, newCategory, that takes one parameter, a category
//  identifier, and returns the Category object as a reference. If an object
//  with the same identifier already exists, then the existing object should be
//  returned. Throw a std::runtime_error if the Category object cannot be
//  inserted into the container.
Category& Wallet::newCategory(const std::string& categoryIdent) {
	// Check if such element exist
	std::vector<Category>::iterator it;
	for (it = categories.begin(); it != categories.end()
		&& !(it->getIdent() == categoryIdent); it++);

	if (it != categories.end()) {
		return *it;
	}

	// If not, create an object and return it
	Category* category = new Category(categoryIdent);
	categories.push_back(*category);
	return getCategory(categoryIdent);
}


// Function, addCategory, that takes one parameter, a Category
//  object, and returns true if the object was successfully inserted. If an
//  object with the same identifier already exists, then the contents should be
//  merged and then return false. Throw a std::runtime_error if the Category
//  object cannot be inserted into the container for whatever reason.
bool Wallet ::addCategory(const Category& newCategory) {
	// If such element exists merge it and return false
	std::string itemIdent = newCategory.getIdent();
	for (Category category : categories) {
		if (category.getIdent() == itemIdent) {
			category.megreItems(newCategory);
			return false;
		}
	}

	// If not, push a new element and return true
	categories.push_back(newCategory);
	return true;
}

// Function, getCategory, that takes one parameter, a Category
//  identifier and returns the Category. If no Category exists, throw an
//  appropriate exception.
Category& Wallet::getCategory(const std::string& categoryIdent) {
	std::vector<Category>::iterator it;
	for (it = categories.begin(); it != categories.end()
		&& !(it->getIdent() == categoryIdent); it++);

	if (it != categories.end()) {
		return *it;
	}

	

	throw std::invalid_argument("category with such a name was not found to get");
}

// Function, deleteCategory, that takes one parameter, a Category
//  identifier, and deletes it from the container, and returns true if the
//  Category was deleted. If no Category exists, throw an appropriate exception.
bool Wallet::deleteCategory(const std::string& categoryIdent ) {
	for (unsigned int i = 0; i < categories.size(); i++) {
		if (categories.at(i).getIdent() == categoryIdent) {
			categories.erase(categories.begin() + i);
			return true;
		}
	}

	throw std::invalid_argument("category with such a name was not found to delete");
	return false;
}

// Function, load, that takes one parameter, a std::string,
//  containing the filename for the database. Open the file, read the contents,
//  and populates the container for this Wallet. If the file does open throw an
//  appropriate exception (either std::runtime_error or a derived class).
void Wallet::load(const std::string& path) {
	// Read the file
	std::ifstream reader(path);
	if (reader.fail()) {
		throw std::runtime_error("No such file of folder");
		return;
	}

	// If successful create json object
	json jsonFile = {};
	reader >> jsonFile;

	/*std::cout << "--- Provided database ---" << std::endl;
	std::cout << std::setw(1) << jsonFile << std::endl;*/

	// Fill the object with the data
	for (auto& it : jsonFile.items()) {
		// Create category and fill it with data
		Category category = Category((std::string) it.key());

		for (auto& it2 : it.value().items()) {
			// Create Item and fill it with data
			Item item = Item((std::string)it2.key());

			for (auto& it3 : it2.value().items()) {
				// Fill the item with data
				item.addEntry((std::string) it3.key(), (std::string) it3.value());
			}

			category.addItem(item);
		}

		this->addCategory(category);
	}

	reader.close();
}

// Function ,save, that takes one parameter, the path of the file
//  to write the database to. The function should serialise the Wallet object
//  as JSON.
void Wallet::save(const std::string& path) const {
	std::ofstream writer(path);
	if (writer.fail()) {
		throw std::runtime_error("error occured during writer initialization");
		return;
	}

	writer << this->str();
	writer.close();
}

// An == operator overload for the Wallet class, such that two
//  Wallet objects are equal only if they have the exact same data.
bool operator==(const Wallet& lhs, const Wallet& rhs) {

	for (Category category : lhs.categories) {
		if (std::find(rhs.categories.begin(), rhs.categories.end(), 
			category) != rhs.categories.end()) {

			return false;
		}
	}

	return true;
}

// Function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Wallet.
std::string Wallet::str() const {
	std::stringstream ss;
	ss << "{" << std::endl;

	unsigned int i = 1;
	for (Category category : categories) {
		ss << category.str();

		if (i++ != categories.size()) {
			ss << ",";
		}

		ss << std::endl;
	}

	ss << "}" << std::endl;

	return ss.str();
}