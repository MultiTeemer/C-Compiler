#include <iostream>
#include <fstream>
#include <string>
#include "Scanner.h"
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[])
{
	try {
		if (argc == 1)
		{
			cout << "C Compiler v0.3" << endl << "Design by Khoschenko Artem" << endl;
			return 0;
		} else if (argc == 2) {
			Scanner scanner((char*) argv[1]);
			while (scanner.hasNext())
			{
				Token* token = scanner.next();
				if (*token != END_OF_FILE)
					cout << token->info() << endl << lineSeparator;
			}
		} else {
			string asmOut(string((char*) argv[2]) + ".asm");
			if (strcmp((char*) argv[1], "-table") == 0)
			{
				Parser parser(Scanner((char*) argv[2]), CodeGenerator(asmOut));
				parser.parse();
				parser.print();
			} else if (strcmp((char*) argv[1], "-code") == 0){
				Parser parser(Scanner((char*) argv[2]), CodeGenerator(asmOut));
				parser.parse();
				parser.generateCode();
			} else {
				Parser parser(Scanner((char*) argv[2]), CodeGenerator(asmOut));
				parser.parseExpression()->print();
			}
		}		
		//Parser parser(Scanner("programm.cpp"), CodeGenerator("programm.asm"));
		//parser.parse();
		//parser.generateCode();
		//parser.print();
	} catch (exception &e) {
		cout << lineSeparator << e.what() << endl << lineSeparator;
	}
	return 0;
}
