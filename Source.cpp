#include <iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<string>
using namespace std;

const int MAX = 100;

vector<string> keywords;
vector<string> dataTypes;
vector<string> constants;
vector<string> operators;
vector<string> identifiers;
vector<string> mInstruction;
vector<string> invalidToken;
bool isOperator(string stringFromFile)
{
	if (stringFromFile == "+" || stringFromFile == ";" || stringFromFile == "-" || stringFromFile == "" || stringFromFile == "/" || stringFromFile == "%" || stringFromFile == "++" || stringFromFile == "--" || stringFromFile == "/=" || stringFromFile == "=" || stringFromFile == "-=" || stringFromFile == "<-" || stringFromFile == ">=" || stringFromFile == "<=" || stringFromFile == ">" || stringFromFile == "<")
	{
		return true;
	}
	return false;
}
bool isIdentifier(string stringFromFile)
{
	if (stringFromFile[0] == '_' || (stringFromFile[0] >= 65 && stringFromFile[0] <= 90) || (stringFromFile[0] >= 97 && stringFromFile[0] <= 122))
	{
		for (int i = 0; i < stringFromFile.length(); i++)
		{
			string temp(1, stringFromFile[i]);
			if (stringFromFile[i] == '=' || stringFromFile[i] == '!' || stringFromFile[i] == '@' || stringFromFile[i] == '"' || stringFromFile[i] == '\'' || stringFromFile[i] == '#' || stringFromFile[i] == isOperator(temp))
			{
				return false;
			}
			else if (i == stringFromFile.length())
			{
				return true;
			}
		}
		return true;
	}
	return false;
}
bool isConstant(string stringFromFile)
{
	int i = 0;
	while (i < stringFromFile.length())
	{
		if (isdigit(stringFromFile[i]))
		{
			if (i == stringFromFile.length() - 1)
			{
				return true;
			}
		}
		i++;
	}
	return false;
}
bool isDataType(string pTok) {
	vector<string> dataTypes{ "doub", "integer", "lng", "charac", "flt", "boolean" };
	for (int i = 0; i < dataTypes.size(); i++)
	{
		if (pTok == dataTypes[i])
		{
			return true;
		}
	}
	return false;
}
bool isKeyWord(string pTok)
{
	vector<string> keywords{ "byself", "structure","def", "terminate", "el", "lng","elif",
		"flip", "cause", "remit" ,"return", "join", "constant", "cont", "since","jump", "execute", "whatif", "till", "clss", "incorrect", "correct"
	};
	for (int i = 0; i < keywords.size(); i++)
	{
		if (pTok == keywords[i])
		{
			return true;
		}
	}
	return false;
}
bool Check_DFA_VN(int current_state, int current_index, string token)
{
	if (current_state == 0 && (isalpha(token[current_index]) || token[current_index] == '_'))
	{
		current_state = 1;
		if (current_index + 1 < token.length())
		{
			return Check_DFA_VN(current_state, current_index + 1, token);
		}
		else
		{
			return true;
		}
	}
	else if (current_state == 1 && (isalpha(token[current_index]) || isdigit(token[current_index]) || token[current_index] == '_'))
	{
		current_state = 2;
		if (current_index + 1 < token.length())
		{
			return Check_DFA_VN(current_state, current_index + 1, token);
		}
		else
		{
			return true;
		}
	}
	else if (current_state == 2 && (isalpha(token[current_index]) || isdigit(token[current_index]) || token[current_index] == '_'))
	{
		current_state = 3;
		if (current_index + 1 < token.length())
		{
			return Check_DFA_VN(current_state, current_index + 1, token);
		}
		else
		{
			return true;
		}
	}
	else if (current_state == 3 && (isalpha(token[current_index]) || isdigit(token[current_index]) || token[current_index] == '_'))
	{
		current_state = 4;
		if (current_index + 1 < token.length())
		{
			return Check_DFA_VN(current_state, current_index + 1, token);
		}
		else
		{
			return true;
		}
	}
	else if (current_state == 4 && (isalpha(token[current_index]) || isdigit(token[current_index]) || token[current_index] == '_'))
	{
		current_state = 5;
		if (current_index + 1 < token.length() && token[current_index + 1] != NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void writeToFile()
{
	fstream fileObj;
	fileObj.open("file.txt", fstream::out);
	fileObj << "a = b + c ;";
	fileObj.close();
}
class Token {
	string token;
	bool validToken;
	bool isDeclared;
public:
	Token()
	{

	}
	Token(string token, bool validation)
	{
		this->token = token;
		this->validToken = validation;
	}
	string getToken()
	{
		return this->token;
	}
	bool getValidation()
	{
		return this->validToken;
	}
	bool getDeclaration() { return this->isDeclared; }
};
vector<Token> tokenizeInstruction(string mData) {
	vector<Token> tokens;
	string token = "";
	for (auto x : mData)
	{
		if (x == ' ')
		{
			Token t = Token(token, false);
			tokens.push_back(t);
			token = "";
		}
		else {
			token = token + x;
		}
	}
	Token t = Token(token, false);
	tokens.push_back(t);
	return tokens;
}

class Node {
	string identifier, scope, type;
	int lineNo;
	Node* next;

public:
	Node() {
		next = NULL;
	}
	Node(string key, string value, string type, int lineNo)
	{
		this->identifier = key;
		this->scope = value;
		this->type = type;
		this->lineNo = lineNo;
		next = NULL;
	}
	friend class SymbolTable;
};
class SymbolTable {
	Node* head[MAX];
public:
	SymbolTable() {
		for (int i = 0; i < MAX; i++)
			head[i] = NULL;
	}
	int hashf(string id); // hash function
	bool insert(string id, string scope, string Type, int lineno);
	string find(string id);
	bool deleteRecord(string id);
	bool modify(string id, string scope, string Type, int lineno);
	void show();
};

void SymbolTable::show()
{
	for (int i = 0; i < MAX; i++)
	{
		Node* start = head[i];
		while (start != NULL)
		{
			cout << "\nName  =  " << start->identifier << "   Scope  =  "
				<< start->scope << "   Type  =  " << start->type
				<< "   Line No  =  " << start->lineNo << "\n  ";
			start = start->next;
		}

	}
}
// Function to modify an identifier
bool SymbolTable::modify(string id, string s, string t, int l) {
	int index = hashf(id);
	Node* start = head[index];

	if (start == NULL)
		return "-1";

	while (start != NULL) {
		if (start->identifier == id) {
			start->scope = s;
			start->type = t;
			start->lineNo = l;
			return true;
		}
		start = start->next;
	}

	return false; // id not found 
}
// Function to delete an identifier
bool SymbolTable::deleteRecord(string id) {
	int index = hashf(id);
	Node* tmp = head[index];
	Node* par = head[index];

	// no identifier is present at that index
	if (tmp == NULL) {
		return false;
	}
	// only one identifier is present
	if (tmp->identifier == id && tmp->next == NULL) {
		tmp->next = NULL;
		delete tmp;
		return true;
	}

	while (tmp->identifier != id && tmp->next != NULL) {
		par = tmp;
		tmp = tmp->next;
	}
	if (tmp->identifier == id && tmp->next != NULL) {
		par->next = tmp->next;
		tmp->next = NULL;
		delete tmp;
		return true;
	}

	// delete at the end
	else {
		par->next = NULL;
		tmp->next = NULL;
		delete tmp;
		return true;
	}
	return false;
}

// Function to find an identifier
string SymbolTable::find(string id) {
	int index = hashf(id);
	Node* start = head[index];

	if (start == NULL)
		return "-1";
	 
	while (start != NULL) {

		if (start->identifier == id) {
			return start->scope;
		}

		start = start->next;
	}

	return "-1"; // not found
}
 
// Function to insert an identifier
bool SymbolTable::insert(string id, string scope, string Type, int lineno) {
	int index = hashf(id);
	Node* p = new Node(id, scope, Type, lineno);

	if (head[index] == NULL) {
		head[index] = p;
		return true;
	}

	else {
		Node* start = head[index];
		while (start->next != NULL)
			start = start->next;

		start->next = p;

		return true;
	}

	return false;
}

int SymbolTable::hashf(string id) {
	int asciiSum = 0;

	for (int i = 0; i < id.length(); i++) {
		asciiSum = asciiSum + id[i];
	}

	return (asciiSum % 100);
}


class TAC {
public:
	string operand1;
	string operand2;
	string opera;
	string result;
	void display()
	{
		if (opera != "=")
		{
			cout << result << "   =   " << operand1 << "  " << opera << "  " << operand2 << endl;
		}
		else
		{
			cout << result << "   =   " << operand1 << endl;
		}

	}
};
class Lexer {
	vector<string> data;
	vector<TAC> TAC_Instructions;
public:

	Lexer(vector<string> data)
	{
		this->data = data;
	}
	void scanningInstruction(string s)
	{
		//****LEXICAL ANALYSiS ( MAkING TOKENS AND LEXEMES ) ********//
		cout << "\t\tTHE LEXICAL ANAYLYSIS (SCANNING/PARSING)\n\n";
		vector<Token> insTok;
		cout << "\t\t The input string without free spaces and comments: ";
		cout << s << endl << endl;
		for (int i = 0; i < data.size(); i++) {
			insTok = tokenizeInstruction(data[i]);
			for (int i = 0; i < insTok.size(); i++) {
				string strTok = insTok[i].getToken();
				if (isKeyWord(strTok)) {
					keywords.push_back(strTok);
				}
				else if (isDataType(strTok)) {
					dataTypes.push_back(strTok);
				}
				else if (isOperator(strTok)) {
					operators.push_back(strTok);
				}
				else if (isConstant(strTok)) {
					constants.push_back(strTok);
				}
				else if (isIdentifier(strTok)) {
					identifiers.push_back(strTok);
				}
				else {
					invalidToken.push_back(strTok);
				}
			}
		}
		cout << "\t\tKEYWORDS: ";
		for (int i = 0; i < keywords.size(); i++) {
			cout << keywords[i] << " ";
		}
		cout << "\n\n\t\t DATATYPES: ";
		for (int i = 0; i < dataTypes.size(); i++) {
			cout << dataTypes[i] << " ";
		}
		cout << "\n\n\t\t OPERATORS: ";
		for (int i = 0; i < operators.size(); i++) {
			cout << operators[i] << " ";
		}
		cout << "\n\n\t\t IDENTIFIERS: ";
		for (int i = 0; i < identifiers.size(); i++) {
			cout << identifiers[i] << " ";
		}
		
	}
	void symbolTableCreation(vector<string>pInstruction) {
		//cout << "check";
		SymbolTable st;
		vector<Token> insTok;
		for (int i = 0; i < pInstruction.size(); i++){
			insTok = tokenizeInstruction(pInstruction[i]);
			for (int j = 0; j < insTok.size(); j++) {
				string s = insTok[j].getToken();
				if (isKeyWord(s)) {
					if (isIdentifier(insTok[j + 1].getToken())) {
						if (insTok[j + 2].getToken() == "{") {
							st.insert(insTok[j + 1].getToken(), "class", s, i + 1);
							j = j + 2;
						}
					}
				}
				if (isDataType(s)) {
					if (isIdentifier(insTok[j + 1].getToken())) {
						if (insTok[j + 2].getToken() == ";" ) {
							st.insert(insTok[j + 1].getToken(), "global", s, i + 1);
						}
					}
				}
				if (isKeyWord(s)) {
					if (isIdentifier(insTok[j + 1].getToken())) {
						if (insTok[j + 2].getToken() == "(" && insTok[j + 3].getToken() == ")") {
							st.insert(insTok[j + 1].getToken(), "Function", s, i + 1);
						}
					}
				}
				if (isKeyWord(s))
				{
					if (insTok[j+1].getToken() == "(")
					{
						int i = 2;
						while (insTok[i].getToken() != ")")
						{
							i++;
						}
						if (insTok[i].getToken() == ")")
						{
							if (insTok[i + 1].getToken() == "{")
							{
								if (isDataType(insTok[i + 2].getToken())) {
									if (isKeyWord(insTok[i + 2].getToken())) {
										st.insert(insTok[i + 2].getToken(), "local", insTok[i + 2].getToken(), 4);
									}
								}
							}
						}
					}
				}
				if (isKeyWord(s))
				{
					if (insTok[j + 1].getToken() == "(")
					{
						int i = 2;
						while (insTok[i].getToken() != ")")
						{
							i++;
						}
						if (insTok[i].getToken() == ")")
						{
							if (insTok[i + 1].getToken() == "{")
							{
								if (isDataType(insTok[i + 2].getToken())) {
									if (isKeyWord(insTok[i + 2].getToken())) {
										st.insert(insTok[i + 2].getToken(), "elif local", insTok[i + 2].getToken(), 4);
									}
								}
							}
						}
					}
				}
				
				if (isKeyWord(s)) {
					if (insTok[j + 1].getToken() == "(") {
						if (isDataType(insTok[j + 2].getToken())) {
							if (isIdentifier(insTok[j + 3].getToken())) {
								st.insert(insTok[j + 3].getToken(), "since loop variable", insTok[j + 2].getToken(), i + 1);
								int i = 4;
								while (insTok[i].getToken() != ")") {
									i++;
								}
								if (insTok[i].getToken() == ")") {
									if (insTok[i+1].getToken() == "{") {
										cout << "\n\ninsertion done\n\n";
									}
								}
							}
						}
			
					}
				}
				if (isKeyWord(s)) {
					if (insTok[j + 1].getToken() == "(") {
						int i = 2;
						while (insTok[i].getToken() != ")") {
							i++;
						}
						if (insTok[i].getToken() == ")") {
							if (insTok[i + 1].getToken() == "{") {
								cout << "\n\till loop checked\n\n";
								
							}
						}
					}
				}
				if (isKeyWord(s)) {
					if (insTok[j + 1].getToken() == "(") {
						int i = 2;
						while (insTok[i].getToken() != ")") {
							i++;
						}
						if (insTok[i].getToken() == ")") {
							if (insTok[i + 1].getToken() == "{") {
								cout << "\n\nSwitch statement\n\n";
							}
						}
					}
				}

			}
		}
		st.show();
	}

	int prec(string c)
	{
		if (c == "^")
			return 3;
		else if (c == "/" || c == "*")
			return 2;
		else if (c == "+" || c == "-")
			return 1;
		else if (c == "=")
			return 0;
		else
			return -1;
	}

	// The main function to convert infix expression
	// to postfix expression
	vector<string> infixToPostfix(vector<string> s)
	{
		stack <string> st;
		vector<string> postfix;
		for (int i = 0; i < s.size(); i++) {       //(a+b)+(c-d) 
			string c = s[i];
			if (isIdentifier(c))
				postfix.push_back(c);

			else if (c == "(")
				st.push("(");

			else if (c == ")") {
				while (st.top() != "(") {
					postfix.push_back(st.top());
					st.pop();
				}
				st.pop();
			}

			// If an operator is scanned
			else {
				while (!st.empty() && prec(s[i]) <= prec(st.top()))
				{
					if (c == "^" && st.top() == "^")
						break;
					else {
						postfix.push_back(st.top());
						st.pop();
					}
				}
				st.push(c);
			}
		}
		while (!st.empty()) {
			postfix.push_back(st.top());
			st.pop();
		}
		return postfix;
	}
	void extractTAC(vector<string> exp)
	{
		stack<string> s;
		for (auto x : exp)
			cout << x << " ";
		cout << endl;
		for (int i = 0; i < exp.size() - 1; i++)
		{
			if (isIdentifier(exp[i]))
			{
				s.push(exp[i]);
			}
			else if (isOperator(exp[i]))
			{
				TAC temp;
				if (!s.empty())
				{
					temp.operand1 = s.top();
					s.pop();
					temp.operand2 = s.top();
					s.pop();
					temp.opera = exp[i];
					string temp1 = "t";
					temp1 += to_string(TAC_Instructions.size());
					temp.result = temp1;
					s.push(temp1);
				}

				TAC_Instructions.push_back(temp);
			}
		}
	}

	void TAC_Insertion(vector<string> instructions)
	{
		for (int i = 0; i < instructions.size(); i++) {
			vector<Token> insTok = tokenizeInstruction(instructions[i]);
			vector<string> st;
			for (int j = 0; j < insTok.size(); j++)
			{
				st.push_back(insTok[j].getToken());
			}
			vector<string> infToPos = infixToPostfix(st);
			extractTAC(infToPos);
			
			for (int i = 0; i < TAC_Instructions.size(); i++)
			{
				TAC_Instructions[i].display();
			}
		}


	}

};

void readFromFile()
{
	fstream fileObj;
	bool comment = false, multiComment = false;
	string newString = "", temp = "";
	int i = 0;
	fileObj.open("file.txt", ios::in);
	if (fileObj.is_open())
	{
		while (!fileObj.eof())
		{
			multiComment = false;
			comment = false;
			getline(fileObj, temp);  //READ LINE BY LINE 
			//REMOVE COMMENTS
			if (temp[0] == '/') {
				if (temp[1] == '/') {
					comment = true;
				}
				else if (temp[1] == '*')
				{
					multiComment = true;
					getline(fileObj, temp);
					while (temp[temp.length() - 2] != '*' && temp[temp.length() - 1] != '/')
					{
						getline(fileObj, temp);
					}
				}
			}
			if (!comment && !multiComment)
			{
				newString += temp;
				mInstruction.push_back(temp);
			}
		}
	}
	Lexer obj(mInstruction);
	//Scanning here the lexical analysis has been done.
	obj.scanningInstruction(newString);
	obj.symbolTableCreation(mInstruction);
	obj.TAC_Insertion(mInstruction);
}


// Driver code
int main()
{
	writeToFile();
	readFromFile();
	return 0;
}
