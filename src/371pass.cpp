// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

#include "371pass.h"
#include "lib_cxxopts.hpp"
#include "wallet.h"

// Main console run command
int App::run(int argc, char *argv[]) {
    auto options = App::cxxoptsSetup();
    auto args = options.parse(argc, argv);

    // Print the help usage if requested
    if (args.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }

    // Read the db and upload a Wallet object to interact with
    const std::string db = args["db"].as<std::string>();
    Wallet wallet{};
    wallet.load(db);

    /*if (wallet.empty()) {
        return 1;
    }*/

    // Just to be sure show the wallet object
    std::cout << " --- Provided database --- " << std::endl;
    std::cout << getJSON(wallet) << std::endl;
    std::cout << " --- Result: --- " << std::endl;

    const Action a = parseActionArgument(args);
    switch (a) {
    case Action::CREATE: {
        bool countCheck = args.count("category") != 0;
        bool itemCheck = args.count("item") != 0;
        bool entryCheck = args.count("entry") != 0;

        if (countCheck && itemCheck && entryCheck) {
            std::string categoryName = args["category"].as<std::string>();
            std::string itemName = args["item"].as<std::string>();
            std::string entryName = args["entry"].as<std::string>();
            std::pair<std::string, std::string> splitted = stringSplit(entryName, ",");

            if (splitted.first == "" && splitted.second == "") {
                return 1;
            }

            // Update the wallet and save it back to JSON
            Category& category = wallet.newCategory(categoryName);
            Item& item = category.newItem(itemName);
            if (!item.addEntry(splitted)) {
                throw std::invalid_argument("Such entry already exists");
                return 1;
            }

            wallet.save(db);
        }
        else {
            throw std::invalid_argument("missing category, item or entry argument(s)");
            return 1;
        }

        break;
    }
        
    case Action::READ: {
        bool countCheck = args.count("category") != 0;
        bool itemCheck = args.count("item") != 0;
        bool entryCheck = args.count("entry") != 0;

        // First check if category exists
        std::string result;
        if (countCheck) {
                std::string categoryName = args["category"].as<std::string>();

            // Then chech if item exists
            if (itemCheck) {
                std::string itemName = args["item"].as<std::string>();

                // Entry check as well
                if (entryCheck) {
                    std::string entryName = args["entry"].as<std::string>();

                    // Print entry
                    result = getJSON(wallet, categoryName,
                        itemName, entryName);
                }
                else {
                    // Print item
                    result = getJSON(wallet, categoryName,
                        itemName);
                }
            }
            else {
                // Print category
                if (entryCheck) {
                    throw std::invalid_argument("missing category, item or entry argument(s)");
                    result = "";
                }
                else {
                    result = getJSON(wallet, categoryName);
                }
            }
        }
        else {
            // Print wallet
            if (itemCheck || entryCheck) {
                throw std::invalid_argument("missing category, item or entry argument(s)");
                result = "";
            }
            else {
                result = getJSON(wallet);
            }
        }

        // If successful print result to the console
        if (result == "") {
            return 1;
        }
        else {
            std::cout << result;
        }

        break;
    }
        
    case Action::UPDATE: {
        bool countCheck = args.count("category") != 0;
        bool itemCheck = args.count("item") != 0;
        bool entryCheck = args.count("entry") != 0;

        if (countCheck && itemCheck && entryCheck) {
            std::string categoryName = args["category"].as<std::string>();
            std::string itemName = args["item"].as<std::string>();
            std::string entryName = args["entry"].as<std::string>();
            std::pair<std::string, std::string> splitted = stringSplit(entryName, ",");

            if (splitted.first == "" && splitted.second == "") {
                return 1;
            }

            // Update the wallet and save it back to JSON
            Category& category = wallet.getCategory(categoryName);
            if (category.empty()) {
                return 1;
            }

            Item& item = category.getItem(itemName);
            if (item.empty()) {
                return 1;
            }

            bool result = item.deleteEntry(splitted.first);
            if (result) {
                item.addEntry(splitted);
            }
            else {
                return 1;
            }

            wallet.save(db);
        }
        else {
            throw std::invalid_argument("missing category, item or entry argument(s)");
            return 1;
        }

        break;
    }
    
    case Action::DELETE: {
        bool countCheck = args.count("category") != 0;
        bool itemCheck = args.count("item") != 0;
        bool entryCheck = args.count("entry") != 0;


        // First check if category exists
        if (countCheck) {
            std::string categoryName = args["category"].as<std::string>();

            // Then chech if item exists
            if (itemCheck) {
                std::string itemName = args["item"].as<std::string>();

                // Entry check as well
                if (entryCheck) {
                    std::string entryName = args["entry"].as<std::string>();

                    // Delete selected entry
                    Category& category = wallet.getCategory(categoryName);
                    if (category.empty()) {
                        return 1;
                    }

                    Item& item = category.getItem(itemName);
                    if (item.empty()) {
                        return 1;
                    }

                    bool result = item.deleteEntry(entryName);
                    if (!result) {
                        throw std::invalid_argument("such entry was not found");
                        return 1;
                    }
                }
                else {
                    // Delete selected item
                    Category& category = wallet.getCategory(categoryName);
                    if (category.empty()) {
                        return 1;
                    }

                    bool result = category.deleteItem(itemName);
                    if (!result) {
                        throw std::invalid_argument("such item was not found");
                        return 1;
                    }
                }
            }
            else {
                // Delete selected category
                if (entryCheck) {
                    throw std::invalid_argument("missing category, item or entry argument(s)");
                    return 1;
                }
                else {
                    bool result = wallet.deleteCategory(categoryName);
                    if (!result) {
                        throw std::invalid_argument("such category was not found");
                        return 1;
                    }
                }
            }
        }
        else {
            // Should I delete the wallet?
            if (itemCheck || entryCheck) {
                throw std::invalid_argument("missing category, item or entry argument(s)");
                return 1;
            }
        }

        wallet.save(db);

        break;
    }

    default:
        return 1;
    }
    
    return 0;
}

// Create a cxxopts instance. You do not need to modify this function.
cxxopts::Options App::cxxoptsSetup() {
    cxxopts::Options cxxopts("371pass", "Student ID: " + STUDENT_NUMBER + "\n");
    
    cxxopts.add_options()(
        "db", "Filename of the 371pass database",
        cxxopts::value<std::string>()->default_value("database.json"))(
    
        "action", "Action to take, can be: 'create', 'read', 'update', 'delete'.",
        cxxopts::value<std::string>())(
    
        "category",
        "Apply action to a category (e.g., if you want to add a category, set the"
        " action argument to 'add' and the category argument to your chosen"
        " category identifier).",
        cxxopts::value<std::string>())(
    
        "item",
        "Apply action to an item (e.g., if you want to add an item, set the "
        "action argument to 'add', the category argument to your chosen category "
        "identifier and the item argument to the item identifier).",
        cxxopts::value<std::string>())(
    
        "entry",
        "Apply action to an entry (e.g., if you want to add an entry, set the "
        "action argument to 'add', the category argument to your chosen category "
        "identifier, the item argument to your chosen item identifier, and the "
        "entry argument to the string 'key,value'). If there is no comma, an "
        "empty entry is inserted. If you are simply retrieving an entry, set the "
        "entry argument to the 'key'. If you are updating an entry key, use a : "
        "e.g., oldkey:newkey,newvalue.",
        cxxopts::value<std::string>())(
    
        "h,help", "Print usage.");
    
    return cxxopts;
}

// Transforms input argument into the action from the enum list
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
    if (args.count("action") == 0) {
        throw std::invalid_argument("invalid action argument(s)");
        return App::Action::NONE;
    }
    std::string input = args["action"].as<std::string>();

    // Transforms each character to lower case version
    std::transform(input.begin(), input.end(), input.begin(), [](char c) {
        return std::tolower(c); 
    });

    // Parse the map to find an action
    auto it = App::actionMap.find(input);
    if (it != App::actionMap.end()) {
        return (App::Action) it->second;
    }
    else {
        throw std::invalid_argument(input + "invalid action argument(s)");
        return App::Action::NONE;
    }
}

// Function, getJSON, that returns a std::string containing the
//  JSON representation of a Wallet object.
std::string App::getJSON(Wallet &wObj) { 
  return wObj.str();
}

// Function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Category in a Wallet object.
std::string App::getJSON(Wallet &wObj, const std::string &c) {
  auto cObj = wObj.getCategory(c);
  if (cObj.empty()) {
      return "";
  }

  return cObj.str();
}

// Function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Item in a Wallet object.
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i) {
   auto cObj = wObj.getCategory(c);
   if (cObj.empty()) {
       return "";
   }

   const auto iObj = cObj.getItem(i);
   if (iObj.empty()) {
       return "";
   }

   return iObj.str();
}

// Function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Entry in a Wallet object.
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i, const std::string &e) {
   auto cObj = wObj.getCategory(c);
   if (cObj.empty()) {
       return "";
   }

   auto iObj = cObj.getItem(i);
   if (iObj.empty()) {
       return "";
   }

   return iObj.getEntry(e);
}

// Split the string onto two strings using first found delimiter
std::pair <std::string, std::string> App::stringSplit(const std::string& string, const std::string& delimiter) {
    std::pair <std::string, std::string> result;
    
    unsigned int delPos = string.find(delimiter);

    if (delPos != std::string::npos) {
        result.first = string.substr(0, delPos);
        result.second = string.substr(++delPos, string.size());
        return result;
    }

    throw std::invalid_argument("entry argumens are not correct");
    return std::make_pair("","");
}