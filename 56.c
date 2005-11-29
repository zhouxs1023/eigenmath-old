/* Unfinished assembler for Intel 8051. */

#include <stdio.h>

char buf[500];
int pass2;
unsigned int pc;
char *token;
int token_flag;
char *syntax = "syntax error";
FILE *fo, *fl;
int nsym;
int nline;

typedef struct {
	char s[34];
	int v, type;
} SYMBOL;

SYMBOL *sym[10000];

main(argc, argv)
int argc;
char *argv[];
{
	FILE *f;

	if (argc != 2) {
		printf("usage: asm51 filename\n");
		exit();
	}

	pass(argv[1]);

	pass2 = 1;

	fo = fopen("object", "w");

	fl = fopen("listing", "w");

	pass(argv[1]);

	dump_symbol_table();
}

pass(s)
char *s;
{
	FILE *f;

	pc = 0;

	nline = 1;

	f = fopen(s, "r");

	while (fgets(buf, 500, f)) {
		nline++;
		parse();
	}

	fclose(f);
}

#define ACALL 0
#define ADD 1
#define ADDC 2
#define AJMP 3
#define ANL 4
#define CJNE 5
#define CLR 6
#define CPL 7
#define DA 8
#define DEC 9
#define DIV 10
#define DJNZ 11
#define INC 12
#define JB 13
#define JBC 14
#define JC 15
#define JMP 16
#define JNB 17
#define JNC 18
#define JNZ 19
#define JZ 20
#define LCALL 21
#define LJMP 22
#define MOV 23
#define MOVC 24
#define MOVX 25
#define MUL 26
#define NOP 27
#define ORL 28
#define POP 29
#define PUSH 30
#define RET 31
#define RETI 32
#define RL 33
#define RLC 34
#define RR 35
#define RRC 36
#define SETB 37
#define SJMP 38
#define SUBB 39
#define SWAP 40
#define XCH 41
#define XCHD 42
#define XRL 43

char *s, tokenstr[500], labelstr[500];

parse()
{
	s = buf;

	token_flag = 0;

	label();

	opcode();
}

label()
{
	if (istoken(*s)) {
		gettoken();
		strcpy(labelstr, tokenstr);
		gettoken();
		if (strcmp(tokenstr, ":") != 0)
			ungettoken();
	} else
		*labelstr = 0;
}

ungettoken()
{
	token_flag = 1;
}

gettoken()
{
	int i;
	char *t = tokenstr;

	if (token_flag) {
		token_flag = 0;
		return;
	}

	while (*s == ' ' || *s == '\t')
		s++;

	token = s;

	if (*s == 0 || *s == '\n' || *s == '\r' || *s == ';') {
		*t = 0;
		return;
	}

	if (istoken(*s))
		while (istoken(*s))
			*t++ = *s++;
	else
		*t++ = *s++;

	*t = 0;
}

istoken(c)
int c;
{
	return c >= '0' && c <= '9' || c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_';
}

char *ops[44] = {
	"ACALL",
	"ADD",
	"ADDC",
	"AJMP",
	"ANL",
	"CJNE",
	"CLR",
	"CPL",
	"DA",
	"DEC",
	"DIV",
	"DJNZ",
	"INC",
	"JB",
	"JBC",
	"JC",
	"JMP",
	"JNB",
	"JNC",
	"JNZ",
	"JZ",
	"LCALL",
	"LJMP",
	"MOV",
	"MOVC",
	"MOVX",
	"MUL",
	"NOP",
	"ORL",
	"POP",
	"PUSH",
	"RET",
	"RETI",
	"RL",
	"RLC",
	"RR",
	"RRC",
	"SETB",
	"SJMP",
	"SUBB",
	"SWAP",
	"XCH",
	"XCHD",
	"XRL",
};

findopcode(s)
char *s;
{
	int l = 0, m, u = 43, x;
	for (;;) {
		m = (l + u) / 2;
		x = strcmp(s, ops[m]);
		if (x == 0)
			return m;
		if (x < 0)
			u = m - 1;
		if (x > 0)
			l = m + 1;
		if (l > u)
			return -1;
	}
}

opcode()
{
	gettoken();

	if (*tokenstr == 0) {
		definelabel();
		return;
	}

	switch (findopcode(tokenstr)) {

	case ACALL:
		definelabel();
		acall();
		break;

	case ADD:
		definelabel();
		add();
		break;

	case ADDC:
		definelabel();
		addc();
		break;

	case AJMP:
		definelabel();
		ajmp();
		break;

	case ANL:
		definelabel();
		anl();
		break;

	case CJNE:
		definelabel();
		cjne();
		break;

	case CLR:
		definelabel();
		clr();
		break;

	case CPL:
		definelabel();
		cpl();
		break;

	case DA:
		definelabel();
		da();
		break;

	case DEC:
		definelabel();
		dec();
		break;

	case DIV:
		definelabel();
		div();
		break;

	case DJNZ:
		definelabel();
		djnz();
		break;

	case INC:
		definelabel();
		inc();
		break;

	case JB:
		definelabel();
		jb();
		break;

	case JBC:
		definelabel();
		jbc();
		break;

	case JC:
		definelabel();
		jc();
		break;

	case JMP:
		definelabel();
		jmp();
		break;

	case JNB:
		definelabel();
		jnb();
		break;

	case JNC:
		definelabel();
		jnc();
		break;

	case JNZ:
		definelabel();
		jnz();
		break;

	case JZ:
		definelabel();
		jz();
		break;

	case LCALL:
		definelabel();
		lcall();
		break;

	case LJMP:
		definelabel();
		ljmp();
		break;

	case MOV:
		definelabel();
		mov();
		break;

	case MOVC:
		definelabel();
		movc();
		break;

	case MOVX:
		definelabel();
		movx();
		break;

	case NOP:
		definelabel();
		nop();
		break;

	case MUL:
		definelabel();
		mul();
		break;

	case ORL:
		definelabel();
		orl();
		break;

	case POP:
		definelabel();
		pop();
		break;

	case PUSH:
		definelabel();
		push();
		break;

	case RET:
		definelabel();
		ret();
		break;

	case RETI:
		definelabel();
		reti();
		break;

	case RL:
		definelabel();
		rl();
		break;

	case RLC:
		definelabel();
		rlc();
		break;

	case RR:
		definelabel();
		rr();
		break;

	case RRC:
		definelabel();
		rrc();
		break;

	case SETB:
		definelabel();
		setb();
		break;

	case SJMP:
		definelabel();
		sjmp();
		break;

	case SUBB:
		definelabel();
		subb();
		break;

	case SWAP:
		definelabel();
		swap();
		break;

	case XCH:
		definelabel();
		xch();
		break;

	case XCHD:
		definelabel();
		xchd();
		break;

	case XRL:
		definelabel();
		xrl();
		break;

	default:
		error("bad opcode");
		break;
	}
}

#define ACCUMULATOR 0
#define DIRECT 1
#define INDIRECT 2
#define REGISTER 3
#define IMMEDIATE 4
#define CARRY 5
#define DPTR 6
#define NOTBIT 7
#define AT_DPTR 8

acall()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	if (pass2 && (addr & 0xf800) != ((pc + 2) & 0xf800))
		error(syntax);

	emit_2(addr >> 3 & 0xe0 | 0x11, addr & 0xff);	/* ACALL addr11 */
}

add()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0x28 | src);			/* ADD A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0x25, src);			/* ADD A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0x26 | src);			/* ADD A,@Ri */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_2(0x24, src);			/* ADD A,#data */

	else
		error(syntax);
}

addc()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0x38 | src);			/* ADDC A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0x35, src);			/* ADDC A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0x36 | src);			/* ADDC A,@Ri */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_2(0x34, src);			/* ADDC A,#data */

	else
		error(syntax);
}

ajmp()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	if (pass2 && (addr & 0xf800) != ((pc + 2) & 0xf800))
		error(syntax);

	emit_2(addr >> 3 & 0xe0 | 0x01, addr & 0xff);	/* AJMP addr11 */
}

anl()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0x58 | src);			/* ANL A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0x55, src);			/* ANL A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0x56 | src);			/* ANL A,@Ri */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_1(0x54, src);			/* ANL A,#data */

	else if (d == DIRECT && s == ACCUMULATOR)
		emit_2(0x52, dst);			/* ANL direct,A */

	else if (d == DIRECT && s == IMMEDIATE)
		emit_3(0x53, dst, src);			/* ANL direct,#data */

	else if (d == CARRY && s == DIRECT)
		emit_2(0x82, src);			/* ANL C,bit */

	else if (d == CARRY && s == NOTBIT)
		emit_2(0xb0, src);			/* ANL C,/bit */

	else
		error(syntax);
}

cjne()
{
	unsigned int addr, d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	comma();

	if (operand(&addr) != DIRECT)
		error(syntax);

	if (d == ACCUMULATOR && s == DIRECT)
		emit_3(0xb5, src, addr - pc - 3);	/* CJNE A,direct,rel */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_3(0xb4, src, addr - pc - 3);	/* CJNE A,#data,rel */

	else if (d == REGISTER && s == IMMEDIATE)
		emit_3(0xb8 | dst, src, addr - pc - 3);	/* CJNE Rn,#data,rel */

	else if (d == INDIRECT && s == IMMEDIATE)
		emit_3(0xb6 | dst, src, addr - pc - 3);	/* CJNE @Ri,#data,rel */

	else
		error(syntax);
}

clr()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0xf4);				/* CLR A */

	else if (d == CARRY)
		emit_1(0xc3);				/* CLR C */

	else if (d == DIRECT)
		emit_2(0xc2, dst);			/* CLR bit */

	else
		error(syntax);
}

cpl()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0xf4);				/* CPL A */

	else if (d == CARRY)
		emit_1(0xb3);				/* CPL C */

	else if (d == DIRECT)
		emit_2(0xb2, dst);			/* CPL bit */

	else
		error(syntax);
}

da()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0xd4);					/* DA A */

	else
		error(syntax);
}

dec()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0x14);				/* DEC A */

	else if (d == REGISTER)
		emit_1(0x18 | dst);			/* DEC Rn */

	else if (d == DIRECT)
		emit_2(0x15, dst);			/* DEC direct */

	else if (d == INDIRECT)
		emit_1(0x16 | dst);			/* DEC @Ri */

	else
		error(syntax);
}

div()
{
	gettoken();
	if (strcmp(tokenstr, "AB") != 0)
		error(syntax);

	emit_1(0x84);					/* DIV AB */
}

djnz()
{
	unsigned int addr, d, dst;

	d = operand(&dst);

	comma();

	if (operand(&addr) != DIRECT)
		error(syntax);

	if (d == REGISTER)
		emit_2(0xd8 | dst, addr - pc - 2);	/* DJNZ Rn,rel */

	else if (d == DIRECT)
		emit_3(0xd5, dst, addr - pc - 3);	/* DJNZ direct,rel */

	else
		error(syntax);
}

inc()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0x04);				/* INC A */

	else if (d == REGISTER)
		emit_1(0x08 | dst);			/* INC Rn */

	else if (d == DIRECT)
		emit_2(0x05, dst);			/* INC direct */

	else if (d == INDIRECT)
		emit_1(0x06 | dst);			/* INC @Ri */

	else if (d == DPTR)
		emit_1(0xa3);				/* INC DPTR */

	else
		error(syntax);
}

jb()
{
	unsigned int bit, addr;

	if (operand(&bit) != DIRECT)
		error(syntax);

	comma();

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_3(0x20, bit, addr - pc - 3);		/* JB bit,rel */
}

jbc()
{
	unsigned int bit, addr;

	if (operand(&bit) != DIRECT)
		error(syntax);

	comma();

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_3(0x10, bit, addr - pc - 3);		/* JBC bit,rel */
}

jc()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_2(0x40, addr - pc - 2);			/* JC rel */
}

jmp()
{
	gettoken();
	if (strcmp(tokenstr, "@") != 0)
		error(syntax);

	gettoken();
	if (strcmp(tokenstr, "A") != 0)
		error(syntax);

	gettoken();
	if (strcmp(tokenstr, "+") != 0)
		error(syntax);

	gettoken();
	if (strcmp(tokenstr, "DPTR") != 0)
		error(syntax);

	emit_1(0x73);					/* JMP @A+DPTR */
}

jnb()
{
	unsigned int bit, addr;

	if (operand(&bit) != DIRECT)
		error(syntax);

	comma();

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_3(0x30, bit, addr - pc - 3);		/* JNB bit,rel */
}

jnc()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_2(0x50, addr - pc - 2);			/* JNC rel */
}

jnz()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_2(0x70, addr - pc - 2);			/* JNZ rel */
}

jz()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_2(0x60, addr - pc - 2);			/* JZ rel */
}

lcall()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_3(0x12, addr >> 8, addr & 0xff);		/* LCALL addr16 */
}

ljmp()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_3(0x02, addr >> 8, addr & 0xff);		/* LJMP addr16 */
}

mov()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0xe8 | src);			/* MOV A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0xe5, src);			/* MOV A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0xe6 | src);			/* MOV A,@Ri */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_2(0x74, src);			/* MOV A,#data */

	else if (d == REGISTER && s == ACCUMULATOR)
		emit_1(0xf8 | dst);			/* MOV Rn,A */

	else if (d == REGISTER && s == DIRECT)
		emit_2(0xa8 | dst, src);		/* MOV Rn,direct */

	else if (d == REGISTER && s == IMMEDIATE)
		emit_2(0x78 | dst, src);		/* MOV Rn,#data */

	else if (d == DIRECT && s == ACCUMULATOR)
		emit_2(0xf5, dst);			/* MOV direct,A */

	else if (d == DIRECT && s == REGISTER)
		emit_2(0x88 | src, dst);		/* MOV direct,Rn */

	else if (d == DIRECT && s == DIRECT)
		emit_3(0x85, src, dst);			/* MOV direct,direct */

	else if (d == DIRECT && s == INDIRECT)
		emit_2(0x86 | src, dst);		/* MOV direct,@Ri */

	else if (d == DIRECT && s == IMMEDIATE)
		emit_3(0x75, dst, src);			/* MOV direct,#data */

	else if (d == INDIRECT && s == ACCUMULATOR)
		emit_1(0xf6 | dst);			/* MOV @Ri,A */

	else if (d == INDIRECT && s == DIRECT)
		emit_2(0xa6 | dst, src);		/* MOV @Ri,direct */

	else if (d == INDIRECT && s == IMMEDIATE)
		emit_2(0x76 | dst, src);		/* MOV @Ri,#data */

	else if (d == CARRY && s == DIRECT)
		emit_2(0xa2, src);			/* MOV C,bit */

	else if (d == DIRECT && s == CARRY)
		emit_2(0x92, dst);			/* MOV bit,C */

	else if (d == DPTR && s == IMMEDIATE)
		emit_3(0x90, src >> 8, src & 0xff);	/* MOV DPTR,#data16 */

	else
		error(syntax);
}

movc()
{
	gettoken();
	if (strcmp(tokenstr, "A") != 0)
		error(syntax);

	comma();

	gettoken();
	if (strcmp(tokenstr, "@") != 0)
		error(syntax);

	gettoken();
	if (strcmp(tokenstr, "A") != 0)
		error(syntax);

	gettoken();
	if (strcmp(tokenstr, "+") != 0)
		error(syntax);

	gettoken();

	if (strcmp(tokenstr, "DPTR") == 0)
		emit_1(0x93);				/* MOVC A,@A+DPTR */

	else if (strcmp(tokenstr, "PC") == 0)
		emit_1(0x83);				/* MOVC A,@A+PC */

	else
		error(syntax);
}

movx()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0xe2 | src);			/* MOVX A,@Ri */

	else if (d == ACCUMULATOR && s == AT_DPTR)
		emit_1(0xe0);				/* MOVX A,@DPTR */

	else if (d == INDIRECT && s == ACCUMULATOR)
		emit_1(0xf2 | dst);			/* MOVX @Ri,A */

	else if (d == AT_DPTR && s == ACCUMULATOR)
		emit_1(0xf0);				/* MOVX @DPTR,A */

	else
		error(syntax);
}

nop()
{
	emit_1(0x00);					/* NOP */
}

mul()
{
	gettoken();
	if (strcmp(tokenstr, "AB") != 0)
		error(syntax);

	emit_1(0xa4);					/* MUL AB */
}

orl()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0x48 | src);			/* ORL A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0x45, src);			/* ORL A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0x46 | src);			/* ORL A,@Ri */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_1(0x44, src);			/* ORL A,#data */

	else if (d == DIRECT && s == ACCUMULATOR)
		emit_2(0x42, dst);			/* ORL direct,A */

	else if (d == DIRECT && s == IMMEDIATE)
		emit_3(0x43, dst, src);			/* ORL direct,#data */

	else if (d == CARRY && s == DIRECT)
		emit_2(0x72, src);			/* ORL C,bit */

	else if (d == CARRY && s == NOTBIT)
		emit_2(0xa0, src);			/* ORL C,/bit */

	else
		error(syntax);
}

pop()
{
	unsigned int d, dst;

	if (operand(&dst) != DIRECT)
		error(syntax);

	emit_2(0xd0, dst);				/* POP direct */
}

push()
{
	unsigned int s, src;

	if (operand(&src) != DIRECT)
		error(syntax);

	emit_2(0xc0, src);				/* PUSH direct */
}

ret()
{
	emit_1(0x22);					/* RET */
}

reti()
{
	emit_1(0x32);					/* RETI */
}

rl()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0x23);					/* RL A */

	else
		error(syntax);
}

rlc()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0x33);					/* RLC A */

	else
		error(syntax);
}

rr()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0x03);					/* RR A */

	else
		error(syntax);
}

rrc()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0x13);					/* RRC A */

	else
		error(syntax);
}

setb()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == CARRY)
		emit_1(0xd3);				/* SETB C */

	else if (d == DIRECT)
		emit_2(0xd2, dst);			/* SETB bit */

	else
		error(syntax);
}

sjmp()
{
	unsigned int addr;

	if (operand(&addr) != DIRECT)
		error(syntax);

	emit_2(0x80, addr - pc - 2);			/* SJMP rel */
}

subb()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0x98 | src);			/* SUBB A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0x95, src);			/* SUBB A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0x96 | src);			/* SUBB A,@Ri */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_2(0x94, src);			/* SUBB A,#data */

	else
		error(syntax);
}

swap()
{
	unsigned int d, dst;

	d = operand(&dst);

	if (d == ACCUMULATOR)
		emit_1(0xc4);					/* SWAP A */

	else
		error(syntax);
}

xch()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0xc8 | src);			/* XCH A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0xc5, src);			/* XCH A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0xc6 | src);			/* XCH A,@Ri */

	else
		error(syntax);
}

xchd()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0xd6 | src);			/* XCHD A,@Ri */

	else
		error(syntax);
}

xrl()
{
	unsigned int d, dst, s, src;

	d = operand(&dst);

	comma();

	s = operand(&src);

	if (d == ACCUMULATOR && s == REGISTER)
		emit_1(0x68 | src);			/* XRL A,Rn */

	else if (d == ACCUMULATOR && s == DIRECT)
		emit_2(0x65, src);			/* XRL A,direct */

	else if (d == ACCUMULATOR && s == INDIRECT)
		emit_1(0x66 | src);			/* XRL A,@Ri */

	else if (d == ACCUMULATOR && s == IMMEDIATE)
		emit_1(0x64, src);			/* XRL A,#data */

	else if (d == DIRECT && s == ACCUMULATOR)
		emit_2(0x62, dst);			/* XRL direct,A */

	else if (d == DIRECT && s == IMMEDIATE)
		emit_3(0x63, dst, src);			/* XRL direct,#data */

	else
		error(syntax);
}

operand(x)
int *x;
{
	gettoken();

	if (strcmp(tokenstr, "A") == 0)
		return ACCUMULATOR;

	if (strcmp(tokenstr, "C") == 0)
		return CARRY;

	if (strcmp(tokenstr, "/") == 0) {
		*x = expr();
		return NOTBIT;
	}

	if (strcmp(tokenstr, "DPTR") == 0)
		return DPTR;

	if (*tokenstr == '#') {
		*x = expr();
		return IMMEDIATE;
	}

	if (*tokenstr == '@') {
		gettoken();
		if (strcmp(tokenstr, "R0") == 0) {
			*x = 0x00;
			return INDIRECT;
		}
		if (strcmp(tokenstr, "R1") == 0) {
			*x = 0x01;
			return INDIRECT;
		}
		if (strcmp(tokenstr, "DPTR") == 0)
			return AT_DPTR;
		error(syntax);
	}

	if (strcmp(tokenstr, "R0") == 0) {
		*x = 0x00;
		return REGISTER;
	}

	if (strcmp(tokenstr, "R1") == 0) {
		*x = 0x01;
		return REGISTER;
	}

	if (strcmp(tokenstr, "R2") == 0) {
		*x = 0x02;
		return REGISTER;
	}

	if (strcmp(tokenstr, "R3") == 0) {
		*x = 0x03;
		return REGISTER;
	}

	if (strcmp(tokenstr, "R4") == 0) {
		*x = 0x04;
		return REGISTER;
	}

	if (strcmp(tokenstr, "R5") == 0) {
		*x = 0x05;
		return REGISTER;
	}

	if (strcmp(tokenstr, "R6") == 0) {
		*x = 0x06;
		return REGISTER;
	}

	if (strcmp(tokenstr, "R7") == 0) {
		*x = 0x07;
		return REGISTER;
	}

	ungettoken();

	*x = expr();

	return DIRECT;
}

expr()
{
	int v;

	gettoken();

	if (strcmp(tokenstr, "+") == 0)
		v = term();
	else if (strcmp(tokenstr, "-") == 0)
		v = -term();
	else {
		ungettoken();
		v = term();
	}

	for (;;) {

		gettoken();

		if (strcmp(tokenstr, "+") == 0)
			v += term();
		else if (strcmp(tokenstr, "-") == 0)
			v -= term();
		else {
			ungettoken();
			break;
		}
	}

	return v;
}

term()
{
	int v;

	v = factor();

	for (;;) {

		gettoken();

		if (strcmp(tokenstr, "*") == 0)
			v *= factor();
		else if (strcmp(tokenstr, "/") == 0)
			v /= factor();
		else {
			ungettoken();
			break;
		}
	}

	return v;
}

factor()
{
	gettoken();

	if (strcmp(tokenstr, "$") == 0)
		return pc;

	else if (isdigit(*tokenstr))
		return number();

	else
		error("bad factor");
}

number()
{
	int c, u;

	c = tokenstr[strlen(tokenstr) - 1];

	if (c >= '0' && c <= '9')
		u = decimal_number(0);
	else
		switch (c) {

		case 'B':
			u = binary_number();
			break;

		case 'D':
			u = decimal_number(1);
			break;

		case 'H':
			u = hex_number();
			break;

		case 'O':
		case 'Q':
			u = octal_number();
			break;

		default:
			error("error in base designator");
			break;
		}

	return u;
}

decimal_number(d)
int d;
{
	int i, n;
	char c;
	unsigned long u = 0;

	n = strlen(tokenstr) - d;

	for (i = 0; i < n; i++) {

		c = tokenstr[i];

		if (c >= '0' && c <= '9')
			u = 10 * u + c - '0';
		else
			error("error in decimal number");

		if (u > 65535)
			error("overflow in decimal number");
	}

	return (unsigned int) u;
}

binary_number()
{
	int i, n;
	char c;
	unsigned long u = 0;

	n = strlen(tokenstr) - 1;

	for (i = 0; i < n; i++) {

		c = tokenstr[i];

		if (c >= '0' && c <= '1')
			u = 2 * u + c - '0';
		else
			error("error in binary number");

		if (u > 65535)
			error("overflow in binary number");
	}

	return (unsigned int) u;
}

hex_number()
{
	int i, n;
	char c;
	unsigned long u = 0;

	n = strlen(tokenstr) - 1;

	for (i = 0; i < n; i++) {

		c = tokenstr[i];

		if (c >= '0' && c <= '9')
			u = 16 * u + c - '0';
		else if (c >= 'A' && c <= 'Z')
			u = 16 * u + c - 'A' + 10;
		else
			error("error in hex number");

		if (u > 65535)
			error("overflow in hex number");
	}

	return (unsigned int) u;
}

octal_number()
{
	int i, n;
	char c;
	unsigned long u = 0;

	n = strlen(tokenstr) - 1;

	for (i = 0; i < n; i++) {

		c = tokenstr[i];

		if (c >= '0' && c <= '7')
			u = 8 * u + c - '0';
		else
			error("error in octal number");

		if (u > 65535)
			error("overflow in octal number");
	}

	return (unsigned int) u;
}

comma()
{
	gettoken();
	if (strcmp(tokenstr, ",") != 0)
		error("comma expected");
}

definelabel()
{
	int i, l, m, u;

	if (*labelstr == 0 || pass2)
		return;

	l = 0;

	u = nsym - 1;

	for (;;) {

		if (l > u)
			break;

		m = (l + u) / 2;

		i = strcmp(labelstr, sym[m]->s);

		if (i < 0)
			u = m - 1;
		else if (i > 0)
			l = m + 1;
		else
			error("label already defined");
	}

	if (nsym == 10000)
		error("symbol table overflow");

	for (i = nsym; i > l; i--)
		sym[i] = sym[i - 1];

	nsym++;

	sym[l] = (SYMBOL *) malloc(sizeof (SYMBOL));

	if (sym[l] == NULL)
		error("out of memory");

	strcpy(sym[l]->s, labelstr);

	sym[l]->v = pc;
}

error(s)
char *s;
{
	printf("%s", buf);

	printf("line %d: %s\n", nline, s);

	exit(0);
}

emit_1(x)
int x;
{
	emit(x);

	if (pass2)
		fprintf(fl, "%04X %02X         %s", pc, x, buf);

	pc++;
}

emit_2(x, y)
int x, y;
{
	emit(x);
	emit(y);

	if (pass2)
		fprintf(fl, "%04X %02X %02X      %s", pc, x, y, buf);

	pc += 2;
}

emit_3(x, y, z)
int x, y, z;
{
	emit(x);
	emit(y);
	emit(z);

	if (pass2)
		fprintf(fl, "%04X %02X %02X %02X   %s", pc, x, y, z, buf);

	pc += 3;
}

emit(i)
int i;
{
	static int n;
	if (pass2) {
		if (i < -128 || i > 255)
			error("operand out of range");
		fprintf(fo, "%02X", i);
		if (++n == 32) {
			n = 0;
			fprintf(fo, "\n");
		}
	}
}

dump_symbol_table()
{
	int i;

	fprintf(fl, "symbol table\n");

	for (i = 0; i < nsym; i++)
		fprintf(fl, "%s %d\n", sym[i]->s, sym[i]->v);
}

/*

test file

	MOV	A,#10H

	NOP
	AJMP	0*256
	LJMP	0
	RR	A
	INC	A
	INC	0
	INC	@R0
	INC	@R1
	INC	R0
	INC	R1
	INC	R2
	INC	R3
	INC	R4
	INC	R5
	INC	R6
	INC	R7
	JBC	0,$+3
	ACALL	0*256
	LCALL	0
	RRC	A
	DEC	A
	DEC	0
	DEC	@R0
	DEC	@R1
	DEC	R0
	DEC	R1
	DEC	R2
	DEC	R3
	DEC	R4
	DEC	R5
	DEC	R6
	DEC	R7
	JB	0,$+3
	AJMP	1*256
	RET
	RL	A
	ADD	A,#0
	ADD	A,0
	ADD	A,@R0
	ADD	A,@R1
	ADD	A,R0
	ADD	A,R1
	ADD	A,R2
	ADD	A,R3
	ADD	A,R4
	ADD	A,R5
	ADD	A,R6
	ADD	A,R7
	JNB	0,$+3
	ACALL	1*256
	RETI
	RLC	A
	ADDC	A,#0
	ADDC	A,0
	ADDC	A,@R0
	ADDC	A,@R1
	ADDC	A,R0
	ADDC	A,R1
	ADDC	A,R2
	ADDC	A,R3
	ADDC	A,R4
	ADDC	A,R5
	ADDC	A,R6
	ADDC	A,R7
	JC	$+2
	AJMP	2*256
	ORL	0,A
	ORL	0,#0
	ORL	A,#0
	ORL	A,0
	ORL	A,@R0
	ORL	A,@R1
	ORL	A,R0
	ORL	A,R1
	ORL	A,R2
	ORL	A,R3
	ORL	A,R4
	ORL	A,R5
	ORL	A,R6
	ORL	A,R7
	JNC	$+2
	ACALL	2*256
	ANL	0,A
	ANL	0,#0
	ANL	A,#0
	ANL	A,0
	ANL	A,@R0
	ANL	A,@R1
	ANL	A,R0
	ANL	A,R1
	ANL	A,R2
	ANL	A,R3
	ANL	A,R4
	ANL	A,R5
	ANL	A,R6
	ANL	A,R7
	JZ	$+2
	AJMP	3*256
	XRL	0,A
	XRL	0,#0
	XRL	A,#0
	XRL	A,0
	XRL	A,@R0
	XRL	A,@R1
	XRL	A,R0
	XRL	A,R1
	XRL	A,R2
	XRL	A,R3
	XRL	A,R4
	XRL	A,R5
	XRL	A,R6
	XRL	A,R7
	JNZ	$+2
	ACALL	3*256
	ORL	C,0
	JMP	@A+DPTR
	MOV	A,#0
	MOV	0,#0
	MOV	@R0,#0
	MOV	@R1,#0
	MOV	R0,#0
	MOV	R1,#0
	MOV	R2,#0
	MOV	R3,#0
	MOV	R4,#0
	MOV	R5,#0
	MOV	R6,#0
	MOV	R7,#0
	SJMP	$+2
	AJMP	4*256
	ANL	C,0
	MOVC	A,@A+PC
	DIV	AB
	MOV	0,0
	MOV	0,@R0
	MOV	0,@R1
	MOV	0,R0
	MOV	0,R1
	MOV	0,R2
	MOV	0,R3
	MOV	0,R4
	MOV	0,R5
	MOV	0,R6
	MOV	0,R7
	MOV	DPTR,#0
	ACALL	4*256
	MOV	0,C
	MOVC	A,@A+DPTR
	SUBB	A,#0
	SUBB	A,0
	SUBB	A,@R0
	SUBB	A,@R1
	SUBB	A,R0
	SUBB	A,R1
	SUBB	A,R2
	SUBB	A,R3
	SUBB	A,R4
	SUBB	A,R5
	SUBB	A,R6
	SUBB	A,R7
	ORL	C,/0
	AJMP	5*256
	MOV	C,0
	INC	DPTR
	MUL	AB

	MOV	@R0,0
	MOV	@R1,0
	MOV	R0,0
	MOV	R1,0
	MOV	R2,0
	MOV	R3,0
	MOV	R4,0
	MOV	R5,0
	MOV	R6,0
	MOV	R7,0
	ANL	C,/0
	ACALL	5*256
	CPL	0
	CPL	C
	CJNE	A,#0,$+3
	CJNE	A,0,$+3
	CJNE	@R0,#0,$+3
	CJNE	@R1,#0,$+3
	CJNE	R0,#0,$+3
	CJNE	R1,#0,$+3
	CJNE	R2,#0,$+3
	CJNE	R3,#0,$+3
	CJNE	R4,#0,$+3
	CJNE	R5,#0,$+3
	CJNE	R6,#0,$+3
	CJNE	R7,#0,$+3
	PUSH	0
	AJMP	6*256
	CLR	0
	CLR	C
	SWAP	A
	XCH	A,0
	XCH	A,@R0
	XCH	A,@R1
	XCH	A,R0
	XCH	A,R1
	XCH	A,R2
	XCH	A,R3
	XCH	A,R4
	XCH	A,R5
	XCH	A,R6
	XCH	A,R7
	POP	0
	ACALL	6*256
	SETB	0
	SETB	C
	DA	A
	DJNZ	0,$+3
	XCHD	A,@R0
	XCHD	A,@R1
	DJNZ	R0,$+2
	DJNZ	R1,$+2
	DJNZ	R2,$+2
	DJNZ	R3,$+2
	DJNZ	R4,$+2
	DJNZ	R5,$+2
	DJNZ	R6,$+2
	DJNZ	R7,$+2
	MOVX	A,@DPTR
	AJMP	7*256
	MOVX	A,@R0
	MOVX	A,@R1
	CLR	A
	MOV	A,0
	MOV	A,@R0
	MOV	A,@R1
	MOV	A,R0
	MOV	A,R1
	MOV	A,R2
	MOV	A,R3
	MOV	A,R4
	MOV	A,R5
	MOV	A,R6
	MOV	A,R7
	MOVX	@DPTR,A
	ACALL	7*256
	MOVX	@R0,A
	MOVX	@R1,A
	CPL	A
	MOV	0,A
	MOV	@R0,A
	MOV	@R1,A
	MOV	R0,A
	MOV	R1,A
	MOV	R2,A
	MOV	R3,A
	MOV	R4,A
	MOV	R5,A
	MOV	R6,A
	MOV	R7,A

*/
