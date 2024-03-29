/*
references:
•	Course Textbook
•	https://andrewbegel.com/cs164/ll1.html
•	https://slaystudy.com/ll1-parsing-table-program-in-c/
•	http://www.dailyfreecode.com/code/ll1-parsing-1665.aspx
*/


#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

#define EPSILON "\u03b5"


enum terminals_list {
	IDENTIFIER = 1,
	EQUALS = 2,
	PLUS = 3,
	MINUS = 4,
	STAR = 5,
	SLASH = 6,
	LEFTPAREN = 7,
	RIGHTPAREN = 8,
	DOLLAR = 9,
};

enum nonterminals_list{
    //S = 1,
    E = 1,
    Q = 2,
    T = 3,
    R = 4,
    F = 5,
};


const static map<char, terminals_list> terminals {
  {'i',      IDENTIFIER },
  {'=',     EQUALS},
  {'+',      PLUS},
  {'-',        MINUS},
  {'*',      STAR},
  {'/',      SLASH},
  {'(',        LEFTPAREN},
  {')',     RIGHTPAREN},
  {'$',      DOLLAR}
};

int getNonTerminal(char c){
    switch(c){
        //case 'S':
        //    return S;
        //    break;
        case 'E':
            return E;
            break;
        case 'Q':
            return Q;
            break;
        case 'T':
            return T;
            break;
        case 'R':
            return R;
            break;
        case 'F':
            return F;
            break;
        defult:
            break;
    }

    return -1;
}

struct ProdRule {
    string token;
    int lexeme;
    string lexemeName;
};

class ParseTable {
	private:
		const static int num_terminal = 9+1;
		const static int num_nonterm = 7;

		// INPUTS
	protected:						
	char table[num_nonterm][num_terminal][10] = { 
        {"NT",  "i",     "=",		"+",        "-",        "*",        "/",       "(",     	")",      	"$"  },
		{"E",   "TQ",     "ERROR",	"ERROR",    "ERROR",    "ERROR",	"ERROR",   "TQ",    	"ERROR",  	"ERROR"  },
		{"Q",   "ERROR",  "=TQ",	"+TQ",      "-TQ",      "ERROR",  "ERROR",   "ERROR", 		"\0",     	"\0"  },
		{"T",   "FR",     "ERROR",	"ERROR",    "ERROR",    "ERROR",	"ERROR",   "FR",    	"ERROR",  	"ERROR"  },
		{"R",   "ERROR",  "\0",		"\0",       "\0",       "*FR",    "/FR",     "ERROR", 		"\0",     	"\0"  },
		{"F",   "i",     "ERROR",	"ERROR",    "ERROR",    "ERROR",	"ERROR",   "(E)",   	"ERROR",  	"ERROR"  }
	};


    public:
        char* getTable(int nonterm, int terminal){
            return this->table[nonterm][terminal];
        }
		
		void printTable(){
			for(int i=0;i<num_nonterm;i++)
			{
				for(int j=0;j<num_terminal;j++)
				{
					cout.setf(ios::right);
					cout.width(10);
					cout<<table[i][j];
				}
				cout<<endl;
			}
		}
};

void parser(string input)
{
	int a, x;
	int cursor = 0;
    char next, top;

	stack<char> stack;
	ParseTable parseTable;

    bool failed = false;
    bool isTerminal = false;

	// print current string to process
	//cout << "\n\tString: " << input << endl;
    //input.append("$", 1);
    cout << "\tinput stream: " << input << endl << endl;

    parseTable.printTable();


    // semicolon
    //if(input[input.length()-1] != ';')
    //{
    //    cout << "END OF STRING MARKER SHOULD BE ';'\n";
    //}


    // Begin parsing using stack
    stack.push('$');
    stack.push('E');
    
    while (!stack.empty()){
        top = stack.top();
        next = input[cursor];
        //set any lowercase a-z as identifier (i)
        if (islower(next)){
            next = 'i';
        }

        //reset isTerminal flag
        isTerminal = false;

        a = terminals.at(next);

		cout << "\n--------------------------------------------------------\n";

        //check if x is a terminal
        if (terminals.count(top)){
            isTerminal = true;

			cout << "\nTerminal:\n";
            cout << "\t" << top << endl;
        }else{
            x = getNonTerminal(top);

        	cout << "\nNon-Terminal:\n";
            cout << "\ttable" << endl << "\t["<< x << "]\t[" << a << "]" << endl;
            cout << "\t" << top << "\t" << next << endl;
        }

        if (isTerminal){
            if (top == next ){
                stack.pop();
                cursor++;
            }
            else {
                cout << "\nERROR:\n\t" << top << " is terminal but next is "<< next << endl ;
                failed = true;
            }
        }
        else {
            if (strcmp(parseTable.getTable(x,a),"ERROR") == 0){
                cout << "\nERROR:\n\t invalid string / production\n";
                failed = true;
            }
            //else if (strcmp(table[x][a], "\0") == 0){
            //    cout << "\nNOTE: epsilon" << endl;
            //    break;
            //}
            else{
                stack.pop();

                char temp[10];
                strncpy(temp, parseTable.getTable(x,a), 10);

				//print prod rule
				cout << "\n  Production Rule:\n";
				if (strcmp(temp, "\0") == 0){
					cout << "\t" << top << " -> " << EPSILON << endl;
				}else{
					cout << "\t" << top << " -> " << temp << endl;
				}
				
				// push reversed rule onto stack
				char reverse_string[10];
				strncpy(reverse_string, temp, 10);
                std::reverse(reverse_string, reverse_string + strlen(reverse_string));
                for(int i = 0; i < strlen(reverse_string); i++){
		            stack.push(reverse_string[i]);
                }
            }
        }

        if (failed){
            break;
        }
    }

	cout << "\n=========================================================================\n";

    if (!failed){
        cout << "\n[SUCCESS]\n";
    } else{
        cout << "\n[FAILED]\n";
    }

	cout << "\n##########################################################################\n";
}