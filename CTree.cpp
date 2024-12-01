#include "CTree.h"


CNode::CNode() {
	i_children_amount = 0;
	data = EMPTY;
	children = NULL;
	parent = NULL;
}
CNode::CNode(int val) {
	data = VALUE;
	i_val = val;
	i_children_amount = DEFAULT_CHILDREN_COUNT_VALUE;
	children = new CNode[i_children_amount];

	parent = NULL;
}
CNode::CNode(string& val) {
	data = VARIABLE;
	s_val = val;
	i_children_amount = DEFAULT_CHILDREN_COUNT_VARIABLE;
	children = new CNode[i_children_amount];

	parent = NULL;
}
CNode::CNode(char val, DataType type) {
	data = type;
	ch_val = val;
	if (data == TRYG)
	{
		i_children_amount = DEFAULT_CHILDREN_COUNT_TRYG;
		children = new CNode[i_children_amount];
	}
	else
	{
		i_children_amount = DEFAULT_CHILDREN_COUNT_ARYTHM_OPERATOR;
		children = new CNode[i_children_amount];
	}
	parent = NULL;
}
CNode::CNode(const CNode& other) {
	data = other.data;

	switch (data) {
	case VALUE:
		i_val = other.i_val;
		break;
	case VARIABLE:
		s_val = other.s_val;
		break;
	case TRYG:
		ch_val = other.ch_val;
		break;
	case ARYTHM_OPERATOR:
		ch_val = other.ch_val;
		break;
	case EMPTY:
	default:
		break;
	}

	i_children_amount = other.i_children_amount;
	children = new CNode[i_children_amount];
	for (size_t i = 0; i < i_children_amount; ++i) {
		if (other.children[i].data != EMPTY) {
			children[i] = *deepCopy(&other.children[i]);
			children[i].parent = this;
		}
		else {
			children[i] = CNode();
		}
	}

	parent = NULL;
}

CNode::~CNode() {
	if (children != NULL) {
		delete[] children;
		children = NULL;
	}
}




CTree::CTree() {
	root = NULL;
	parsed = NULL;
}

CTree::CTree(const CTree& other) {
	if (other.root != NULL) {
		root = deepCopy(other.root);
		parsed = root;
	}
	else {
		root = NULL;
		parsed = NULL;
	}

	s_variables = other.s_variables;
}

CTree::~CTree() {
	if (root != NULL) {
		delete root;
	}
}



void CTree::operator=(const CTree& pNewTree) {

	if (this == NULL)
	{
		*this = CTree();
	}
	if (this == &pNewTree) {
		return;
	}

	if (root != NULL) {
		delete root;
	}

	if (pNewTree.root != NULL) {
		root = deepCopy(pNewTree.root);
		parsed = root;
	}
	else {
		root = NULL;
		parsed = NULL;
	}

	s_variables = pNewTree.s_variables;

}
CNode* CTree::findLeaf(CNode* node) {
	if (node == NULL) return NULL;

	if (node->i_children_amount == 0) {
		return node;
	}

	for (size_t i = 0; i < node->i_children_amount; ++i) {
		CNode* childLeaf = findLeaf(&node->children[i]);
		if (childLeaf != NULL) {
			return childLeaf;
		}
	}
	return NULL;
}

CTree CTree::operator+(const CTree& pNewTree) {
	CTree new_tree;

	if (root == NULL && pNewTree.root == NULL) {
		return new_tree;
	}

	if (root == NULL || root->i_children_amount == 0) {
		new_tree.root = deepCopy(pNewTree.root);
		new_tree.parsed = new_tree.root;
		return new_tree;
	}

	if (pNewTree.root == NULL) {
		new_tree.root = deepCopy(root);
		new_tree.parsed = new_tree.root;
		return new_tree;
	}

	new_tree.root = deepCopy(root);
	new_tree.parsed = new_tree.root;

	CNode* leaf = findLeaf(new_tree.root);

	if (leaf != NULL) {
		CNode* new_root = deepCopy(pNewTree.root);

		if (leaf->parent != NULL) {
			CNode* parent = leaf->parent;

			for (size_t i = 0; i < parent->i_children_amount; ++i) {
				if (&parent->children[i] == leaf) {
					delete[] leaf->children;
					parent->children[i] = *new_root;
					new_root->parent = parent;
					break;
				}
			}
		}
		else {
			new_tree.root = new_root;
			new_tree.parsed = new_root;
			new_root->parent = NULL;
		}
	}

	return new_tree;
}







CNode* CNode::deepCopy(const CNode* original) {
	if (original == NULL) {
		return NULL;
	}

	CNode* copy = new CNode(*original);

	if (original->i_children_amount > 0) {
		copy->children = new CNode[original->i_children_amount];

		for (size_t i = 0; i < original->i_children_amount; ++i) {
			if (original->children[i].data != EMPTY) {
				copy->children[i] = *deepCopy(&original->children[i]);
				copy->children[i].parent = copy;
			}
		}
	}
	else {
		copy->children = NULL;
	}

	return copy;
}

CNode* CTree::deepCopy(const CNode* original) {
	return CNode::deepCopy(original);
}



bool CTree::enter(string str) {

	removeInvalidChars(str);

	parsed = root;

	int i_pointer = 0;

	while (i_pointer != string::npos) {
		size_t next_space = str.find(" ", i_pointer);
		string substring;

		if (next_space != string::npos) {
			substring = str.substr(i_pointer, next_space - i_pointer);
			i_pointer = next_space + 1;
		}
		else {
			substring = str.substr(i_pointer);
			i_pointer = string::npos;
		}

		parseData(substring);
	}
	return !insertCommon(1);
}

void CTree::removeInvalidChars(std::string& str) {
	size_t j = 0;
	bool b_in_space = false;

	size_t start = 0;
	while (start < str.size() && std::isspace(str[start])) {
		++start;
	}

	for (size_t i = start; i < str.size(); ++i) {
		char c = str[i];

		if (std::isalnum(c) || c == '+' || c == '-' || c == '*' || c == '/') {
			str[j++] = c;
			b_in_space = false;
		}
		else if (std::isspace(c)) {
			if (!b_in_space) {
				str[j++] = ' ';
				b_in_space = true;
			}
		}
	}

	if (j > 0 && std::isspace(str[j - 1])) {
		--j;
	}

	str.resize(j);
}


bool CTree::parseData(string str) {
	str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());

	bool b_helper = false;

	for (size_t i = 0; i < DEFAULT_OPERATOR_AMOUNT; i++)
	{
		if (str.find(OPERATORS[i]) != -1)
		{
			b_helper = true;
			break;
		}
	}

	//operator
	if (b_helper) {
		if (str.length() != 1)
		{
			return false;
		}

		return insert(str[0]);
	}

	//sin / cos
	if (str == TRYGONOM[0] || str == TRYGONOM[1])
	{
		return insert(str[0]);
	}

	b_helper = true;
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i]))
		{
			b_helper = false;
			break;
		}
	}

	if (b_helper) {
		int val;
		stringstream(str) >> val;
		return insert(val);
	}
	else {
		return insert(str);
	}
}


bool CTree::insert(int val) {
	if (insertCommon(val))
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool CTree::insert(char val) {

	if (root == NULL)
	{
		if (val == 's' || val == 'c')
		{
			root = new CNode(val, TRYG);
			parsed = root;
		}
		else
		{
			root = new CNode(val, ARYTHM_OPERATOR);
			parsed = root;
		}
		return true;
	}

	int i_pointer = 0;

	while (i_pointer < parsed->i_children_amount && parsed->children[i_pointer].data != EMPTY) {
		i_pointer++;
	}
	if (i_pointer == parsed->i_children_amount)
	{
		if (parsed->parent != NULL)
		{
			parsed = parsed->parent;
			return insert(val);
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (val == 's' || val == 'c')
		{
			CNode* new_node = new CNode(val, TRYG);
			parsed->children[i_pointer] = *new_node;
			new_node->parent = parsed;
			parsed = new_node;
		}
		else
		{
			CNode* new_node = new CNode(val, ARYTHM_OPERATOR);
			parsed->children[i_pointer] = *new_node;
			new_node->parent = parsed;
			parsed = new_node;
		}
	}
	return true;
}
bool CTree::insert(string val) {

	if (insertCommon(val))
	{
		if (std::find(s_variables.begin(), s_variables.end(), val) == s_variables.end()) {
			s_variables.push_back(val);
		}
		return true;
	}
	return false;
}
template<typename T>
bool CTree::insertCommon(T val) {
	if (root == NULL)
	{
		root = new CNode(val);
		parsed = root;
		return true;
	}
	if (parsed->data == ARYTHM_OPERATOR || parsed->data == TRYG || parsed == root)
	{
		int i_pointer = 0;

		while (i_pointer < parsed->i_children_amount && parsed->children[i_pointer].data != EMPTY) {
			i_pointer++;
		}

		if (i_pointer == parsed->i_children_amount)
		{
			if (parsed->parent != NULL)
			{
				parsed = parsed->parent;
				return insert(val);
			}
			else
			{
				return false;
			}
		}
		else
		{
			CNode* new_node = new CNode(val);
			parsed->children[i_pointer] = *new_node;
			new_node->parent = parsed;
		}
		return true;
	}
	return false;
}


string CTree::inOrder() {
	if (this == NULL)
	{
		return "";
	}
	string result;
	inOrderRecursion(*root, result);
	return result;
}

void CTree::inOrderRecursion(const CNode& node, string& result) {
	switch (node.data) {
	case ARYTHM_OPERATOR:
		result += node.ch_val;
		result += " ";
		break;
	case VALUE:
		result += to_string(node.i_val);
		result += " ";
		break;
	case VARIABLE:
		result += node.s_val;
		result += " ";
		if (find(s_variables.begin(), s_variables.end(), node.s_val) == s_variables.end()) {
			s_variables.push_back(node.s_val);
		}
		break;
	case TRYG:
		if (node.ch_val == 's') {
			result += "sin ";
		}
		else {
			result += "cos ";
		}
		break;
	default:
		break;
	}

	for (size_t i = 0; i < node.i_children_amount; i++) {
		if (node.children[i].data != EMPTY) {
			inOrderRecursion(node.children[i], result);
		}
	}
}


int CTree::calculate(int values[]) {
	int len = 0;

	return calculateRecursion(*root, values);
}

int CTree::calculateRecursion(const CNode& currentNode, int variables[]) {
	if (currentNode.data == TRYG)
	{
		switch (currentNode.ch_val)
		{
		case 's': return sin(calculateRecursion(currentNode.children[0], variables)); break;
		case 'c': return cos(calculateRecursion(currentNode.children[0], variables)); break;
		default:
			break;
		}
	}
	else if (currentNode.data == ARYTHM_OPERATOR)
	{
		switch (currentNode.ch_val)
		{
		case '+': return calculateRecursion(currentNode.children[0], variables) + calculateRecursion(currentNode.children[1], variables); break;
		case '-': return calculateRecursion(currentNode.children[0], variables) - calculateRecursion(currentNode.children[1], variables); break;
		case '*': return calculateRecursion(currentNode.children[0], variables) * calculateRecursion(currentNode.children[1], variables); break;
		case '/': return calculateRecursion(currentNode.children[0], variables) / calculateRecursion(currentNode.children[1], variables); break;
		default:
			break;
		}
	}
	else
	{
		if (currentNode.data == VALUE)
		{
			return currentNode.i_val;
		}
		else
		{
			int i_position = 0;
			for (int i = 0; i < s_variables.size(); i++)
			{
				if (s_variables[i] == currentNode.s_val)
				{
					break;
				}
				i_position++;
			}

			return variables[i_position];
		}
	}
}

void CTree::printVariables() {
	for (int i = 0; i < s_variables.size(); i++)
	{
		cout << s_variables[i] << " ";
	}
	cout << endl;
}

int CTree::cntgt(int val) {
	return cntgtRecursion(val, 0, *root);
}

int CTree::cntgtRecursion(int val, int ans, CNode& node) {
	if (node.data == VALUE)
	{
		if (node.i_val > val)
		{
			return ans + 1;
		}
	}
	for (size_t i = 0; i < node.i_children_amount; i++)
	{
		if (node.children[i].data != EMPTY)
		{
			ans = cntgtRecursion(val, ans, node.children[i]);
		}
	}
	return ans;
}


CResult<void, CError> CTree::crEnter(string str) {
	vector<CError*> v_errors;

	int i_initial_len = str.length();
	removeInvalidChars(str);
	//sprawdzam czy podany string jest prawidlowy
	if (i_initial_len!=str.length()||str.length()==0)
	{
		v_errors.push_back(new CError("Incorrect input format!"));
	}

	parsed = root;

	int i_pointer = 0;

	while (i_pointer != string::npos) {
		size_t next_space = str.find(" ", i_pointer);
		string substring;

		if (next_space != string::npos) {
			substring = str.substr(i_pointer, next_space - i_pointer);
			i_pointer = next_space + 1;
		}
		else {
			substring = str.substr(i_pointer);
			i_pointer = string::npos;
		}
		if (!parseData(substring))
		{
			if (isFull())
			{
				v_errors.push_back(new CError("Couldn't add element to tree: '" + substring + "', tree is already full!"));
			}
			else
			{
				v_errors.push_back(new CError("Couldn't add element to tree: '"+substring+"'!"));
			}
		}
	}
	//sprawdzam czy drzewo jest zapelnione
	if (insertCommon(1))
	{
		v_errors.push_back(new CError("Equation incorrect | not enough values or variables!"));
	}
	if (v_errors.empty())
	{
		return CResult<void, CError>::cOk();
	}
	else
	{
		return CResult<void, CError>::cFail(v_errors);
	}

}


bool CTree::isFull() 
{
	if (parsed == root)
	{
		while (parsed->data == ARYTHM_OPERATOR || parsed->data == TRYG) {
			if (parsed->children[parsed->data==ARYTHM_OPERATOR ? 1 : 0].data == EMPTY)
			{
				return false;
			}
			*parsed = parsed->children[parsed->data == ARYTHM_OPERATOR ? 1 : 0];
		}
		parsed = root;
	}
	return true;
}