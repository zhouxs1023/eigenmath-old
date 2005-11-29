/* Pascal grammar checker. */

#include <stdio.h>

#define YYAND 128
#define YYARRAY 129
#define YYBEGIN 130
#define YYCASE 131
#define YYCONST 132
#define YYDIV 133
#define YYDO 134
#define YYDOWNTO 135
#define YYELSE 136
#define YYEND 137
#define YYFILE 138
#define YYFOR 139
#define YYFUNCTION 140
#define YYGOTO 141
#define YYIF 142
#define YYIN 143
#define YYLABEL 144
#define YYMOD 145
#define YYNIL 146
#define YYNOT 147
#define YYOF 148
#define YYOR 149
#define YYPACKED 150
#define YYPROCEDURE 151
#define YYPROGRAM 152
#define YYRECORD 153
#define YYREPEAT 154
#define YYSET 155
#define YYTHEN 156
#define YYTO 157
#define YYTYPE 158
#define YYUNTIL 159
#define YYVAR 160
#define YYWHILE 161
#define YYWITH 162
#define YYIDENTIFIER 163
#define YYDIGITSTRING 164
#define YYSTRING 165
#define YYCOLONEQUAL 166
#define YYPERIODPERIOD 167
#define YYNE 168
#define YYGE 169
#define YYLE 170
#define YYREAD 171
#define YYREADLN 172
#define YYWRITE 173
#define YYWRITELN 174

int token, token2;

char tokenstr[500], tokenstr2[500];

char infilename[100];

FILE *infile;

main(argc, argv)

int argc;

char *argv[];

{

	strcpy(infilename, argv[1]);

	infile = fopen(infilename, "r");

	y_get_token();

	y_get_token();

	y_program();

}

y_get_token()

{

	token = token2;

	strcpy(tokenstr, tokenstr2);

	if (*tokenstr)

		printf("%s", tokenstr + 4);

	fgets(tokenstr2, 500, infile);

	tokenstr2[strlen(tokenstr2) - 1] = 0;

	sscanf(tokenstr2, "%d", &token2);

}

y_error(s)

char *s;

{

	printf("<<<%s\n", s);

	exit(1);

}

/*---------------------------------------------------------------------------*/

y_actual_parameter_list()

{

	y_left_paren();

	y_expression();

	while (token == ',') {

		y_get_token();

		y_expression();

	}

	y_right_paren();

}

y_array_type()

{

	if (token == YYARRAY)

		y_get_token();

	else

		y_error(" 'array' expected");

	y_left_bracket();

	y_index_type();

	while (token == ',') {

		y_get_token();

		y_index_type();

	}

	y_right_bracket();

	y_of();

	y_type_denoter();

}

y_assignment_statement()

{

	y_variable_access();

	if (token == YYCOLONEQUAL)

		y_get_token();

	else

		y_error(" ':=' expected");

	y_expression();

}

y_block()

{

	if (token == YYLABEL)

		y_label_declaration_part();

	if (token == YYCONST)

		y_constant_definition_part();

	if (token == YYTYPE)

		y_type_definition_part();

	if (token == YYVAR)

		y_variable_declaration_part();

	for (;;)

		if (token == YYPROCEDURE)

			y_procedure_declaration();

		else if (token == YYFUNCTION)

			y_function_declaration();

		else

			break;

	y_compound_statement();

}

y_case_list_element()

{

	y_constant();

	while (token == ',') {

		y_get_token();

		y_constant();

	}

	y_colon();

	y_statement();

}

y_case_statement()

{

	y_get_token();

	y_expression();

	y_of();

	y_case_list_element();

	while (token == ';') {

		y_get_token();

		if (token == YYEND)

			break;

		else

			y_case_list_element();

	}

	y_end();

}

y_goto_statement()

{

	y_get_token();

	if (token == YYDIGITSTRING)

		y_get_token();

	else

		y_error(" label expected");

}

y_compound_statement()

{

	if (token == YYBEGIN)

		y_get_token();

	else

		y_error(" 'begin' expected");

	y_statement();

	while (token == ';') {

		y_get_token();

		y_statement();

	}

	if (token == YYEND)

		y_get_token();

	else

		y_error(" 'end' expected");

}

y_constant()

{

	if (token == YYSTRING)

		y_get_token();

	else {

		/* sign */

		if (token == '+' || token == '-')

			y_get_token();

		if (token == YYDIGITSTRING)

			y_get_token();

		else if (token == YYIDENTIFIER)

			y_get_token();

		else

			y_error(" constant expected");

	}

}

y_constant_definition()

{

	y_identifier();

	y_equal();

	y_constant();

	y_semicolon();

}

y_constant_definition_part()

{

	y_get_token();

	y_constant_definition();

	while (token == YYIDENTIFIER)

		y_constant_definition();

}

y_directive()

{

	y_get_token();

}

y_enumerated_type()

{

	y_error(" enumerated types are not supported");

}

y_expression()

{

	y_simple_expression();

	switch (token) {

	case '=':

		y_get_token();

		y_simple_expression();

		break;

	case '>':

		y_get_token();

		y_simple_expression();

		break;

	case '<':

		y_get_token();

		y_simple_expression();

		break;

	case YYNE:

		y_get_token();

		y_simple_expression();

		break;

	case YYGE:

		y_get_token();

		y_simple_expression();

		break;

	case YYLE:

		y_get_token();

		y_simple_expression();

		break;

	case YYIN:

		y_get_token();

		y_simple_expression();

		break;

	default:

		break;

	}

}

y_factor()

{

	if (token == '(') {

		y_get_token();

		y_expression();

		y_right_paren();

	} else if (token == YYIDENTIFIER) {

		if (token2 == '(')

			y_function_designator();

		else

			y_variable_access();

	} else if (token == YYDIGITSTRING) {

		y_get_token();

	} else if (token == YYSTRING) {

		y_get_token();

	} else if (token == YYNOT) {

		y_get_token();

		y_factor();

	} else

		y_error(" error in factor");

}

y_file_type()

{

	if (token == YYFILE)

		y_get_token();

	else

		y_error(" 'file' expected");

	y_of();

	y_type_denoter();

}

y_for_statement()

{

	y_get_token();

	y_identifier();

	if (token == YYCOLONEQUAL)

		y_get_token();

	else

		y_error(" ':=' expected");

	y_expression();

	if (token == YYTO)

		y_get_token();

	else if (token == YYDOWNTO)

		y_get_token();

	else

		y_error(" 'to' or 'downto' expected");

	y_expression();

	if (token == YYDO)

		y_get_token();

	else

		y_error(" 'do' expected");

	y_statement();

}

/*---------------------------------------------------------------------------*/

y_formal_parameter_list()

{

	y_left_paren();

	y_formal_parameter_section();

	while (token == ';') {

		y_get_token();

		y_formal_parameter_section();

	}

	y_right_paren();

}

y_formal_parameter_section()

{

	if (token == YYVAR)

		y_get_token();

	y_identifier_list();

	y_colon();

	y_identifier();

}

/*---------------------------------------------------------------------------*/

y_function_declaration()

{

	y_get_token();

	y_identifier();

	if (token == '(')

		y_formal_parameter_list();

	y_colon();

	y_identifier();

	y_semicolon();

	y_block();

	y_semicolon();

}

y_function_designator()

{

	y_get_token();

	if (token == '(')

		y_actual_parameter_list();

}

y_identifier()

{

	if (token != YYIDENTIFIER)

		y_error(" identifier expected");

	y_get_token();

}

y_identifier_list()

{

	y_identifier();

	while (token == ',') {

		y_get_token();

		y_identifier();

	}

}

y_if_statement()

{

	y_get_token();

	y_expression();

	if (token == YYTHEN)

		y_get_token();

	else

		y_error(" 'then' expected");

	y_statement();

	if (token == YYELSE) {

		y_get_token();

		y_statement();

	}

}

y_index_type()

{

	y_ordinal_type();

}

y_label_declaration_part()

{

	y_get_token();

	y_constant();

	while (token == ',') {

		y_get_token();

		y_constant();

	}

	y_semicolon();

}

y_new_ordinal_type()

{

	if (token == '(')

		y_enumerated_type();

	else

		y_subrange_type();

}

y_new_structured_type()

{

	if (token == YYPACKED)

		y_get_token();

	y_unpacked_structured_type();

}

y_new_type()

{

	switch (token) {

	case '(':

	case '+':

	case '-':

	case YYDIGITSTRING:

	case YYIDENTIFIER:

		y_new_ordinal_type();

		break;

	case YYARRAY:

	case YYPACKED:

	case YYRECORD:

		y_new_structured_type();

		break;

	default:

		y_error(" error in new-type");

	}

}

y_ordinal_type()

{

	if (token == YYIDENTIFIER /* and is an ordinal type identifier */ )

		y_get_token();

	else

		y_new_ordinal_type();

}

y_procedure_declaration()

{

	y_get_token();

	y_identifier();

	if (token == '(')

		y_formal_parameter_list();

	y_semicolon();

	y_block();

	y_semicolon();

}

y_procedure_statement()

{

	y_get_token();

	if (token == '(')

		y_actual_parameter_list();

}

y_program()

{

	if (token == YYPROGRAM)

		y_get_token();

	else

		y_error(" 'program' expected");

	y_identifier();

	if (token == '(') {

		y_get_token();

		y_identifier();

		while (token == ',')

			y_identifier();

		y_right_paren();

	}

	y_semicolon();

	y_block();

	if (token == '.')

		y_get_token();

	else

		y_error(" '.' expected");

}

/*---------------------------------------------------------------------------*/

y_record_type()

{

	y_get_token();

	y_field_list();

	y_end();

}

y_field_list()

{

	if (token != YYCASE)

		do {

			y_identifier();

			while (token == ',') {

				y_get_token();

				y_identifier();

			}

			y_colon();

			y_type_denoter();

			if (token == YYEND)

				break;

			y_semicolon();

		} while (token != YYCASE && token != YYEND);

	if (token == YYCASE) {

		y_get_token();

		y_identifier();

		if (token == ':') {

			y_get_token();

			y_identifier();

		}

		y_of();

		do {

			y_constant();

			while (token == ',') {

				y_get_token();

				y_constant();

			}

			y_colon();

			y_left_paren();

			y_field_list();

			y_right_paren();

			if (token == YYEND)

				break;

			y_semicolon();

		} while (token != YYEND);

	}

}

/*---------------------------------------------------------------------------*/

y_repeat_statement()

{

	y_get_token();

	y_statement();

	while (token == ';') {

		y_get_token();

		y_statement();

	}

	if (token == YYUNTIL)

		y_get_token();

	else

		y_error(" 'until' expected");

	y_expression();

}

y_set_type()

{

	y_error(" set types are not supported");

}

y_simple_expression()

{

	if (token == '+') {

		y_get_token();

		y_term();

	} else if (token == '-') {

		y_get_token();

		y_term();

	} else

		y_term();

	for (;;)

		if (token == '+') {

			y_get_token();

			y_term();

		} else if (token == '-') {

			y_get_token();

			y_term();

		} else if (token == YYOR) {

			y_get_token();

			y_term();

		} else

			break;

}

y_simple_statement()

{

	y_assignment_statement();

}

y_statement()

{

	if (token == YYDIGITSTRING) {

		y_get_token();

		y_colon();

	}

	if (token == YYBEGIN)

		y_compound_statement();

	else if (token == YYIDENTIFIER)

		if (token2 == '(' || token2 == ';')

			y_procedure_statement();

		else

			y_assignment_statement();

	else if (token == YYCASE)

		y_case_statement();

	else if (token == YYGOTO)

		y_goto_statement();

	else if (token == YYFOR)

		y_for_statement();

	else if (token == YYIF)

		y_if_statement();

	else if (token == YYREAD)

		y_read();

	else if (token == YYREADLN)

		y_readln();

	else if (token == YYREPEAT)

		y_repeat_statement();

	else if (token == YYWHILE)

		y_while_statement();

	else if (token == YYWRITE)

		y_write();

	else if (token == YYWRITELN)

		y_writeln();

}

y_subrange_type()

{

	y_constant();

	if (token == YYPERIODPERIOD)

		y_get_token();

	else

		y_error(" '..' expected");

	y_constant();

}

y_term()

{

	y_factor();

	for (;;)

		if (token == '*') {

			y_get_token();

			y_factor();

		} else if (token == '/') {

			y_get_token();

			y_factor();

		} else if (token == YYDIV) {

			y_get_token();

			y_factor();

		} else if (token == YYMOD) {

			y_get_token();

			y_factor();

		} else if (token == YYAND) {

			y_get_token();

			y_factor();

		} else

			break;

}

y_type_definition_part()

{

	y_get_token();

	y_type_definition();

	while (token == YYIDENTIFIER)

		y_type_definition();

}

y_type_definition()

{

	y_get_token();

	if (token == '=')

		y_get_token();

	else

		y_error(" '=' expected");

	y_type_denoter();

	y_semicolon();

}

y_type_denoter()

{

	if (token == YYIDENTIFIER)

		y_type_identifier();

	else

		y_new_type();

}

y_type_identifier()

{

	if (token != YYIDENTIFIER)

		y_error(" identifier expected");

	y_get_token();

}

y_unpacked_structured_type()

{

	switch (token) {

	case YYARRAY:

		y_array_type();

		break;

	case YYRECORD:

		y_record_type();

		break;

	case YYSET:

		y_set_type();

		break;

	case YYFILE:

		y_file_type();

		break;

	default:

		y_error(" error in unpacked-structured-type");

		break;

	}

}

y_variable_access()

{

	y_get_token();

	for (;;)

		if (token == '^') {

			y_get_token();

		} else if (token == '.') {

			y_get_token();

			y_get_token();

		} else if (token == '[') {

			y_get_token();

			y_expression();

			while (token == ',') {

				y_get_token();

				y_expression();

			}

			y_right_bracket();

		} else

			break;

}

y_variable_declaration()

{

	y_identifier_list();

	y_colon();

	y_type_denoter();

	y_semicolon();

}

y_variable_declaration_part()

{

	y_get_token();

	y_variable_declaration();

	while (token == YYIDENTIFIER)

		y_variable_declaration();

}

y_while_statement()

{

	y_get_token();

	y_expression();

	y_do();

	y_statement();

}

y_read()

{

	y_get_token();

	y_read_parameter_list();

}

y_readln()

{

	y_get_token();

	if (token == '(')

		y_read_parameter_list();

}

y_read_parameter_list()

{

	y_left_paren();

	y_variable_access();

	while (token == ',') {

		y_get_token();

		y_variable_access();

	}

	y_right_paren();

}

y_write()

{

	y_get_token();

	y_write_parameter_list();

}

y_writeln()

{

	y_get_token();

	if (token == '(')

		y_write_parameter_list();

	else

		;

}

y_write_parameter_list()

{

	y_left_paren();

	y_write_parameter();

	while (token == ',') {

		y_get_token();

		y_write_parameter();

	}

	y_right_paren();

}

y_write_parameter()

{

	y_expression();

	if (token == ':') {

		y_get_token();

		y_expression();

		if (token == ':') {

			y_get_token();

			y_expression();

		}

	}

}

/*---------------------------------------------------------------------------*/

y_colon()

{

	if (token == ':')

		y_get_token();

	else

		y_error(" ':' expected");

}

y_do()

{

	if (token == YYDO)

		y_get_token();

	else

		y_error(" 'do' expected");

}

y_end()

{

	if (token == YYEND)

		y_get_token();

	else

		y_error(" 'end' expected");

}

y_equal()

{

	if (token == '=')

		y_get_token();

	else

		y_error(" '=' expected");

}

y_left_bracket()

{

	if (token == '[')

		y_get_token();

	else

		y_error(" '[' expected");

}

y_left_paren()

{

	if (token == '(')

		y_get_token();

	else

		y_error(" '(' expected");

}

y_of()

{

	if (token == YYOF)

		y_get_token();

	else

		y_error(" 'of' expected");

}

y_right_bracket()

{

	if (token == ']')

		y_get_token();

	else

		y_error(" ']' expected");

}

y_right_paren()

{

	if (token == ')')

		y_get_token();

	else

		y_error(" ')' expected");

}

y_semicolon()

{

	if (token == ';')

		y_get_token();

	else

		y_error(" ';' expected");

}
