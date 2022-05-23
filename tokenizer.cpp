

#include <iostream>
#include <cctype>
#include <deque>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <utility>
using namespace std;

enum Token {
	literal, identifier, plus_s, minus_s, Multiple, equals, leftparen, rightparen, error, eof, newline, terminal
};
struct Tokenizer {
	Token token;
	string word;
};

string ts(Token t) {
	switch (t) {
	case literal: 
		return "literal token"; break;
	case identifier:
		return "identifier token"; break;
	case plus_s:
		return "+"; break;
	case minus_s: 
		return "-"; break;
	case Multiple: 
		return "*"; break;
	case leftparen: 
		return "("; break;
	case rightparen: 
		return ")"; break;
	case equals: 
		return "="; break;
	case error: 
		return "error\n"; break;
	case eof: 
		return "eof"; break;
	case newline: 
		return "\n"; break;
	case terminal: 
		return "terminal"; break;
	}
	return "erro";
}
class Tokenizerf {
private:
	ifstream inFile;
	pair<Token, string> curr_token;
	pair<Token, string> get_token() {
		char c;
		string word, line;
		while (inFile.get(c)) {
			if (isdigit(c)) {
				if (c == '0') {
					char next = inFile.peek();
					if (isdigit(next)) { 
						getline(inFile, line);
						return make_pair(error, "error"); 
					}
					else return make_pair(literal, "0");
				}
				else if (c >= '1' && c <= '9') { 
					while (isdigit(c)) {
						word += c;
						inFile.get(c);
					}
					inFile.putback(c);
					return make_pair(literal, word);
				}
			}
			else if (isalpha(c) || c == '_') {
				while (isalnum(c) || c == '_') {
					word += c;
					inFile.get(c);
				}
				inFile.putback(c);
				return make_pair(identifier, word);
			}
			else if (c == '+') 
				return make_pair(plus_s, "+");
			else if (c == '-') 
				return make_pair(minus_s, "-");
			else if (c == '*') 
				return make_pair(Multiple, "*");
			else if (c == '=') 
				return make_pair(equals, "=");
			else if (c == '(') 
				return make_pair(leftparen, "(");
			else if (c == ')') 
				return make_pair(rightparen, ")");
			else if (c == ';') 
				return make_pair(terminal, ";");
		}
		return make_pair(eof, "End");
	}
public:
	Tokenizerf() = delete;
	Tokenizerf(string input) {
		inFile.open(input);
	}
	~Tokenizerf() {
		inFile.close();
	}
	deque<pair<Token, string>> tokenizer() {
		deque<pair<Token, string>> t;
		curr_token = get_token();
		while (curr_token.first != eof) {
			t.push_back(curr_token);
			curr_token = get_token();
		}
		t.push_back(make_pair(eof, "End"));
		return t;
	}
};
class test {
private:
	deque<pair<Token, string>> tokens;
	pair<Token, string> current;
	std::map<std::string, int> state;

public:
	test(deque<pair<Token, string>> tokens) {
		this->tokens = tokens;
	}
	void get() {
		current = tokens.front();
		tokens.pop_front();
	}

	void match(Token t) {
		if (current.first == t) {
			get();
		}
		else {
			cout << ts(t) << "Error Token " << ts(current.first) << "\n";
			exit(-1);
		}
	}
	
	void printSymbolTable() {
		for (pair<Token, string> r : tokens) {
			cout << r.second << ts(r.first) << endl;
		}
	}

	
	void pg() {
		get();
		while (!tokens.empty()) {
			assignment();
		}
		for (auto const& x : state)
		{
			cout << x.first  
				<< " = "
				<< x.second 
				<< endl;
		}
		
	}

	
	void assignment() {
		if (current.first != identifier) {
			cout << "Invaild\n"; exit(-1);
		}
		else {
			string foo = current.second;
			match(identifier);
			match(equals);
			int x = exp();
			state[foo] = x;
			match(terminal);
		}
	}


	int expPrime() {
		if (current.first == plus_s) {
			match(plus_s);
			return exp();
		}
		else if (current.first == minus_s) {
			match(minus_s);
			return -exp();
		}
	}

	
	int exp() {
		int i = term();
		if (current.first == plus_s || current.first == minus_s) {
			i += expPrime();
		}
		return i;
	}

	
	int term() {
		int i = fact();
		if (current.first == Multiple) {
			i *= termPrime();
		}
		return i;
	}

	int termPrime() {
		match(Multiple);
		return term();
	}

	
	int fact() {
		if (current.first == leftparen) {
			match(leftparen);
			int i = exp();
			match(rightparen);
			return i;
		}
		else if (current.first == minus_s) {
			match(minus_s);
			return -fact();
		}
		else if (current.first == plus_s) {
			match(plus_s);
			return fact();
		}
		else if (current.first == literal) {
			int i = stoi(current.second);
			match(literal);
			return i;
		}
		else if (current.first == identifier) {
			if (state.find(current.second) == state.end()) { 
				match(terminal);
				cout << current.second << " error.\n"; exit(-1);
			}
			string tmp = current.second;
			match(identifier);
			return state[tmp];
		}
	}
};

int main() {
	string filename = "input.txt";
	Tokenizerf pen(filename);
	deque<pair<Token, string>> t = pen.tokenizer();
	test qit(t);
	qit.pg();

	return 0;
}

