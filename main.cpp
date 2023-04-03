#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include "grammar.h"

using namespace std;

bool is_correct_rule(string& r);
string trim(string& r);

int main(){

    Grammar g;
    string rule = " ";

    cout << "Введите правила грамматики (конец ввода - пустая строка): " << std::endl;

    getline(cin, rule);
    while (rule.compare("\0")){
        
        rule = trim(rule);

        if (is_correct_rule(rule)){
            g.add_rule(rule);
        }
        else {
            std::cout << "Некорректная запись правлиа грамматкики !" << std::endl;
        }
        getline(cin, rule);
    }

    return 0;
}

bool is_correct_rule(string& r){
    return regex_match( r, regex("[A-Z]=[A-Za-z0-9]+(\\|[A-Za-z0-9]+)*") );
}

string trim(string& r){
    return regex_replace(r, regex("(\\s+)"), "");
}