#ifndef SCANNER_H_
#define SCANNER_H_
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>

//--------------------------------- �ʷ����������е����Ͷ����볣������
enum class token_type					// �Ǻ�����
{
	ORIGIN, SCALE, ROT, IS, TO,			// ������
	STEP, DRAW, FOR, FROM,				// ������
	COLOR, RED, GREEN, BLUE, BLACK,		// �����֣���ɫ����
	BACKGROUND,							// ������
	WIDTH,								// ������
	ANIMATION, ON, OFF,	SPEED, 			// ������
	VIEW_ORIGIN, VIEW_ROTATE, VIEW_SCALE,	// ������
	T,									// ����
	SEMICO, L_BRACKET, R_BRACKET, COMMA,// �ָ�����
	PLUS, MINUS, MUL, DIV, POWER,		// �����
	FUNC,								// ����
	CONST_ID,							// ����
	NONTOKEN,							// �ռǺ�
	ERRTOKEN							// ����Ǻ�
};
struct token_rec						// �Ǻ�����ű�ṹ
{
	token_type  	type;				// �Ǻŵ����
	char		* lexeme;				// ���ɼǺŵ��ַ���
	double		value;					// ��Ϊ���������ǳ�����ֵ
	double(*func_ptr)(double);			// ��Ϊ���������Ǻ�����ָ��
};
static token_rec token_table[] =		// ���ű�����
{ { token_type::CONST_ID, "PI", 3.1415926, NULL },
{ token_type::CONST_ID, "E", 2.71828, NULL },
{ token_type::T, "T", 0.0, NULL },
{ token_type::FUNC, "SIN", 0.0, sin },
{ token_type::FUNC, "COS", 0.0, cos },
{ token_type::FUNC, "TAN", 0.0, tan },
{ token_type::FUNC, "LN", 0.0, log },
{ token_type::FUNC, "EXP", 0.0, exp },
{ token_type::FUNC, "SQRT", 0.0, sqrt },
{ token_type::ORIGIN, "ORIGIN", 0.0, NULL },
{ token_type::SCALE, "SCALE", 0.0, NULL },
{ token_type::COLOR, "COLOR", 0.0, NULL },
{ token_type::RED, "RED", 0.0, NULL },
{ token_type::GREEN, "GREEN", 0.0, NULL },
{ token_type::BLUE, "BLUE", 0.0, NULL },
{ token_type::BLACK, "BLACK", 0.0, NULL },
{ token_type::BACKGROUND, "BACKGROUND", 0.0, NULL },
{ token_type::ANIMATION, "ANIMATION", 0.0, NULL },
{ token_type::ON, "ON", 0.0, NULL },
{ token_type::OFF, "OFF", 0.0, NULL },
{ token_type::SPEED, "SPEED", 0.0, NULL },
{ token_type::ROT, "ROT", 0.0, NULL },
{ token_type::WIDTH, "WIDTH", 0.0, NULL },
{ token_type::IS, "IS", 0.0, NULL },
{ token_type::FOR, "FOR", 0.0, NULL },
{ token_type::FROM, "FROM", 0.0, NULL },
{ token_type::TO, "TO", 0.0, NULL },
{ token_type::STEP, "STEP", 0.0, NULL },
{ token_type::DRAW, "DRAW", 0.0, NULL },
{ token_type::VIEW_ORIGIN, "VIEW_ORIGIN", 0.0, NULL },
{ token_type::VIEW_ROTATE, "VIEW_ROTATE", 0.0, NULL },
{ token_type::VIEW_SCALE, "VIEW_SCALE", 0.0, NULL }
};
const int  token_len = 100;			// �Ǻ���󳤶�
const char str_end = '\0';			// �ַ���������־

//--------------------------------- �ʷ��������������
class scanner_class {
protected:
	FILE *in_flie;					// �����ļ���
	char token_buf[token_len];		// �Ǻ��ַ�����

public:
	unsigned int line_no;				// ���ټǺ�����Դ�ļ��к�
	scanner_class()	{ line_no = 1; }
	~scanner_class() {}
	int init_scanner(const char *FileName);		// ��ʼ���ʷ�������
	void close_scanner(void);			// �رմʷ�������
	token_rec get_token(void);			// ��ȡ�Ǻź���

private:
	char get_char(void);				// ������Դ�����ж���һ���ַ�
	void back_char(char next_char);			// ��Ԥ�����ַ��˻ص�����Դ������
	void add_in_token_str(char next_char);		// �����ַ����ǺŻ�����
	void empty_token_str();				// ��ռǺŻ�����
	token_rec check_token(const char *c_str);	// �ж��������ַ����Ƿ��ڷ��ű���
};
#endif