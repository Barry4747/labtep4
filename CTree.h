#ifndef CTREE_H
#define CTREE_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include "CResult.h"

using namespace std;

const char OPERATORS[] = { '+', '-', '/', '*' };
const string TRYGONOM[] = { "sin", "cos" };
const int DEFAULT_CHILDREN_COUNT_VALUE = 0;
const int DEFAULT_CHILDREN_COUNT_ARYTHM_OPERATOR = 2;
const int DEFAULT_CHILDREN_COUNT_VARIABLE = 0;
const int DEFAULT_CHILDREN_COUNT_TRYG = 1;

const size_t DEFAULT_OPERATOR_AMOUNT = 4;

enum DataType {
	EMPTY,
	VALUE,
	ARYTHM_OPERATOR,
	VARIABLE,
	TRYG
};

struct CNode {
	DataType data;
	union {
		int i_val;
		char ch_val;
	};
	string s_val;

	int i_children_amount;

	CNode* children;
	CNode* parent;

	CNode();
	CNode(int val);
	CNode(string& val);
	CNode(char val, DataType type);
	CNode(const CNode& other);
	~CNode();

	static CNode* deepCopy(const CNode* original);
};

class CTree {
public:
	CTree();
	CTree(const CTree& other_tree);
	~CTree();

	void operator=(const CTree& pNewTree);
	CTree operator+(const CTree& pNewTree);

	bool enter(string str);
	CResult<void, CError> crEnter(string str);

	bool insert(int val);
	bool insert(char val);
	bool insert(string val);

	string inOrder();

	int calculate(int values[]);

	void printVariables();

	int cntgt(int val);
private:
	vector<string> s_variables;
	CNode* root;
	CNode* parsed;

	CNode* deepCopy(const CNode* original);

	bool parseData(string str);
	void removeInvalidChars(string& str);

	CNode* findLeaf(CNode* node);

	template<typename T>
	bool insertCommon(T val);

	void inOrderRecursion(const CNode& node, string& result);
	int calculateRecursion(const CNode& currentNode, int variables[]);

	int cntgtRecursion(int val, int ans, CNode& node);

	bool isFull();

};

#endif 