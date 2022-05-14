// compile with C++11 
// flag:
//       -std=c++11

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include "lexer.h"
#include "parser.h"

using namespace std;

// Modified file io
// originally from https://www.programmingnotes.org/4699/c-simple-lexer-using-a-finite-state-machine/ 

int main()
{
    // declare variables
    ifstream infile;
    ofstream outfile;
    string fileName = "";
    string expression = "";
    vector<Token> tokens;

    // get data from user
    cout << "\nPlease enter the name of the file: ";
    getline(cin, fileName);

    infile.open(fileName.c_str());
    outfile.open("out_" + fileName);

    if (infile.fail())
    {
        cout << "\n** ERROR - the file \"" << fileName << "\" cannot be found!\n\n";
        exit(1);
    }

    int col = REJECT;
    int currentState = REJECT;
    int prevState = REJECT;
    int line_num = 0;
    string currentToken = "";
    bool blockComment = false;
    bool hasSemicolon = false;

    // use a loop to scan each line in the file
    while (getline(infile, expression))
    {
        line_num++;
        // use the "Lexer" function to isolate integer, real, operator,
        // string, and unknown tokens
        tokens = lexer(expression, prevState, currentState, currentToken);

        string currentLine;
        char *cstr;

        // display the tokens to the screen
        for (unsigned x = 0; x < tokens.size(); ++x)
        {
            outfile << std::left << std::setw(10) << tokens[x].lexemeName << "\t\t"
                 << tokens[x].token << endl;
            if (tokens[x].lexemeName == "IDENTIFIER"){
                tokens[x].token = 'i';
            }

            cstr = &tokens[x].token[0];
            currentLine.push_back(cstr[0]);

        }
        
        std::streambuf *oldbuf = std::cout.rdbuf();
        std::cout.rdbuf(outfile.rdbuf()); 

        parser(currentLine, line_num);

        //reset back to standard input
        std::cout.rdbuf(oldbuf);
        

    }

    infile.close();

    return 0;
} // end of main
