/*
 * IO related functions
 */

#include <vector>
#include <string>
using namespace std;


// tokenize input str, and store the tokens in a vector
void tokenizeLine(const string & str, vector<string> * tokens) {
    size_t prev=0, pos;
    // delimiters: space, comma, tab
    while ((pos = str.find_first_of(" ,\t", prev)) != string::npos) {
        if (pos > prev)
            tokens->push_back(str.substr(prev, pos-prev));
        prev = pos + 1;
    }
    if (prev < str.length())
        tokens->push_back(str.substr(prev, string::npos));
}


bool isDotData(const string & line){
    vector<string> tokens;
    tokenizeLine(line, &tokens);
    if (tokens.empty())
        return false;
    return (tokens[0] == ".data");
}

bool isDotText(const string & line){
    vector<string> tokens;
    tokenizeLine(line, &tokens);
    if (tokens.empty())
        return false;
    return (tokens[0] == ".text");
}
