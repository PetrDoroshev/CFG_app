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
    int length;

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

    cout << "Преобразованная грамматика: " << endl;
    g.simplify();
    g.print_rules();

    cout << endl;
    cout << "Введите максимальную длину цепочки: ";
    cin >> length;
    cout << endl;


    vector<string> words = g.generate_words(length);
    for (auto& w: words){
        std::cout << w << std::endl;
    }


    return 0;   
}

bool is_correct_rule(string& r){
    return regex_match( r, regex("[A-Z]=.+(\\|.+)*") );
}

string trim(string& r){
    return regex_replace(r, regex("(\\s+)"), "");
}