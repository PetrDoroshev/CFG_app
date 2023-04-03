#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

class Grammar{

private:
    std::unordered_map< char, std::vector<std::string> > rules;

public:

    Grammar(){ }
    void add_rule(std::string& rule);
    void generate_strings(int n);

};

void Grammar::add_rule(std::string& rule){
    char left = rule[0];
    std::string r = "";
    std::vector<std::string> right;

    for (int i = 2; i < rule.size(); i++){
        if (rule[i] != '|'){
            r += rule[i];
        }
        else {
            right.push_back(r);
            r = "";
        }
    }
    right.push_back(r);
    rules[left] = right;

}

void Grammar::generate_strings(int n){
    
}