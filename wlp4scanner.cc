#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <sstream>
#include <cstdlib>

void formatError(const std::string &line) {
    std::cerr << line << std::endl;
    std::cerr << "ERROR: Lexer error" << std::endl;
    std::exit(1);
}

std::map<std::string, std::string> keywordMap = {
    {"wain", "WAIN"}, {"int", "INT"}, {"if", "IF"}, {"else", "ELSE"},
    {"while", "WHILE"}, {"println", "PRINTLN"}, {"putchar", "PUTCHAR"},
    {"getchar", "GETCHAR"}, {"return", "RETURN"}, {"NULL", "NULL"},
    {"new", "NEW"}, {"delete", "DELETE"}
};

std::map<std::string, std::string> operatorMap = {
    {"(", "LPAREN"}, {")", "RPAREN"}, {"{", "LBRACE"}, {"}", "RBRACE"},
    {"=", "BECOMES"}, {"==", "EQ"}, {"!=", "NE"}, {"<", "LT"}, {">", "GT"},
    {"<=", "LE"}, {">=", "GE"}, {"+", "PLUS"}, {"-", "MINUS"}, {"*", "STAR"},
    {"/", "SLASH"}, {"%", "PCT"}, {",", "COMMA"}, {";", "SEMI"},
    {"[", "LBRACK"}, {"]", "RBRACK"}, {"&", "AMP"}
};

bool isValidIDStart(char c) {
    return std::isalpha(c);
}

bool isValidIDChar(char c) {
    return std::isalnum(c);
}

void scanLine(const std::string &line) {
    if (line.find('!') != std::string::npos) {
        formatError(line);
    }

    size_t i = 0;
    while (i < line.length()) {
        if (std::isspace(line[i])) {
            ++i;
            continue;
        }

        if (line[i] == '/' && i + 1 < line.length() && line[i + 1] == '/') {
            break;
        }

        if (isValidIDStart(line[i])) {
            std::string lexeme;
            while (i < line.length() && isValidIDChar(line[i])) {
                lexeme += line[i++];
            }
            if (keywordMap.count(lexeme)) {
                std::cout << keywordMap[lexeme] << " " << lexeme << std::endl;
            } else {
                std::cout << "ID " << lexeme << std::endl;
            }
            continue;
        }

        if (std::isdigit(line[i])) {
            std::string num;
            while (i < line.length() && std::isdigit(line[i])) {
                num += line[i++];
            }
            try {
                if (std::stoll(num) > 2147483647) {
                    formatError(line);
                }
            } catch (...) {
                formatError(line);
            }
            std::cout << "NUM " << num << std::endl;
            continue;
        }

        std::string op(1, line[i]);
        if (i + 1 < line.length()) {
            std::string twoCharOp = op + line[i + 1];
            if (operatorMap.count(twoCharOp)) {
                std::cout << operatorMap[twoCharOp] << " " << twoCharOp << std::endl;
                i += 2;
                continue;
            }
        }

        if (operatorMap.count(op)) {
            std::cout << operatorMap[op] << " " << op << std::endl;
            ++i;
            continue;
        }

        formatError(line);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string line;
    while (std::getline(std::cin, line)) {
        scanLine(line);
    }
    line.clear();
    std::quick_exit(0);
}

