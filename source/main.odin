package slash

import "core:fmt"
import "core:unicode"

/*

	7 + 21 * 14;

[MUL, ADD, NAT(7), NAT(21), NAT(14), TER]

*/

main :: proc() {
	
}

Utf8 :: distinct u8
Utf32 :: distinct u32

lexer_advance :: proc(lexer: ^Lexer) -> (ended: bool) {
	using lexer

	if byte_position == input_length do return true

	decode_buffer: [4]Utf8
	decode_buffer_mass: uint = input_length - byte_position - 1
	if decode_buffer_mass > 4 do decode_buffer_mass = 4
	for i: uint = 0; i < decode_buffer_mass; i += 1 {
		decode_buffer[i] = input[byte_position + i]
	}
	increment: u8
	character, increment = utf8_decode(&decode_buffer)
	byte_position += auto_cast increment
	return false
}

lexer_lex :: proc(lexer: ^Lexer) -> (error: Error) {
	using lexer

	for {
		if lexer_advance(lexer) do return .Success
		if !is_whitespace(character) do break
	}

	#partial switch cast(Token_Type)character {
	case .Plus_Sign, .Hyphen_Minus, .Asterisk, .Slash, .Percent_Sign,
		.Semicolon:
		token.type = auto_cast character
	case:
		if is_letter(character) || (character == '_') {
			identifier := identifier_list_reserve(&global_identifier_list)
			for identifier.length < MAX_IDENTIFIER_CHARACTER_COUNT {
				identifier.length += utf8_encode(character, &identifier.data[i])
				if lexer_advance(lexer) do break
				if !(is_character(character) || (character == '_') || is_number(character)) do break
			}
			identifier_shrink(identifier)
			token.type = .Identifier
		} else if is_number(character) {
		}
	}

	return .Success
}

is_whitespace :: proc(input: Utf32) -> bool {
	return unicode.is_white_space(auto_cast input)
}

is_letter :: proc(input: Utf32) -> bool {
	return unicode.is_letter(auto_cast input)
}

is_number :: proc(input: Utf32) -> bool {
	return unicode.is_number(auto_cast input)
}

Error :: enum {
	Success = 0
}

Lexer :: struct {
	input: [^]Utf8,
	input_length: uint,
	character: Utf32,
	byte_position: uint,
	tokens: [^]Token,
	token: Token,
}

global_utf8_class_table: [32]u8 = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	2, 2, 2, 3, 3, 4, 5
}

utf8_decode :: proc(input: ^[4]Utf8) -> (codepoint: Utf32, length: u8) {
	lengths: []u8 = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0
    }
	masks: []u8 = {0x00, 0x7f, 0x1f, 0x0f, 0x07}
	mins: []u32 = {4194304, 0, 128, 2048, 65536}
	shiftc: []u8 = {0, 18, 12, 6, 0}
	shifte: []u8 = {0, 6, 4, 2, 0}

	length = auto_cast lengths[input[0] >> 3]
	next: ^Utf8 = &input[length + auto_cast(length == 0)]

	codepoint =  auto_cast(input[0] & auto_cast masks[length]) << 18
	codepoint |= auto_cast(input[1] & 0x3f) << 12
	codepoint |= auto_cast(input[2] & 0x3f) << 6
	codepoint |= auto_cast(input[3] & 0x3f)
	codepoint >>= shiftc[length]

/*
	error = auto_cast(auto_cast codepoint < mins[length]) << 6
	error |= auto_cast((codepoint >> 11) == 0x1b) << 7
	error |= auto_cast(codepoint > 0x10ffff) << 8
	error |= auto_cast(input[1] < 0xc0) >> 2
	error |= auto_cast(input[1] < 0xc0) >> 4
	error |= auto_cast(input[1] >> 6)
	error ~= 0x2a
	error >>= shifte[length]
*/
	
	return
}

MAX_IDENTIFIER_CHARACTER_COUNT :: 64

Identifier :: struct {
	data: [MAX_IDENTIFIER_CHARACTER_COUNT]Utf32,
	mass: u8,
}

Token :: struct {
	type: Token_Type,
	byte_position: u32,
	value: struct #raw_union {
		identifier: ^Identifier,
		natural: Natural,
		integer: Integer,
		real: Real,
		string: String,
		
	}
}

Token_Type :: enum u8 {
	Null = 0,

	Natural = 1,
	Integer = 2,
	Real    = 3,
	String  = 4,

	Plus_Sign    = '+',
	Hyphen_Minus = '-',
	Asterisk     = '*',
	Slash        = '/',
	Percent_Sign = '%',

	Semicolon = ';',
}

Natural :: u64
Integer :: i64
Real    :: f64
String  :: rawptr

Quantity :: u64

Literal_Value :: struct {
	code: Instruction_Code,
	value: struct #raw_union {
		identifier: ^Identifier,
		natural: Natural,
		integer: Integer,
		real: Real,
		string: String,
	},
}

// <code> {<arg>}
Instruction_Code :: enum u8 {
	Null = 0,

	Natural = 1,
	Integer = 2,
	Real    = 3,
	String  = 4,

	Termination = ';',

	Addition       = '+',
	Subtraction    = '-',
	Multiplication = '*',
	Division       = '/',
	Modulus        = '%',
}
