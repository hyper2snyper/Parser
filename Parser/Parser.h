#pragma once
#include <iostream>
#include "String.h"
#include "Node.h"

class Parser 
{
	Node* head{};
public:

	~Parser()
	{
		Node* current{ head };
		while (current != nullptr)
		{
			if (current->left == nullptr && current->right == nullptr)
			{
				Node* temp{current};
				current = current->parent;
				if (current != nullptr && current->left == temp)
				{
					current->left = nullptr;
				}
				if (current != nullptr && current->right == temp)
				{
					current->right = nullptr;
				}
				delete temp;
				continue;
			}
			if (current->left != nullptr)
			{
				current = current->left;
				continue;
			}
			if (current->right != nullptr)
			{
				current = current->right;
				continue;
			}
		}
	}

	void parse(const char* c)
	{
		String str{ c };
		head = parse_string(str);
	}

	static Node* parse_string(String str)
	{
		Node* head{};
		for (size_t i{}; str[i] != '\0';)
		{
			if (str[i] == ' ')
			{
				i++;
				continue;
			}
			Node* newNode = createNode(i, str);
			if (newNode == nullptr)
			{
				i++;
				continue;
			}
			if (head == nullptr)
			{
				head = newNode;
				continue;
			}
			head->insert(newNode, head);
		}
		return head;
	}

	void evaluate()
	{
		if (head == nullptr)
		{
			std::cout << "No Input\n";
			return;
		}
		bool error{};
		int out{ head->evaluate(error) };
		if (error) return;
		std::cout << out << "\n\n";
	}

	static Node* createNode(size_t& i, String& str)
	{
		Node* newNode{};
		if (str[i] >= '0' && str[i] <= '9')
		{
			newNode = new NumberNode();
			String raw{ str[i] };
			i++;
			for (; str[i] >= '0' && str[i] <= '9'; ++i)
			{
				raw += str[i];
			}
			newNode->save(raw);
			return newNode;
		}

		if (str[i] == '~' || str[i] == '(' || (str[i] == '-' && (str[i + 1] >= '0' && str[i + 1] <= '9' || str[i+1] == '(')))
		{
			newNode = new U_Operator();
			newNode->save({ str[i] });
			i++;
			
			if (str[i - 1] == '(')
			{
				String raw{ str[i++] };
				for (; str[i] != ')'; ++i)
				{
					raw += str[i];
				}
				Node* inside{ parse_string(raw) };
				newNode->left = inside;
				inside->parent = newNode;
				i++;
			}

			return newNode;
		}


		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || (str[i] == '<' && str[i+1] == '<') || (str[i] == '>' && str[i + 1] == '>'))
		{
			newNode = new B_OperatorNode();

			String raw{ str[i] };
			if (str[i] == '<' || str[i] == '>')
			{
				raw += str[i + 1];
				i++;
			}
			newNode->save(raw);
			i++;
			return newNode;
		}

		return newNode;
	}
};