#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <random>
#include <iterator>
#include <algorithm>
#include <queue>


class Grammar {

private:
    std::unordered_map< char, std::vector<std::string> > rules;
    char start_char;
    int rules_count = 0;
    
    bool is_terminal(char c);
    void remove_non_productive_chars();
    void remove_unreachable_chars();


public:

    Grammar(){ }
    void add_rule(std::string& rule);
    void set_start_char(char sc);
    std::vector<std::string> generate_words(int length);
    void simplify();
    void print_rules();
};

void Grammar::print_rules(){
    for (auto& rule: rules) {
        std::cout << rule.first  << " = ";
        for (int i = 0; i < rule.second.size(); i++) {
            std::cout << rule.second[i];
            if (i != rule.second.size() - 1)
                std::cout << " | ";
        }
        std::cout << std::endl;
    }

};

void Grammar::remove_non_productive_chars() { 

    char nonterm_lst[rules_count];

    bool is_generating[26] = { false };
    int counter[rules_count] = {0};
    std::vector<std::vector<int>> concerned_rules(26);
    std::queue<char> q;

    int rule_num = 0;

    for (auto& rule: rules) {
        for (int i = 0; i < rule.second.size(); i++) {
            nonterm_lst[rule_num] = rule.first;
            for (auto& c: rule.second[i]){
                if (!is_terminal(c)){
                    counter[rule_num]++;
                    concerned_rules[c - 'A'].push_back(rule_num);
                }
            }
            if (counter[rule_num] == 0 && !is_generating[rule.first - 'A']) {
                q.push(rule.first);
                is_generating[rule.first - 'A'] = true;
            }
            rule_num++;
        }
    }

    while (!q.empty()) { 

        for (auto& rule_num: concerned_rules[q.front() - 'A']){
            counter[rule_num]--; 

            if (counter[rule_num] == 0 && !is_generating[nonterm_lst[rule_num] - 'A']) {
                q.push(nonterm_lst[rule_num]);
                is_generating[nonterm_lst[rule_num] - 'A'] = true;
            }
        }
        q.pop();
    }  

    for (auto it = rules.begin(); it != rules.end();) {
        if (!is_generating[it->first - 'A']) {
            it = rules.erase(it); 
            continue;
        }
      
        for (int i = 0; i < it->second.size(); ) {
            for (auto& c: it->second[i]){
                if (!is_terminal(c) & !is_generating[c - 'A']){
                    it->second.erase(it->second.begin() + i);
                    i--;
                    break;
                }
            } 
            i++; 
        }
        it++;   
    }

}

void Grammar::remove_unreachable_chars() {
    
    bool is_reachable[26] = { false };
    std::queue<char> q;

    q.push(start_char);
    is_reachable[start_char - 'A'] = true;
    
    while (!q.empty()) {
        for (auto& rule: rules[q.front()]) {
            for (auto& c: rule) {
                if (!is_terminal(c) && !is_reachable[c - 'A']){
                    q.push(c);
                    is_reachable[c - 'A'] = true;
                    
                }
            }
        }
        q.pop();
    }

    std::cout << "Достижимые символы: " << std::endl;

    for (int i = 0; i < 26; i++){
        if (is_reachable[i])
            std::cout << (char)(i + 'A') << std::endl;
    }
    std::cout << std::endl;
}

void Grammar::simplify() {
    remove_non_productive_chars();
    remove_unreachable_chars();
}

void Grammar::set_start_char(char sc){
    start_char = sc;
}

void Grammar::add_rule(std::string& rule) {
    
    if (!start_char)
        set_start_char(rule[0]);

    char left = rule[0];
    std::string r = "";

    for (int i = 2; i < rule.size(); i++){

        if (rule[i] == '\\') {
            if (i != rule.size() - 1) {
                if (rule[i + 1] == 's')
                    r += ' ';
                i++;    
            }
        }
        else if (rule[i] == '|'){
            rules[left].push_back(r);
            rules_count++;
            r = "";
        }
        else {
            r += rule[i];
        }
    }

    rules[left].push_back(r);
    rules_count++;

}

std::string select_randomly(std::vector<std::string>& v) {
    std::vector<std::string> out;
    std::sample(v.begin(), v.end(), std::back_inserter(out), 1, std::mt19937{std::random_device{}()});
    return out[0];
}

bool Grammar::is_terminal(char c){
    return !(c >= 'A' && c <= 'Z');
}

std::vector<std::string> Grammar::generate_words(int lenght){
    std::vector<std::string> words;
    std::vector<std::string> stack = {std::string(1, start_char)};
    bool non_terminal_found;
    char non_terminal;
    int i;

    while (stack.size() > 0) {

        std::string str = stack.back();
        stack.pop_back();
        non_terminal_found = false;

        if (str.size() <= lenght) {

            for (i = 0; i < str.size(); i++){
                if (!is_terminal(str[i])) {
                    non_terminal = str[i];
                    non_terminal_found = true;
                    break;
                }
            }  

            if (!non_terminal_found) {
                words.push_back(str);
            }
            else {

                for (std::string& rule: rules[non_terminal]) {
                    std::string n_str = str;
                    stack.push_back(n_str.replace(i, 1, rule));
                }
            }
        }  
    }

    return words;
}


