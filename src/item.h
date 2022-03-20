// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// An Item class contains multiple 'entries' as
// key/value pairs (e.g., a key might be 'username'
// and a value would be the username, another might be
// 'url' and the value is the website address the
// username is for.
// -----------------------------------------------------

#ifndef ITEM_H
#define ITEM_H

#include <map>
#include <string>

class Item {
private:
	std::string name;
	std::map<std::string, std::string> data;

public:
	Item(const std::string& name);
	Item();

	size_t size() const;
	bool empty() const;

	void setIdent(const std::string& newIdent);
	std::string getIdent() const;

	bool addEntry(const std::string& key, const std::string& value);
	bool addEntry(const std::pair<std::string, std::string>& newEntry);//
	std::string getEntry(const std::string& key) const;
	bool deleteEntry(const std::string& key);
	bool mergeEntries(const Item& mergeItem);//

	friend bool operator==(const Item& lhs, const Item& rhs);
	friend bool operator<(const Item& lhs, const Item& rhs);//

	std::string str() const;
};

#endif // ITEM_H
