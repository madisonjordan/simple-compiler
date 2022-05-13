/*
KEYWORDS 	=	int, float, bool, if, else, then, do, while, for, and, or, function
IDENTIFIERS 	=	legal identifiers must start with alphabetic character follow by digits, alpha, or $
SEPARATORS 	=	'(){}[],.:; sp(space)
OPERATORS 	=	*+-=/><%


Additional examples:
Valid IDENTIFIERS	:  	num1, large$, num$1, num2, num2$, a, ab, ...
Valid Numbers		:	integers whole numbers (1,2,3,...) and reals floats (5.0, 0.9, 1.75, ...)
Valid Block Comments	:	!  this is a block comment to be ignored by the Lexical Analyzer !
*/

#include <string>
#include <map>

using namespace std;


// Modified Functions, and State Table 
// originally from https://www.programmingnotes.org/4699/c-simple-lexer-using-a-finite-state-machine/  


// These are the inputs for the FSM.
enum TransitionStates {
	REJECT = 0,				// REJECT is the starting state and final state.
	INTEGER = 1,
	REAL = 2,
	OPERATOR = 3,
	STRING = 4,
	SPACE = 5,
	SEPARATOR = 6,
	COMMENT = 7,
	UNKNOWN = 8,
};

class FSM;
          					 
class StateTable {
	friend FSM;	

		// INPUTS
	protected:						 /* INTEGER,   REAL,    OPERATOR,   STRING,   SPACE,   SEPARATOR,  COMMENT,  UNKNOWN  */
		int table[9][9] = { {REJECT,	INTEGER,   REAL,    OPERATOR,   STRING,   SPACE,   SEPARATOR,  COMMENT,  UNKNOWN  },
		/* STATE 1 */		{INTEGER,   INTEGER,   REAL,    REJECT,     REJECT,	  REJECT,   REJECT,    COMMENT,  REJECT  },
		/* STATE 2 */		{REAL,      REAL,     UNKNOWN,  REJECT,     REJECT,   REJECT,   REJECT,    COMMENT,  REJECT  },
		/* STATE 3 */		{OPERATOR,  REJECT,   REJECT,   REJECT,     REJECT,   REJECT,   REJECT,    COMMENT,  REJECT  },
		/* STATE 4 */		{STRING,    STRING,   REJECT,   REJECT,     STRING,   REJECT,   REJECT,    COMMENT,  REJECT  },
		/* STATE 5 */		{SPACE,     REJECT,   REJECT,   REJECT,     REJECT,   REJECT,   REJECT,    COMMENT,  REJECT  },
		/* STATE 6 */		{SEPARATOR, REJECT,   REJECT,   REJECT,     REJECT,   REJECT,   REJECT,    COMMENT,  REJECT  },
		/* STATE 7 */		{COMMENT,	COMMENT,  COMMENT,  COMMENT,    COMMENT,  COMMENT,  COMMENT,   REJECT,   COMMENT  },
		/* STATE 8 */		{UNKNOWN,   UNKNOWN,  UNKNOWN,  UNKNOWN,    UNKNOWN,  REJECT,   REJECT,    COMMENT,  UNKNOWN }
		};

    public:
        int getTable(int state, int col){
            return this->table[state][col];
        }
};

// https://craftinginterpreters.com/scanning.html
enum TokenType {
  // Keywords.
  INT, FLOAT, BOOL, IF, ELSE, THEN, DO, WHILE, FOR, AND, OR, FUNCTION
};

struct Token {
    string token;
    int lexeme;
    string lexemeName;
};


const static map<string,TokenType> keywords {
  {"int",       INT},
  {"float",     FLOAT},
  {"bool",      BOOL},
  {"if",        IF},
  {"else",      ELSE},
  {"then",      THEN},
  {"do",        DO},
  {"while",     WHILE},
  {"for",       FOR},
  {"and",       AND},
  {"or",        OR},
  {"function",  FUNCTION}
};

// function prototypes
int Get_FSM_Col(char);
string GetLexemeName(int);


/**
* FUNCTION: GetLexemeName
* USE: Returns the string equivalent of an integer lexeme token type.
* @param lexeme - An integer lexeme token type.
* @return - An std::string string representing the name of the integer
*        lexeme token type.
*/
string GetLexemeName(int lexeme)
{
    switch(lexeme)
    {
        case INTEGER:
           return "INTEGER";
           break;
        case REAL:
           return "REAL  ";
           break;
        case OPERATOR:
           return "OPERATOR";
           break;
        case COMMENT:
            return "COMMENT";
            break;
        case STRING:
           return "STRING";
           break;
        case UNKNOWN:
           return "UNKNOWN";
           break;
        case SPACE:
           return "SPACE";
           break;
        case SEPARATOR:
           return "SEPARATOR";
           break;
        default:
           return "ERROR";
           break;
    }
};


vector<Token> lexer(string expression, int prevState, int currentState, string currentToken) {
    vector<Token> tokens;
	Token access;
	StateTable stateTable;
    int col = REJECT;
    char currentChar = ' ';


    // Begin the string walk.
     // use an FSM to parse the expression
    for(unsigned x = 0; x < expression.length();)
    {

		// What is the input?
        currentChar = expression[x];
 
		// get current column/input of current character getCol(char)
        col = Get_FSM_Col(currentChar);
 
        // get the current state of the expression
        currentState = stateTable.getTable(currentState,col);

		// State switching mechanism
		// Is current state starting state? (Did you just arrive on a new lexeme?)
		if(currentState == REJECT)
        {    
            // Only add tokens and lexemes that are not SPACE or COMMENT to container.
			if(prevState != SPACE && prevState != COMMENT)
			{
				access.token = currentToken;
                access.lexeme = prevState;
                access.lexemeName = GetLexemeName(access.lexeme);
                if(access.lexemeName == "STRING"){
                    if(keywords.find(currentToken) == keywords.end()){
                    //keyword found
                    access.lexemeName = "IDENTIFIER";
                        
                    }
                    else
                        access.lexemeName = "KEYWORD";
                        
                }
                tokens.push_back(access);
			}
			// If previous state is COMMENT. We move the expression index forward.
 
			// Reset lexeme string.
            currentToken = "";
		}
		// Will skip comments entirely without catching.
		// Catch and concat character for current lexeme
		else
        {
            currentToken += currentChar;
            ++x;
        }
		
		// Reassign previous state
        prevState = currentState;
	}

    // this ensures the last token gets saved when
    // we reach the end of the loop (if a valid token exists)
    if(currentState != SPACE && currentState !=COMMENT  && currentToken != "")
    {// ^^ we dont care about whitespace
        access.token = currentToken;
        access.lexeme = currentState;
        access.lexemeName = GetLexemeName(access.lexeme);
        tokens.push_back(access);
    }
	// Handling block comments that extend onto other lines
 
   

    

	return tokens;
};

// end of Lexer
 

/**
* FUNCTION: Get_FSM_Col
* USE: Determines the state of the type of character being examined.
* @param currentChar - A character.
* @return - Returns the state of the type of character being examined.
*/
int Get_FSM_Col(char currentChar)
{
    // check for whitespace
    if(isspace(currentChar))
    {
        return SPACE;
    }
    // check for comment block
	else if(currentChar == '!')
    {
        return COMMENT;
    }

    // check for integer numbers
    else if(isdigit(currentChar))
    {
        return INTEGER;
    }
 
    // check for real numbers
    else if(currentChar == '.')
    {
        return REAL;
    }

    // check for characters
    else if(isalpha(currentChar) || currentChar == '$')
    {
        return STRING;
    }

    else if(ispunct(currentChar)){
    //OPERATORS
        switch(currentChar) {
            case '-': 
                return OPERATOR; 
                break;
            case '+': 
                return OPERATOR; 
                break;
            case '*': 
                return OPERATOR; 
                break;
            case '=': 
                return OPERATOR; 
                break; 
             case '/': 
                return OPERATOR; 
                break; 
             case '<': 
                return OPERATOR; 
                break; 
             case '>': 
                return OPERATOR; 
                break; 
            case '%':
                return OPERATOR; 
                break; 
        //SEPARATORS
            case '\'': 
                return SEPARATOR; 
                break;
            case '(': 
                return SEPARATOR; 
                break;
            case ')': 
                return SEPARATOR; 
                break;
            case '[': 
                return SEPARATOR; 
                break;
            case ']': 
                return SEPARATOR; 
                break;
            case '{': 
                return SEPARATOR; 
                break;
            case '}': 
                return SEPARATOR; 
                break;
            case ',': 
                return SEPARATOR; 
                break;
            case ';': 
                return SEPARATOR; 
                break;
            case ':': 
                return SEPARATOR; 
                break;
        }
    }

    return UNKNOWN;
}; // end of Get_FSM_Col
 
