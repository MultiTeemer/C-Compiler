#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Scanner.h"

using namespace std;

Scanner::Scanner(const char* filename): input(filename), filename(filename), line(1), col(1),
	cur_tok(0), cur_state(START), buffer(""), transitions(0)
{
	if (!input)
		throw exception("Illegal filename");

	transitions.resize(STATES_COUNT);
	for (int i = 0; i < STATES_COUNT; i++)
		transitions[i].resize(EVENTS_COUNT);	
	
	addDefaultTransition(START);
	addAnyEventTransition(END, END, &Scanner::EOFDetected);

	// Identifier
	addDefaultTransition(WORD, &Scanner::IdentifierDetected);
	addTransition(WORD, LETTER_FOUNDED, WORD, 0);
	addTransition(WORD, UNDERSCOPE_FOUNDED, WORD, 0);
	addTransition(WORD, DIGIT_FOUNDED, WORD, 0);
	addTransition(WORD, E_CHAR_FOUNDED, WORD, 0);
	
	addTransition(WORD, GRID_FOUNDED, ERROR, &Scanner::ParseError);

	// Separator
	addDefaultTransition(SEP, &Scanner::SeparatorDetected);

	// Exclamation
	addTransition(START, EXCLAMATION_MARK_FOUNDED, EXCL, 0);
	addDefaultTransition(EXCL, &Scanner::OperationDetected);
	addTransition(EXCL, EQUAL_OP_FOUNDED, NOT_EQUAL_OP, 0);
	addDefaultTransition(NOT_EQUAL_OP, &Scanner::OperationDetected);

	// Operation
	addDefaultTransition(OP, &Scanner::OperationDetected);

	addTransition(OP, GRID_FOUNDED, ERROR, &Scanner::ParseError);

	// Div by mod
	addDefaultTransition(MOD_OP, &Scanner::OperationDetected);
	addTransition(MOD_OP, EQUAL_OP_FOUNDED, MOD_ASSIGN_OP, 0);
	addDefaultTransition(MOD_ASSIGN_OP, &Scanner::OperationDetected);

	// Ampersand
	addDefaultTransition(AMP, &Scanner::OperationDetected);
	addTransition(AMP, AMP_FOUNDED, OP, 0);
	addTransition(AMP, EQUAL_OP_FOUNDED, AND_ASSIGN_OP, 0);
	addDefaultTransition(AND_ASSIGN_OP, &Scanner::OperationDetected);

	// Pipe
	addDefaultTransition(PIPE, &Scanner::OperationDetected);
	addTransition(PIPE, PIPE_FOUNDED, OP, 0);
	addTransition(PIPE, EQUAL_OP_FOUNDED, OR_ASSIGN_OP, 0);
	addDefaultTransition(OR_ASSIGN_OP, &Scanner::OperationDetected);

	// Circumflex
	addDefaultTransition(BITWISE_XOR_OP, &Scanner::OperationDetected);
	addTransition(BITWISE_XOR_OP, EQUAL_OP_FOUNDED, XOR_ASSIGN_OP, 0);
	addDefaultTransition(XOR_ASSIGN_OP, &Scanner::OperationDetected);

	// Plus, increment
	addDefaultTransition(PLUS_OP, &Scanner::OperationDetected);
	addTransition(PLUS_OP, PLUS_FOUNDED, INC_OP, 0);
	addDefaultTransition(INC_OP, &Scanner::OperationDetected);
	addTransition(PLUS_OP, EQUAL_OP_FOUNDED, PLUS_ASSIGN_OP, 0);
	addDefaultTransition(PLUS_ASSIGN_OP, &Scanner::OperationDetected);

	// Minus, decrement
	addDefaultTransition(MINUS_OP, &Scanner::OperationDetected);
	addTransition(MINUS_OP, MINUS_FOUNDED, DEC_OP, 0);
	addTransition(MINUS_OP, GREATER_OP_FOUNDED, OP, 0);
	addDefaultTransition(DEC_OP, &Scanner::OperationDetected);
	addTransition(MINUS_OP, EQUAL_OP_FOUNDED, MINUS_ASSIGN_OP, 0);
	addDefaultTransition(MINUS_ASSIGN_OP, &Scanner::OperationDetected);

	// Dot
	addDefaultTransition(DOT_OP, &Scanner::OperationDetected);
	addTransition(DOT_OP, DIGIT_FOUNDED, FLOAT_DIGIT, 0);
	addTransition(WORD, DOT_FOUNDED, OP, &Scanner::IdentifierDetected);

	// Asterisk
	addTransition(START, ASTERISK_FOUNDED, ASTERISK, 0);
	addDefaultTransition(ASTERISK, &Scanner::OperationDetected);
	addTransition(ASTERISK, EQUAL_OP_FOUNDED, MULT_ASSIGN_OP, 0);
	addDefaultTransition(MULT_ASSIGN_OP, &Scanner::OperationDetected);
	
	// Slash
	addDefaultTransition(SLASH, &Scanner::OperationDetected);
	addTransition(SLASH, ASTERISK_FOUNDED, IN_COMMENT, 0);
	addTransition(SLASH, SLASH_FOUNDED, IN_INLINE_COMMENT, 0);
	addTransition(SLASH, EQUAL_OP_FOUNDED, DIV_ASSIGN_OP, 0);
	addDefaultTransition(DIV_ASSIGN_OP, &Scanner::OperationDetected);

	// Inline comment
	lockItself(IN_INLINE_COMMENT);
	addTransition(IN_INLINE_COMMENT, NEWLINE_FOUDED, START, &Scanner::CommentDetected);
	addTransition(IN_INLINE_COMMENT, EOF_FOUNDED, END, &Scanner::CommentDetected);

	// Multiline comment
	lockItself(IN_COMMENT);
	addTransition(IN_COMMENT, ASTERISK_FOUNDED, COMMENT_CLOSE, 0);	
	addDefaultTransition(COMMENT_END, &Scanner::CommentDetected);
	addAnyEventTransition(COMMENT_CLOSE, IN_COMMENT, 0);
	addTransition(COMMENT_CLOSE, SLASH_FOUNDED, COMMENT_END, 0);
	addTransition(COMMENT_CLOSE, ASTERISK_FOUNDED, COMMENT_CLOSE, 0);

	addTransition(IN_COMMENT, EOF_FOUNDED, ERROR, &Scanner::ParseError);
	addTransition(COMMENT_CLOSE, EOF_FOUNDED, ERROR, &Scanner::ParseError);

	// Dot
	addTransition(START, DOT_FOUNDED, FLOAT_DIGIT, 0);
	addTransition(MINUS_OP, DOT_FOUNDED, FLOAT_DIGIT, &Scanner::OperationDetected);
	addTransition(PLUS_OP, DOT_FOUNDED, FLOAT_DIGIT, &Scanner::OperationDetected);

	// Integer
	addDefaultTransition(INT_DIGIT, &Scanner::IntegerDetected);
	addTransition(INT_DIGIT, DIGIT_FOUNDED, INT_DIGIT, 0);

	addTransition(INT_DIGIT, DOT_FOUNDED, FLOAT_DIGIT, 0);
	addTransition(INT_DIGIT, E_CHAR_FOUNDED, FLOAT_DEGREE, 0);

	// Float
	addDefaultTransition(FLOAT_DIGIT, &Scanner::FloatDetected);
	addTransition(FLOAT_DIGIT, DIGIT_FOUNDED, FLOAT_DIGIT, 0);

	addTransition(FLOAT_DIGIT, E_CHAR_FOUNDED, FLOAT_DEGREE, 0);
	addTransition(FLOAT_DEGREE, DIGIT_FOUNDED, FLOAT_DEGREE_ONLY_DIGITS, 0);
	addTransition(FLOAT_DEGREE, PLUS_FOUNDED, FLOAT_DEGREE_DIGIT_EXPECTED, 0);
	addTransition(FLOAT_DEGREE, MINUS_FOUNDED, FLOAT_DEGREE_DIGIT_EXPECTED, 0);
	addAnyEventTransition(FLOAT_DEGREE_DIGIT_EXPECTED, ERROR, &Scanner::ParseError);
	addTransition(FLOAT_DEGREE_DIGIT_EXPECTED, DIGIT_FOUNDED, FLOAT_DEGREE_ONLY_DIGITS, 0);
	
	addDefaultTransition(FLOAT_DEGREE_ONLY_DIGITS, &Scanner::FloatDetected);
	addTransition(FLOAT_DEGREE_ONLY_DIGITS, DIGIT_FOUNDED, FLOAT_DEGREE_ONLY_DIGITS, 0);

	addTransition(FLOAT_DEGREE, EOF_FOUNDED, END, &Scanner::ParseError);
	addTransition(FLOAT_DEGREE, OPERATION_FOUNDED, ERROR, &Scanner::ParseError);
	addTransition(FLOAT_DEGREE, QUOTE_FOUNDED, ERROR, &Scanner::ParseError);
	addTransition(FLOAT_DEGREE, DOUBLE_QUOTE_FOUNDED, ERROR, &Scanner::ParseError);
	addTransition(FLOAT_DIGIT, LETTER_FOUNDED, ERROR, &Scanner::ParseError);
	addTransition(FLOAT_DEGREE, LETTER_FOUNDED, ERROR, &Scanner::ParseError);
	addTransition(FLOAT_DEGREE_ONLY_DIGITS, LETTER_FOUNDED, ERROR, &Scanner::ParseError);

	// Char
	addAnyEventTransition(SINGLE_CHAR, SINGLE_CHAR_CLOSE);
	addTransition(SINGLE_CHAR, BACKSLASH_FOUNDED, ESC_IN_CHAR, 0);
	addAnyEventTransition(ESC_IN_CHAR, SINGLE_CHAR_CLOSE);
	addDefaultTransition(CHAR_END, &Scanner::CharDetected);
	addTransition(ESC_IN_CHAR, NEWLINE_FOUDED, ERROR, &Scanner::ParseError);
	addAnyEventTransition(SINGLE_CHAR_CLOSE, ERROR, &Scanner::ParseError);
	addTransition(SINGLE_CHAR_CLOSE, QUOTE_FOUNDED, CHAR_END, 0);
	addTransition(ESC_IN_CHAR, BACKSLASH_FOUNDED, SINGLE_CHAR_CLOSE, 0);

	// String
	lockItself(STR);
	addTransition(STR, DOUBLE_QUOTE_FOUNDED, STRING_END, 0);
	addDefaultTransition(STRING_END, &Scanner::StringDetected);

	addTransition(STR, BACKSLASH_FOUNDED, ESC_IN_STRING, 0);
	addAnyEventTransition(ESC_IN_STRING, STR);

	addTransition(STR, NEWLINE_FOUDED, ERROR, &Scanner::ParseError);
	addTransition(STR, EOF_FOUNDED, ERROR, &Scanner::ParseError);

	// Preprocessor directives
	addDefaultTransition(PREPROC_DIRECTIVE, &Scanner::PreprocDirectiveDetected);
	addTransition(PREPROC_DIRECTIVE, LETTER_FOUNDED, PREPROC_DIRECTIVE, 0);
	addTransition(PREPROC_DIRECTIVE, E_CHAR_FOUNDED, PREPROC_DIRECTIVE, 0);

	addTransition(PREPROC_DIRECTIVE, UNDERSCOPE_FOUNDED, ERROR, &Scanner::ParseError);
	
	// >, <, >=, <=, ==, =, >>, <<
	addDefaultTransition(GREATER_OP, &Scanner::OperationDetected);
	addTransition(GREATER_OP, EQUAL_OP_FOUNDED, GREATER_OR_EQUAL_OP, 0);
	addTransition(GREATER_OP, GREATER_OP_FOUNDED, BITWISE_SHIFT_RIGHT_OP, 0);	

	addDefaultTransition(GREATER_OR_EQUAL_OP, &Scanner::OperationDetected);
	addDefaultTransition(BITWISE_SHIFT_RIGHT_OP, &Scanner::OperationDetected);
	
	addTransition(BITWISE_SHIFT_RIGHT_OP, EQUAL_OP_FOUNDED, BITWISE_SHIFT_RIGHT_ASSIGN_OP, 0);
	addDefaultTransition(BITWISE_SHIFT_RIGHT_ASSIGN_OP, &Scanner::OperationDetected);

	addDefaultTransition(LESS_OP, &Scanner::OperationDetected);
	addTransition(LESS_OP, EQUAL_OP_FOUNDED, LESS_OR_EQUAL_OP, 0);
	addTransition(LESS_OP, LESS_OP_FOUNDED, BITWISE_SHIFT_LEFT_OP, 0);

	addDefaultTransition(LESS_OR_EQUAL_OP, &Scanner::OperationDetected);
	addDefaultTransition(BITWISE_SHIFT_LEFT_OP, &Scanner::OperationDetected);

	addTransition(BITWISE_SHIFT_LEFT_OP, EQUAL_OP_FOUNDED, BITWISE_SHIFT_LEFT_ASSIGN_OP, 0);
	addDefaultTransition(BITWISE_SHIFT_LEFT_ASSIGN_OP, &Scanner::OperationDetected);

	addDefaultTransition(ASSING_OP, &Scanner::OperationDetected);
	addTransition(ASSING_OP, EQUAL_OP_FOUNDED, EQUAL_OP, 0);
	addDefaultTransition(EQUAL_OP, &Scanner::OperationDetected);
}

Token* Scanner::next()
{
	LexerStatesT tmp = cur_state;
	int shift = col == 1 ? -1 : 0;
	bool nl = false;
	bool loop = true;
	while (loop)
	{
		char c;
		c = input.get();
		shift++;
		LexerEventsT event;
		if (c == -1)
			event = EOF_FOUNDED;
		else if (c == '#')
			event = GRID_FOUNDED;
		else if (c == 'e')
			event = E_CHAR_FOUNDED;
		else if (isalpha(c))
			event = LETTER_FOUNDED;
		else if (isalnum(c))
			event = DIGIT_FOUNDED;
		else if (c == '_')
			event = UNDERSCOPE_FOUNDED;
		else if (c == '.')
			event = DOT_FOUNDED;
		else if (c == '/')
			event = SLASH_FOUNDED;
		else if (c == '\\')
			event = BACKSLASH_FOUNDED;
		else if (c == '*')
			event = ASTERISK_FOUNDED;
		else if (c == '+') 
			event = PLUS_FOUNDED;
		else if (c == '-') 
			event = MINUS_FOUNDED;
		else if (c == '\'')
			event = QUOTE_FOUNDED;
		else if (c == '"')
			event = DOUBLE_QUOTE_FOUNDED;
		else if (c == '&')
			event = AMP_FOUNDED;
		else if (c == '|')
			event = PIPE_FOUNDED;
		else if (c == '%') 
			event = PERCENT_FOUNDED;
		else if(c == '^')
			event = CIRCUMFLEX_FOUNDED;
		else if (c == '{' || c == '}' || c == ';')
			event = SEPARATOR_FOUNDED;
		else if (c == '(' || c == ')' || c == '[' 
				|| c == ']' || c == ',' || c == '~')
			event = OP_FOUNDED;
		else if (c == ':')
			event = COLON_FOUNDED;
		else if (c == '\n') 
		{
			event = NEWLINE_FOUDED;
			nl = true;
		} else if (isspace(c))
			event = SPACE_FOUNDED;
		else if (c == '?')
			event = QUESTION_MARK_FOUNDED;
		else if (c == '!')
			event = EXCLAMATION_MARK_FOUNDED;
		else if (c == '>')
			event = GREATER_OP_FOUNDED;
		else if (c == '<')
			event = LESS_OP_FOUNDED;
		else if (c == '=')
			event = EQUAL_OP_FOUNDED;
		else
			event = UNKNOWN;
		LexerTransitionT& trans = transitions[cur_state][event];
		cur_state = trans.targetState;
		if (tmp == START)
			tmp = cur_state;
		if (trans.action)
		{
			(this->*(trans.action))();
			loop = false;
		}		
		if (!isspace(c) || cur_state == IN_COMMENT || cur_state == IN_INLINE_COMMENT || cur_state == STR)
			buffer.push_back(c);
		if (nl)
		{
			line++;
			col = 1;
			shift = 0;
			nl = false;
		}
	}
	col += shift;
	return cur_tok;
}

bool Scanner::hasNext() const
{
	return cur_state != END;
}

Token* Scanner::get() 
{
	return cur_tok;
}

void Scanner::addDefaultTransition(LexerStatesT state, LexerActionT action)
{
	addTransition(state, SPACE_FOUNDED, START, action);
	addTransition(state, NEWLINE_FOUDED, START, action);
	addTransition(state, ASTERISK_FOUNDED, ASTERISK, action);
	addTransition(state, OPERATION_FOUNDED, OP, action);
	addTransition(state, QUESTION_MARK_FOUNDED, OP, action);
	addTransition(state, COLON_FOUNDED, OP, action);
	addTransition(state, MINUS_FOUNDED, MINUS_OP, action);
	addTransition(state, PLUS_FOUNDED, PLUS_OP, action);
	addTransition(state, SEPARATOR_FOUNDED, SEP, action);
	addTransition(state, LETTER_FOUNDED, WORD, action);
	addTransition(state, UNDERSCOPE_FOUNDED, WORD, action);
	addTransition(state, E_CHAR_FOUNDED, WORD, action);
	addTransition(state, SLASH_FOUNDED, SLASH, action);
	addTransition(state, EXCLAMATION_MARK_FOUNDED, EXCL, action);
	addTransition(state, QUOTE_FOUNDED, SINGLE_CHAR, action);
	addTransition(state, DOUBLE_QUOTE_FOUNDED, STR, action);
	addTransition(state, DIGIT_FOUNDED, INT_DIGIT, action);
	addTransition(state, DOT_FOUNDED, DOT_OP, action);
	addTransition(state, GREATER_OP_FOUNDED, GREATER_OP, action);
	addTransition(state, LESS_OP_FOUNDED, LESS_OP, action);
	addTransition(state, EQUAL_OP_FOUNDED, ASSING_OP, action);
	addTransition(state, GRID_FOUNDED, PREPROC_DIRECTIVE, action);
	addTransition(state, AMP_FOUNDED, AMP, action);
	addTransition(state, PIPE_FOUNDED, PIPE, action);
	addTransition(state, CIRCUMFLEX_FOUNDED, BITWISE_XOR_OP, action);
	addTransition(state, PERCENT_FOUNDED, MOD_OP, action);
	addTransition(state, EOF_FOUNDED, END, action);
	addTransition(state, OP_FOUNDED, OP, action);
	addTransition(state, UNKNOWN, ERROR, &Scanner::ParseError);
	addTransition(state, BACKSLASH_FOUNDED, ERROR, &Scanner::ParseError);
}

void Scanner::lockItself(LexerStatesT state)
{
	addTransition(state, SPACE_FOUNDED, state, 0);
	addTransition(state, NEWLINE_FOUDED, state, 0);
	addTransition(state, ASTERISK_FOUNDED, state, 0);
	addTransition(state, OPERATION_FOUNDED, state, 0);
	addTransition(state, QUESTION_MARK_FOUNDED, state, 0);
	addTransition(state, COLON_FOUNDED, state, 0);
	addTransition(state, MINUS_FOUNDED, state, 0);
	addTransition(state, PLUS_FOUNDED, state, 0);
	addTransition(state, SEPARATOR_FOUNDED, state, 0);
	addTransition(state, LETTER_FOUNDED, state, 0);
	addTransition(state, UNDERSCOPE_FOUNDED, state, 0);
	addTransition(state, E_CHAR_FOUNDED, state, 0);
	addTransition(state, SLASH_FOUNDED, state, 0);
	addTransition(state, BACKSLASH_FOUNDED, state, 0);
	addTransition(state, EXCLAMATION_MARK_FOUNDED, state, 0);
	addTransition(state, EOF_FOUNDED, state, 0);
	addTransition(state, QUOTE_FOUNDED, state, 0);
	addTransition(state, DOUBLE_QUOTE_FOUNDED, state, 0);
	addTransition(state, DIGIT_FOUNDED, state, 0);
	addTransition(state, DOT_FOUNDED, state, 0);
	addTransition(state, GREATER_OP_FOUNDED, state, 0);
	addTransition(state, LESS_OP_FOUNDED, state, 0);
	addTransition(state, EQUAL_OP_FOUNDED, state, 0);
	addTransition(state, GRID_FOUNDED, state, 0);
	addTransition(state, AMP_FOUNDED, state, 0);
	addTransition(state, PIPE_FOUNDED, state, 0);
	addTransition(state, CIRCUMFLEX_FOUNDED, state, 0);
	addTransition(state, PERCENT_FOUNDED, state, 0);
	addTransition(state, OP_FOUNDED, state, 0);
	addTransition(state, UNKNOWN, state, 0);
}

void Scanner::addAnyEventTransition(LexerStatesT fromState, LexerStatesT toState, LexerActionT action)
{
	addTransition(fromState, SPACE_FOUNDED, toState, action);
	addTransition(fromState, NEWLINE_FOUDED, toState, action);
	addTransition(fromState, ASTERISK_FOUNDED, toState, action);
	addTransition(fromState, OPERATION_FOUNDED, toState, action);
	addTransition(fromState, QUESTION_MARK_FOUNDED, toState, action);
	addTransition(fromState, COLON_FOUNDED, toState, action);
	addTransition(fromState, MINUS_FOUNDED, toState, action);
	addTransition(fromState, PLUS_FOUNDED, toState, action);
	addTransition(fromState, SEPARATOR_FOUNDED, toState, action);
	addTransition(fromState, LETTER_FOUNDED, toState, action);
	addTransition(fromState, UNDERSCOPE_FOUNDED, toState, action);
	addTransition(fromState, E_CHAR_FOUNDED, toState, action);
	addTransition(fromState, SLASH_FOUNDED, toState, action);
	addTransition(fromState, BACKSLASH_FOUNDED, toState, action);
	addTransition(fromState, EXCLAMATION_MARK_FOUNDED, toState, action);
	addTransition(fromState, EOF_FOUNDED, toState, action);
	addTransition(fromState, QUOTE_FOUNDED, toState, action);
	addTransition(fromState, DOUBLE_QUOTE_FOUNDED, toState, action);
	addTransition(fromState, DIGIT_FOUNDED, toState, action);
	addTransition(fromState, DOT_FOUNDED, toState, action);
	addTransition(fromState, GREATER_OP_FOUNDED, toState, action);
	addTransition(fromState, LESS_OP_FOUNDED, toState, action);
	addTransition(fromState, EQUAL_OP_FOUNDED, toState, action);
	addTransition(fromState, GRID_FOUNDED, toState, action);
	addTransition(fromState, AMP_FOUNDED, toState, action);
	addTransition(fromState, PIPE_FOUNDED, toState, action);
	addTransition(fromState, CIRCUMFLEX_FOUNDED, toState, action);
	addTransition(fromState, PERCENT_FOUNDED, toState, action);
	addTransition(fromState, OP_FOUNDED, toState, action);
	addTransition(fromState, UNKNOWN, toState, action);
}

void Scanner::addTransition(LexerStatesT fromState, LexerEventsT event, LexerStatesT toState, LexerActionT act)
{
	LexerTransitionT& trans = transitions[fromState][event];
	trans.targetState = toState;
	trans.action = act;
}

char Scanner::escapingChar(const string& key)
{
	map<string, char> table;
	table["\\n"] = '\n';
	table["\\t"] = '\t';
	table["\\\\"] = '\\';
	table["\'"] = '\'';
	return table[key];
}

string Scanner::prepareStrVal(const string& val)
{
	string res;
	bool esc = false;
	for (int i = 1; i < val.length() - 1; i++)
	{
		char c = val[i];
		if (val[i] == '\\')
		{
			string key;
			key.push_back(val[i]);
			key.push_back(val[i + 1]);
			c = escapingChar(key);		
			i++;
		}
		res.push_back(c);
	}
	return res;
}

void Scanner::IdentifierDetected()
{
	char a[][10] = { "break", "char", "const", "continue", "do", "else", "float", "for", 
		"if", "int", "return", "struct", "sizeof", "typedef", "void",  "while" };
	vector<string> keywords;
	for (int i = 0; i < 21; i++)
		keywords.push_back(string(a[i]));
	bool isKeyword = false;
	int keyword = -1;
	for (int i = 0; i < keywords.size(); i++)
		if (keywords[i] == buffer)
		{
			isKeyword = true;
			keyword = i;
		}
	if (isKeyword)
		cur_tok = new KeywordToken(line, col, buffer, (KeywordsT) keyword);
	else if (buffer == "scanf")
		cur_tok = new OpToken(line, col, buffer, SCANF);
	else if (buffer == "printf")
		cur_tok = new OpToken(line, col, buffer, PRINTF);
	else 
		cur_tok = new IdentifierToken(line, col, buffer);
	buffer.clear();
}

void Scanner::EOFDetected()
{
	cur_tok = new EOFToken(line, col);
	buffer.clear();
}

void Scanner::SeparatorDetected()
{
	char separators[] = { '{', '}', ';' };
	int sep = -1;
	for (int i = 0; i < SEPARATORS_COUNT; i++)
		if (buffer[0] == separators[i])
			sep = i;
	cur_tok = new SepToken(line, col, buffer, (SeparatorsT) sep);
	buffer.clear();
}

void Scanner::IntegerDetected()
{
	try {
		int val = stoi(buffer);
		cur_tok = new IntegerToken(line, col, buffer, val);
		buffer.clear();
	} catch (out_of_range& e) {
		throw ScannerException("Integer is out of range", line, col);
	}
}

void Scanner::FloatDetected()
{
	try {
		float val = stof(buffer);
		cur_tok = new FloatToken(line, col, buffer, val);
		buffer.clear();
	} catch (out_of_range& e) {
		throw ScannerException("Float is out of range", line, col);
	}
}

void Scanner::OperationDetected()
{
	char operations[][5] = { "+", "-", "*", "/", "%", "++", "--", "+=", "-=", "*=", "/=", "%=", "?", ":",
		"&", "|", "&&", "||", "!", "^", "&=", "|=", "^=", "=", "==", "!=", ">", "<", ">=", "<=", "<<", ">>", ".", "->",
		"<<=", ">>=", "(", ")", "[", "]", ",", "~" };
	int op = -1;
	for (int i = 0; i < OPERATIONS_COUNT; i++)
		if (buffer == operations[i])
			op = i;
	if (op == -1)
		throw ScannerException("Invalid operation", line, col);
	cur_tok = new OpToken(line, col, buffer, (OperationsT) op);
	buffer.clear();
}

void Scanner::CommentDetected()
{
	cur_tok = new StringValToken(COMMENT, line, col, buffer);
	buffer.clear();
}

void Scanner::CharDetected()
{
	char val;
	if (buffer.length() == 3)
		val = buffer[1];
	 else {
		string key;
		key.push_back(buffer[1]);
		key.push_back(buffer[2]);
		val = escapingChar(key);
	}
	cur_tok = new CharToken(line, col, buffer, val);
	buffer.clear();
}

void Scanner::StringDetected()
{
	cur_tok = new StringToken(STRING, line, col, buffer, prepareStrVal(buffer));
	buffer.clear();
}

void Scanner::PreprocDirectiveDetected()
{
	char directives[][10] = {"#define", "#undef", "#include", "#if", "#ifdef", "#ifndef", "#else",
		"#elif", "#endif", "#line", "#error", "#pragma", "#"};
	bool correct = false;
	for (int i = 0; i < 13; i++)
		if (buffer == directives[i])
			correct = true;
	if (!correct)
		ParseError();
	cur_tok = new StringValToken(PREPROCESSOR_DIRECTIVE, line, col, buffer);
	buffer.clear();
}

void Scanner::ParseError()
{
	char* msg;
	int size = buffer.length() + resourceSize;
	msg = (char*) malloc(sizeof(char) * size);
	sprintf(msg, "Invalid last character in line %d: %s", line, buffer.c_str());
	string what(msg);
	free(msg);
	throw ScannerException(what.c_str(), line, col);	
}

