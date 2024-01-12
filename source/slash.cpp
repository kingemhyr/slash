#include "slash.h"

void lex();
void parse();
void verify();
void generate();
void report();

int main(int argc, char **argv)
{
	if (argc == 1) FUCK(-1, "Not enough arguments");

	lex();
	parse();
	verify();
	generate();
	report();

	return 0;
}

using u8 = uint8_t;
using u16 = uint16_t;
using u64 = uint64_t;

void copy(u64 c, char *d, const char *s) {
	__builtin_memcpy(d, s, c);
}

enum class Error {
	redeclaration,
};

void err(Error error);

enum class Token_Type : u16 {
	error = 0,
	identifier = 1,
	string = 2,
	floating_point = 3,
	signed_integer = 4,
	unsigned_integer = 5,
	colon = ':',
	equals_sign = '=',
	left_curly_bracket = '{',
	period = '.',
};

struct Token {
	u64 length;
	union {
		const char *string;
	};
	Token_Type type;

	bool is_numeric() {
		return type >= Token_Type::floating_point
			&& type <= Token_Type::unsigned_integer;
	}

	bool is_binary_operator() {
		switch (type) {
		default: return false;
		}
	}
};

struct Parser {
	Token token;

	void lex();
};

/*

## Alias the contents of "User32.lib"
win32 :: #import "User32.lib";

## Make an instance of "User32.lib"
win32_2 := #import "User32.lib";

*/

struct Path {
};

struct Scope {
	u8 type:1;
};

using Identifier = char[64];

struct Declaration_Data {
	Path type;
	Scope scope;
};

struct Declaration {
	Identifier identifier;
	Declaration_Data data;
};

enum class Syntax_Type {
	declaration,
	path,
};

Token current_token;
Scope *current_scope;

void skip_to(Token_Type);

void expect(Token_Type);
void expect(Token_Type, Token_Type);
void expect(Token_Type, Token_Type, Token_Type);

void parse_path(Path *path, bool omittable);

void parse_runtime_scope(Scope *scope) {
	// expression|procedure
	lex();
	if (current_token.is_numeric()) {
		lex();
		if (current_token.is_binary_operator()) {
			lex();
		}
		else {
		}
	}
	else if (current_token.type == Token_Type::string) {
	}
	else if (current_token.type == Token_Type::identifier) {
	}
	else if (current_token.type == Token_Type::period) {
	}
	else if (current_token.type == Token_Type::left_curly_bracket) {
	}
}

void parse_comptime_scope(Scope *scope) {
	// expression|type|source
}

void parse_declaration(Declaration *out) {
	expect(Token_Type::identifier);
	copy(current_token.length, out->identifier, current_token.string);
	expect(Token_Type::colon);
	parse_path(&out->data.type, true);
	expect(Token_Type::equals_sign, Token_Type::colon);
	switch (current_token.type) {
	case Token_Type::equals_sign: parse_runtime_scope(&out->data.scope); break;
	case Token_Type::colon:		  parse_comptime_scope(&out->data.scope); break;
	default: __builtin_unreachable();
	}

}

struct Program {

};

void parse_program(Program *out) {
}

template<typename Key, typename Value>
struct Table {
	u64 count;
	Key (*keys)[];
	Value (*values)[];

	Value *push(Key *key, Value *value);
	Value *get(Key *key);
};

Table<char[64], Declaration_Data> *current_declarations;

bool parse_program(Declaration *out) {
	bool result = true;
	Declaration declaration;
	for (;;) {
		parse_declaration(&declaration);
	}
	return result;
}

#if 0
void v_report(const char *fn, unsigned line, report_level_t lvl, const char *fmt, va_list vargs)
{
	char buf[512];
	const char *str;
	uint64_t off;
	switch (lvl)
	{
	case report_level_NONE:
		UNREACHABLE();
		break;
	case report_level_NOTE:
		off = sizeof("NOTE:");
		str = "NOTE:";
		break;
	case report_level_WARNING:
		off = sizeof("WARNING:");
		str = "WARNING:";
		break;
	case report_level_ERROR:
		off = sizeof("ERROR:");
		str = "ERROR:";
		break;
	}
	(void)memcpy(buf, str, off);
	off += snprintf(buf +off - 1, sizeof(buf), "%s:%u", fn, line);
	off += vsnprintf(buf+off-1, sizeof(buf) - off, fmt, vargs);
	(void)fputs(buf, stderr);
}

void report(const char *fn, unsigned line, report_level_t lvl, const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	v_report(fn, line, lvl, fmt, vargs);
	va_end(vargs);
}

void fuck(const char *fn, unsigned line, int code, const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	v_report(fn, line, report_level_ERROR, fmt, vargs);
	va_end(vargs);
	exit(code);
}
#endif
