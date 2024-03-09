#pragma once
#include "String.h"

enum NodeType
{
	EMPTY,
	NUMBER,
	OPERATOR,
	U_OPERATOR,
};


enum NodeFlags
{
	NONE = 0,
	REQUIRES_CHILD = (1<<0),
};

class Node
{
public:

	NodeType type{ EMPTY };
	int flags{ NONE };

	String raw{};

	Node* parent{};
	Node* left{};
	Node* right{};

	virtual void save(String input)
	{
		raw = input;
	}
	virtual int evaluate(bool& error) = 0;

	virtual void sort(Node*& head) {};

	virtual void insert(Node* newNode, Node*& head)
	{
		
		if (left == nullptr)
		{
			parent = newNode;
			newNode->left = head;
			head = newNode;
			return;
		}
		if (right == nullptr)
		{
			right = newNode;
			newNode->parent = head;
			sort(head);
			return;
		}
		if (right->flags &= REQUIRES_CHILD)
		{
			right->left = newNode;
			newNode->parent = right;
			return;
		}
		parent = newNode;
		newNode->left = head;
		head = newNode;
	}
};


class NumberNode : public Node
{
public:

	int val{};

	void save(String input) override
	{
		Node::save(input);
		val = input;
		type = NUMBER;
	}

	int evaluate(bool& error) override
	{
		if (left != nullptr || right != nullptr) //Numbers should not be parents. 
		{
			std::cout << "Syntax Error.\n";
			error = true;
			return 0;
		}
		return val;
	}
};

class B_OperatorNode : public Node
{
public:
	enum Operator
	{
		Operator_Add,
		Operator_Sub,
		Operator_Mult,
		Operator_Div,
		Operator_LShift,
		Operator_RShift,
	};
	Operator op;

	void save(String input) override
	{
		Node::save(input);
		switch (input[0])
		{
		case '+': 
			op = Operator_Add;
			break;
		case '-': 
			op = Operator_Sub;
			break;
		case '*': 
			op = Operator_Mult;
			break;
		case '/': 
			op = Operator_Div;
			break;
		}
		if (input == "<<")
		{
			op = Operator_LShift;
		}
		if (input == ">>")
		{
			op = Operator_RShift;
		}
		type = OPERATOR;
	}

	int evaluate(bool& error) override
	{
		if (left == nullptr || right == nullptr)
		{
			std::cout << "Syntax Error\n";
			error = true;
			return 0;
		}
		int lval{};
		int rval{};

		if (left->type == NUMBER)
		{
			NumberNode* lnum{ (NumberNode*)left };
			lval = lnum->val;
		} else {
			lval = left->evaluate(error);
			if (error) return 0;
		}

		if (right->type == NUMBER)
		{
			NumberNode* rnum{ (NumberNode*)right };
			rval = rnum->val;
		} else {
			rval = right->evaluate(error);
			if (error) return 0;
		}


		switch (op)
		{
		case Operator_Add:
			return lval + rval;
		case Operator_Sub:
			return lval - rval;
		case Operator_Mult:
			return lval * rval;
		case Operator_Div:
		{
			if (rval == 0)
			{
				error = true;
				std::cout << "Error: Division by 0.\n";
				return 0;
			}
			return lval / rval;
		}
		case Operator_LShift:
			return lval << rval;
		case Operator_RShift:
			return lval >> rval;
		}
		return 0;

	}


	void sort(Node*& head)
	{
		if (left->type == NUMBER && right->type == NUMBER) return;
		if (left->type == OPERATOR)
		{
			B_OperatorNode* lop{ (B_OperatorNode*)left };
			if (lop->op < op)
			{
				Node* newhead{ lop };
				left = newhead->right;
				left->parent = this;

				newhead->right = newhead->left;
				newhead->left = this;
				parent = newhead;
				head = newhead;
				head->parent = nullptr;
			}
		}
	}

};



class U_Operator : public Node
{
public:
	enum Operator
	{
		Operator_Not,
		Operator_Negative,
		Operator_Parenthesis,
	};
	Operator op;
	

	void save(String input) override
	{
		Node::save(input);
		switch (input[0])
		{
		case '~':
			op = Operator_Not;
			break;
		case '-':
			op = Operator_Negative;
			break;
		case '(':
			op = Operator_Parenthesis;
			break;
		}
		type = U_OPERATOR;
		flags |= REQUIRES_CHILD;
	}

	int evaluate(bool& error) override
	{
		if (left == nullptr)
		{
			std::cout << "Syntax Error.\n";
			error = true;
			return 0;
		}
		int val{};
		switch (left->type)
		{
		case NUMBER:
		{
			NumberNode* number{ (NumberNode*)left };
			val = number->val;
			break;
		}
		default: val = left->evaluate(error);
		}

		switch (op)
		{
		case Operator_Not: return ~val;
		case Operator_Negative: return -val;
		default: return val;
		}
		
	}

	void insert(Node* newNode, Node*& head) override
	{
		if (left != nullptr)
		{
			if (left->left == nullptr && left->flags & REQUIRES_CHILD)
			{
				left->left = newNode;
				newNode->parent = left;
				return;
			}

			parent = newNode;
			newNode->left = this;
			head = newNode;
			return;
		}

		newNode->parent = this;
		left = newNode;
	}
};

