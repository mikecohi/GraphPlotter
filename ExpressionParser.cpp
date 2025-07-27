#include "ExpressionParser.h"
#include "ExpressionNode.h"

#include <cctype>
#include <stdexcept>
#include <set>
#include <stack>
#include <cmath>
#include <cstdlib>

// Returns the precedence level of an operator.
// Higher value means higher precedence.
int ExpressionParser::getPrecedence(const std::string& op) {
    if (op == "^") return 4;
    if (op == "*" || op == "/") return 3;
    if (op == "+" || op == "-") return 2;
    return 0;
}

// Checks if an operator is left-associative.
// Only the power operator (^) is right-associative.
bool ExpressionParser::isLeftAssociative(const std::string& op) {
    return op != "^";
}

// Returns true if a token is a supported operator.
bool ExpressionParser::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

// Returns true if a token is a supported function name.
bool ExpressionParser::isFunction(const std::string& token) {
    static const std::set<std::string> funcs = { "sin", "cos", "tan", "log", "exp", "sqrt", "abs" };
    return funcs.count(token);
}

// Returns true if a token is a valid number (float).
bool ExpressionParser::isNumber(const std::string& token) {
    if (token.empty()) return false;
    char* end;
    std::strtof(token.c_str(), &end);
    return *end == '\0';
}

// Converts a raw expression string into a list of tokens (numbers, operators, variables, functions, etc.)
std::vector<std::string> ExpressionParser::tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string curr;

    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        if (std::isspace(c)) continue;

        // Number or decimal point
        if (std::isdigit(c) || c == '.') {
            curr += c;
        }
        else {
            // Push current number token
            if (!curr.empty()) {
                tokens.push_back(curr);
                curr.clear();
            }

            // Function or variable name
            if (std::isalpha(c)) {
                std::string func;
                while (i < expr.size() && std::isalpha(expr[i])) func += expr[i++];
                --i;
                tokens.push_back(func);
            }
            // Operator or parentheses
            else if (c == '(' || c == ')' || isOperator(std::string(1, c))) {
                tokens.push_back(std::string(1, c));
            }
            // Variable 'x'
            else if (c == 'x') {
                tokens.push_back("x");
            }
        }
    }

    // Push the final token if any
    if (!curr.empty()) tokens.push_back(curr);
    return tokens;
}

// Converts infix tokens to postfix notation using the Shunting Yard algorithm.
std::vector<std::string> ExpressionParser::toRPN(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> ops;

    for (const std::string& token : tokens) {
        if (isNumber(token) || token == "x") {
            // Operand: directly add to output
            output.push_back(token);
        }
        else if (isFunction(token)) {
            // Function: push to operator stack
            ops.push(token);
        }
        else if (isOperator(token)) {
            // Operator: pop operators/functions from stack with higher or equal precedence
            while (!ops.empty() && (
                isFunction(ops.top()) ||
                (isOperator(ops.top()) && (
                    (isLeftAssociative(token) && getPrecedence(token) <= getPrecedence(ops.top())) ||
                    (!isLeftAssociative(token) && getPrecedence(token) < getPrecedence(ops.top()))
                    ))
                )) {
                output.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
        else if (token == "(") {
            ops.push(token);
        }
        else if (token == ")") {
            // Pop operators until matching '('
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }

            if (ops.empty()) throw std::runtime_error("Mismatched parentheses");

            ops.pop(); // Remove '('

            // If function is on top of stack, pop it too
            if (!ops.empty() && isFunction(ops.top())) {
                output.push_back(ops.top());
                ops.pop();
            }
        }
    }

    // Pop any remaining operators
    while (!ops.empty()) {
        if (ops.top() == "(" || ops.top() == ")")
            throw std::runtime_error("Mismatched parentheses");

        output.push_back(ops.top());
        ops.pop();
    }

    return output;
}

// Builds an abstract syntax tree (AST) from postfix (RPN) tokens.
std::unique_ptr<ExpressionNode> ExpressionParser::buildAST(const std::vector<std::string>& rpn) {
    std::stack<std::unique_ptr<ExpressionNode>> stk;

    for (const std::string& token : rpn) {
        if (isNumber(token)) {
            stk.push(std::make_unique<ConstantNode>(std::stof(token)));
        }
        else if (token == "x") {
            stk.push(std::make_unique<VariableNode>());
        }
        else if (isOperator(token)) {
            if (stk.size() < 2) throw std::runtime_error("Missing operand for operator");
            auto right = std::move(stk.top()); stk.pop();
            auto left = std::move(stk.top()); stk.pop();
            stk.push(std::make_unique<BinaryOpNode>(token[0], std::move(left), std::move(right)));
        }
        else if (isFunction(token)) {
            if (stk.empty()) throw std::runtime_error("Missing operand for function");
            auto operand = std::move(stk.top()); stk.pop();
            stk.push(std::make_unique<UnaryFuncNode>(token, std::move(operand)));
        }
    }

    if (stk.size() != 1) throw std::runtime_error("Invalid expression");
    return std::move(stk.top());
}

// Parses a string expression into an ExpressionNode tree (AST).
std::unique_ptr<ExpressionNode> ExpressionParser::parse(const std::string& expression) {
    auto tokens = tokenize(expression);
    auto rpn = toRPN(tokens);
    return buildAST(rpn);
}
