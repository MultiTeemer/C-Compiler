#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tokens.h"
#include "Exceptions.h"

using namespace std;

typedef enum {
	WORD,
	INT_DIGIT,
	FLOAT_DIGIT,
	FLOAT_DEGREE,
	FLOAT_DEGREE_ONLY_DIGITS,
	FLOAT_DEGREE_DIGIT_EXPECTED,
	STR,
	SINGLE_CHAR,
	SINGLE_CHAR_CLOSE,
	START,
	IN_COMMENT,
	COMMENT_CLOSE,
	IN_INLINE_COMMENT,
	ERROR,
	OP,
	SLASH,
	ESC_IN_CHAR,
	ESC_IN_STRING,
	DOT_OP,
	PLUS_OP,
	MINUS_OP,
	INC_OP,
	DEC_OP,
	END, 
	SEP,
	CHAR_END,
	COMMENT_END,
	STRING_END,
	PREPROC_DIRECTIVE,
	LESS_OP,
	GREATER_OP,
	ASSING_OP,
	EQUAL_OP,
	NOT_EQUAL_OP,
	EXCL,
	GREATER_OR_EQUAL_OP,
	LESS_OR_EQUAL_OP,
	BITWISE_SHIFT_LEFT_OP,
	BITWISE_SHIFT_RIGHT_OP,
	HEXADECIMAL,
	AMP,
	PIPE,
	ASTERISK,
	MULT_ASSIGN_OP,
	DIV_ASSIGN_OP,
	MOD_ASSIGN_OP,
	PLUS_ASSIGN_OP,
	MINUS_ASSIGN_OP,
	AND_ASSIGN_OP,
	OR_ASSIGN_OP,
	BITWISE_XOR_ASSIGN_OP,
	MOD_OP,
	BITWISE_SHIFT_LEFT_ASSIGN_OP,
	BITWISE_SHIFT_RIGHT_ASSIGN_OP,
	BITWISE_XOR_OP,
	STATES_COUNT
} LexerStatesT;

typedef enum {
	DIGIT_FOUNDED,
	LETTER_FOUNDED,
	SPACE_FOUNDED,
	NEWLINE_FOUDED,
	DOT_FOUNDED,
	EOF_FOUNDED,
	OP_FOUNDED,
	COMMA_FOUNDED,
	SEPARATOR_FOUNDED,
	UNDERSCOPE_FOUNDED,
	COLON_FOUNDED,
	QUESTION_MARK_FOUNDED,
	EXCLAMATION_MARK_FOUNDED,
	OPERATION_FOUNDED,
	SLASH_FOUNDED,
	BACKSLASH_FOUNDED,
	ASTERISK_FOUNDED,
	PLUS_FOUNDED,
	MINUS_FOUNDED,
	GRID_FOUNDED,
	E_CHAR_FOUNDED,
	QUOTE_FOUNDED,
	DOUBLE_QUOTE_FOUNDED,
	GREATER_OP_FOUNDED,
	LESS_OP_FOUNDED,
	EQUAL_OP_FOUNDED,
	AMP_FOUNDED,
	PIPE_FOUNDED,
	CIRCUMFLEX_FOUNDED,
	PERCENT_FOUNDED,
	UNKNOWN,
	EVENTS_COUNT
} LexerEventsT;

class Scanner;

typedef void (Scanner::*LexerActionT)();

typedef struct {
	LexerStatesT targetState;
	LexerActionT action;
} LexerTransitionT;

typedef vector<LexerTransitionT> TransitionsRowT;
typedef vector<TransitionsRowT> TransitionTableT;

class Scanner
{
private:	
	int line;
	int col;
	string filename;
	ifstream input;
	Token* cur_tok;
	LexerStatesT cur_state;
	TransitionTableT transitions;
	string buffer;
	void addDefaultTransition(LexerStatesT state, LexerActionT action = 0);
	void addAnyEventTransition(LexerStatesT fromState, LexerStatesT toState, LexerActionT action = 0);
	void addTransition(LexerStatesT fromState, LexerEventsT event, LexerStatesT toState, LexerActionT act);
	void lockItself(LexerStatesT state);
	void pushData();
	void PreprocDirectiveDetected();
	void IdentifierDetected();
	void SeparatorDetected();
	void OperationDetected();
	void IntegerDetected();
	void FloatDetected();
	void CommentDetected();
	void StringDetected();
	void CharDetected();
	void ParseError();	
	void EOFDetected();
	char escapingChar(const string& key);
	string prepareStrVal(const string& val);
public:	
	friend class Parser;
	Scanner(const char* filename);
	Scanner(const Scanner& clone): cur_tok(0), transitions(clone.transitions), input(clone.filename),
		filename(clone.filename), cur_state(START), line(clone.line), col(clone.col) {}
	Token* get();
	Token* next();
	bool hasNext() const;
};

#endif