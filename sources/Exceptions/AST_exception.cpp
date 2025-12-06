//
// Created by bowma on 11/25/2025.
//

#include <stdexcept>

class AST_exception final : public std::runtime_error {
    public:
    explicit AST_exception(const char* msg) : std::runtime_error(msg) {}

    explicit AST_exception(const std::string& msg) : std::runtime_error(msg) {}

    explicit AST_exception(const std::string& msg, const int line) : std::runtime_error(msg + std::to_string(line+1)) {}

    explicit AST_exception(const std::string& msg, const int line, const std::string& symbol) : std::runtime_error(msg + std::to_string(line+1) + " " + symbol) {}
};
