#pragma once

#include "include.h"

typedef unsigned char u8_t;

#ifdef _WIN64
#	define LLONG long long
#else
#	define LLONG long
#endif
typedef LLONG s64_t;
typedef unsigned LLONG u64_t;
#undef LLONG

// A negative value on failure, and a positive value on success.
typedef s64_t result_t;

constexpr int MAX_IDENTIFIER_LENGTH = 64;

template<typename T>
struct list_t
{
	u64_t capacity;
	u64_t quantity;
	T (*items)[];
};

template<typename T, typename U>
struct table_t
{
	u64_t capacity;
	u64_t quantity;
	T (*keys)[];
	U (*values)[];
};

template<typename T>
struct inferior_node_t
{
	inferior_node_t *prior;
	T value;
};

enum token_type_t
{
	IDENTIFIER = 1,
	OCTOTHORPE = '#',
	COLON = ':',
};

enum syntax_type_t
{
	DECLARATION,
	MACRO,
};

enum symbol_type_t : u8_t
{
	SCOPE,
	VARIABLE,
	CONSTANT,
	PROCEDURE,
};

struct expression_t
{
};

struct symbol_t;

using symbol_table_t = table_t<char[MAX_IDENTIFIER_LENGTH], symbol_t>;

union instruction_t
{
	enum : u8_t
	{
		END = 0,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		IF,
		ELIF,
		LOOP,
		JMP,
		RET,
		EQ,
		GT,
		GTE,
		LT,
		LTE,
		ASS,
	}
	code;
	u64_t value;
};

struct body_t
{
	body_t *superior;
	symbol_table_t symbols;
	list_t<instruction_t> instructions;
};

struct scope_t_
{
	symbol_t *symbol;
	symbol_table_t symbols;
};

using scope_t = inferior_node_t<scope_t_>;

struct variable_t
{
	symbol_t *symbol;
	expression_t expression;
};

struct constant_t
{
	symbol_t *symbol;
	expression_t expresion;
};

struct procedure_t
{
	symbol_t *symbol;
	body_t body;
};

struct symbol_t
{
	enum : u8_t
	{
		SCOPE,
		VARIABLE,
		CONSTANT,
		PROCEDURE,
	}
	type;
	union
	{
		scope_t scope;
		variable_t variable;
		constant_t constant;
		procedure_t procedure;
	};
};

struct compiler_t
{

};

struct program_t
{
	u64_t scope_quantity;
	scope_t (*scopes)[];
};

struct compilation_error_t
{
	enum : u8_t
	{
		UNEXPECTED_TOKEN,
		DECLARATION_PARSING,
		MACRO_PARSING,
	}
	code;
};

void report_compilation_error(compilation_error_t);

enum report_level_t
{
	NONE,
	NOTE,
	WARNING,
	ERROR,
};

void v_report(const char *fn, unsigned line, report_level_t lvl, const char *fmt, va_list vargs);

[[gnu::format(printf, 4, 5)]]
void report(const char *fn, unsigned line,  report_level_t lvl, const char *fmt, ...);

[[noreturn, gnu::format(printf, 4, 5)]]
void fuck(const char *fn, unsigned line, int code, const char *fmt, ...);

#define REPORT(...) report(__FUNCTION__, __LINE__, __VA_ARGS__)
#define REPORT_NOTE(...) REPORT(report_level_NOTE, __VA_ARGS__)
#define REPORT_WARNING(...) REPORT(report_level_WARNING, __VA_ARGS__)
#define REPORT_ERROR(...) REPORT(report_level_ERROR, __VA_ARGS__)
#define FUCK(...) fuck(__FUNCTION__, __LINE__, __VA_ARGS__)
