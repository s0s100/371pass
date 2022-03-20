// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// A category contains one or more Items, each with
// their own identifier ('ident').
// -----------------------------------------------------

#ifndef CATEGORY_H
#define CATEGORY_H

#include <set>
#include <vector>

#include "item.h"

class Category {
private:
	std::string name;
	std::set<Item> items;
	// std::vector<Item> items;

public:
	Category();
	Category(const std::string& name);

	size_t size() const;
	bool empty() const;

	std::string getIdent() const;
	void getIdent(const std::string& newIdent);

	Item& newItem(const std::string& itemIdent);
	bool addItem(const Item& newItem);
	Item getItem(const std::string& itemIdent) const;
	bool deleteItem(const std::string& itemIdent);
	bool megreItems(const Category& mergeCategory);//

	friend bool operator==(const Category& lhs, const Category& rhs);
	friend bool operator<(const Category& lhs, const Category& rhs);//

	std::string str() const;
};

#endif // CATEGORY_H
