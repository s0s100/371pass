// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 1916371
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// This file contains declarations for the helper
// functions used for initialising and running 371pass.
// -----------------------------------------------------

#ifndef _371PASS_H
#define _371PASS_H

#include <string>
#include <map>

#include "lib_cxxopts.hpp"
#include "wallet.h"

namespace App {
    const std::string STUDENT_NUMBER = "1916371";

    enum Action { CREATE, READ, UPDATE, DELETE, NONE };

    // String -> Enum transformation map
    static std::map<std::string, Action> actionMap = {
        {"create", Action::CREATE},
        {"read", Action::READ},
        {"update", Action::UPDATE},
        {"delete", Action::DELETE}        
    };

    int run(int argc, char *argv[]);

    cxxopts::Options cxxoptsSetup();

    App::Action parseActionArgument(cxxopts::ParseResult &args);

    std::string getJSON(Wallet &w);
    std::string getJSON(Wallet &w, const std::string &c);
    std::string getJSON(Wallet &w, const std::string &c, const std::string &i);
    std::string getJSON(Wallet &wObj, const std::string &c, const std::string &i,
        const std::string &e);

    std::pair <std::string, std::string> stringSplit(const std::string& string, const std::string& delimiter);
}

#endif // _371PASS_H