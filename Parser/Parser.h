#pragma once
#include <iostream>
#include <iomanip>
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


	void print_tree()
	{
		if (head == nullptr) return;
		Node* current{ head };
		size_t height{};
		find_height(head, height, 1);

		size_t rung{ height };
		std::cout << std::setw((height + 1)*2) << " " << (char*)head->raw << "\n";
		_print(head, 0, height);

	}

	void find_height(Node* current, size_t& max, size_t height)
	{
		if (current == nullptr) return;

		find_height(current->left, max, height + 1);
		if (height > max) max = height;
		find_height(current->right, max, height + 1);
	}

	void _print(Node* current, size_t height, size_t max)
	{
		if (current == nullptr) return;
		if (current->left != nullptr)
		{
			std::cout << std::setw((max - height)*2) << " " << (char*)current->left->raw;
			if (current->right == nullptr) std::cout << "\n";
		}
		if (current->right != nullptr)
		{
			std::cout << " " << (char*)current->right->raw << "\n";
		}
		_print(current->left, height + 1, max);
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
				String raw{""};
				size_t nested{};
				for (; str[i] != '\0'; ++i)
				{
					if (str[i] == '(') nested++;
					if (str[i] == ')') nested--;
					raw += str[i];

					if (nested == 0 && str[i + 1] == ')') break;
				}
				Node* inside{ parse_string(raw) };
				if (inside == nullptr) return newNode;
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