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
    string currentToken = "";
    bool blockComment = false;
    bool hasSemicolon = false;

    // use a loop to scan each line in the file
    while (getline(infile, expression))
    {
        // use the "Lexer" function to isolate integer, real, operator,
        // string, and unknown tokens
        tokens = lexer(expression, prevState, currentState, currentToken);

        string currentLine;
        char *cstr;
        hasSemicolon = false;


        // display the tokens to the screen
        for (unsigned x = 0; x < tokens.size(); ++x)
        {
            outfile << std::left << std::setw(10) << tokens[x].lexemeName << "\t\t"
                 << tokens[x].token << endl;
            if (tokens[x].lexemeName == "IDENTIFIER"){
                tokens[x].token = 'i';
            }

            //check if has semicolon and don't include in parsing
            if (tokens[x].token == ";"){
                tokens[x].token = '$'; 
                hasSemicolon = true;
            }
            cstr = &tokens[x].token[0];
            currentLine.push_back(cstr[0]);

        }
        
        // don't parse if missing semicolon
        if (!hasSemicolon){
            outfile << "\nERROR:\n\t No Semicolon!\n";
            outfile << "\n##########################################################################\n";
        }else{
            std::streambuf *oldbuf = std::cout.rdbuf();
            std::cout.rdbuf(outfile.rdbuf()); 

            parser(currentLine);

            //reset back to standard input
            std::cout.rdbuf(oldbuf);
        }
        

    }

    infile.close();

    return 0;
} // end of main
