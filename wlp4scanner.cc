#include <iostream>
#include <string>
#include <cctype>
#include <map>

void formatError(const std::string &message) {
    std::cerr << "ERROR" << std::endl;
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

void scanInput(const std::string &input) {
    size_t i = 0;

    while (i < input.length()) {
        if (std::isspace(input[i])) {
            ++i;
            continue;
        }

        if (input[i] == '/' && i + 1 < input.length() && input[i + 1] == '/') {
            while (i < input.length() && input[i] != '\n') {
                ++i;
            }
            continue;
        }

        if (isValidIDStart(input[i])) {
            std::string lexeme;
            while (i < input.length() && isValidIDChar(input[i])) {
                lexeme += input[i++];
            }
            if (keywordMap.count(lexeme)) {
                std::cout << keywordMap[lexeme] << " " << lexeme << std::endl;
            } else {
                std::cout << "ID " << lexeme << std::endl;
            }
            continue;
        }

        if (std::isdigit(input[i])) {
            std::string num;
            while (i < input.length() && std::isdigit(input[i])) {
                num += input[i++];
            }
            try {
                if (std::stoll(num) > 2147483647) {
                    formatError("NUM token out of range");
                }
            } catch (...) {
                formatError("Invalid NUM token");
            }
            std::cout << "NUM " << num << std::endl;
            continue;
        }

        std::string op(1, input[i]);
        if (i + 1 < input.length()) {
            std::string twoCharOp = op + input[i + 1];
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

        formatError("Invalid token");
    }
}

int main() {
    std::string input, line;
    while (std::getline(std::cin, line)) {
        input += line + "\n";
    }

    scanInput(input);
    return 0;
}
