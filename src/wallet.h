// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// The root object that holds all data for 371pass. This
// class contains Categories, which in turn hold Items,
// which hold a mapping of entries.
// -----------------------------------------------------

#ifndef WALLET_H
#define WALLET_H

#include <set>
#include <vector>

#include "category.h"

class Wallet {
private:
	// std::set<Category> categories;
	std::vector<Category> categories;

public: 
	Wallet();

	size_t size();
	bool empty();

	Category& newCategory(const std::string& categoryIdent);
	bool addCategory(const Category& newCategory);
	Category& getCategory(const std::string& categoryIdent);
	bool deleteCategory(const std::string& categoryIdent);

	friend bool operator==(const Wallet& lhs, const Wallet& rhs);

	std::string str() const;

	void load(const std::string& path);
	void save(const std::string& path) const;
};

#endif // WALLET_H
