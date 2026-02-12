#include <bits/stdc++.h>
using namespace std;

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    OPERATOR,
    PUNCTUATION,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;

    Token(TokenType t, const string& v) : type(t), value(v) {}
};

string tokenTypeToString(TokenType t) {
    switch (t) {
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::PUNCTUATION: return "PUNCTUATION";
        default: return "UNKNOWN";
    }
}

string fileReader(const string& fileName) {
    ifstream f(fileName);
    if (!f.is_open()) {
        cerr << "Error opening file: " << fileName << "\n";
        return "";
    }

    // Read the ENTIRE file 
    stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool isDigit(char c) { return (c >= '0' && c <= '9'); }
bool isWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

bool isKeyword(const string& word) {
    static const unordered_set<string> keywords = {
        "let", "where", "true", "false", "not", "fn", "ls", "gr", "ge",
        "aug", "le", "nil", "dummy", "or", "in", "eq", "ne", "and",
        "rec", "within"
    };
    return keywords.count(word) > 0;
}

bool isOperatorChar(char c) {
    static const unordered_set<char> ops = {'+', '-', '*', '/', '=', '.', };
    return ops.count(c) > 0;
}

bool isPunctuationChar(char c) {
    static const unordered_set<char> punc = {'(', ')', '{', '}', ';', ','};
    return punc.count(c) > 0;
}

// Reads letters/digits/underscore as a word (identifier/keyword)
string getNextWord(const string& input, int& pos) {
    int start = pos;
    while (pos < (int)input.size() &&
           (isAlpha(input[pos]) || isDigit(input[pos]) || input[pos] == '_')) {
        pos++;
    }
    return input.substr(start, pos - start);
}

// Reads a number (integer or float). Float if it contains one dot.
string getNextNumber(const string& input, int& pos) {
    int start = pos;

    while (pos < (int)input.size()) {
        if (isDigit(input[pos])) {
            pos++;
        } else {
            break;
        }
    }
    return input.substr(start, pos - start);
}

int main() {
    string input = fileReader("test1.txt");
    if (input.empty()) return 0;

    vector<Token> tokens;
    int pos = 0;

    while (pos < input.size()) {
        char c = input[pos];

        //Skip whitespace
        if (isWhitespace(c)) {
            pos++;
            continue;
        }

        //Keyword or identifier
        if (isAlpha(c)) {
            string word = getNextWord(input, pos); // pos moves forward!
            if (isKeyword(word)) tokens.emplace_back(TokenType::KEYWORD, word);
            else tokens.emplace_back(TokenType::IDENTIFIER, word);
            continue;
        }

        // Number 
        if (isDigit(c)) {
            string num = getNextNumber(input, pos); // pos moves forward!
            tokens.emplace_back(TokenType::INTEGER, num);
            continue;
        }

        //Operator
        if (isOperatorChar(c)) {
            if(c =='-' && input[pos+1] == '>'){
                tokens.emplace_back(TokenType::OPERATOR, "->");
                pos+=2;
                continue;
                }      
            tokens.emplace_back(TokenType::OPERATOR, string(1, c));
            pos++;
            continue;
         } 

        //Punctuation
        if (isPunctuationChar(c)) {
            tokens.emplace_back(TokenType::PUNCTUATION, string(1, c));
            pos++;
            continue;
        }

        // Unknown
        tokens.emplace_back(TokenType::UNKNOWN, string(1, c));
        pos++;
    }

    // Print tokens
    for (auto& t : tokens) {
        cout << "<" << tokenTypeToString(t.type)
             << " \""<< t.value << "\"" << ">" << "\n";
    }

    return 0;
}


