#include<iostream>
#include<string>

namespace DesignPattern {

//抽象表达式
class Expression {
public:
    using ptr = std::shared_ptr<Expression>;

    virtual bool interpret(_STD string context) = 0;
};

//终结符表达式
class TerminalExpression :public Expression {
public:
    TerminalExpression(_STD string data) {
        this->m_data = data;
    }

    bool interpret(_STD string context) {
        if (context.find(m_data) != _STD string::npos) {
            return true;
        }
        return false;
    }

private:
    _STD string m_data;

}; // class TerminalExpression

//非终结符表达式
class OrExpression :public Expression {
private:
    Expression* expr_left = nullptr;
    Expression* expr_tight = nullptr;

public:
    OrExpression(Expression* expr_left, Expression* expr_tight) {
        this->expr_left = expr_left;
        this->expr_tight = expr_tight;
    }

    bool interpret(_STD string context) {
        return expr_left->interpret(context) || expr_tight->interpret(context);
    }
}; // class OrExpression

//非终结符表达式
class AndExpression :public Expression {
private:
    Expression* expr_left = nullptr;
    Expression* expr_tight = nullptr;

public:
    AndExpression(Expression* expr_left, Expression* expr_tight) {
        this->expr_left = expr_left;
        this->expr_tight = expr_tight;
    }

    bool interpret(_STD string context) {
        return expr_left->interpret(context) && expr_tight->interpret(context);
    }
}; // class AndExpression

} // namespace DesignPattern