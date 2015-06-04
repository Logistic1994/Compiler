#include "scanner.h"

//--------------------------------- �﷨���������е����Ͷ���
typedef double(*func_ptr)(double);
typedef struct tree_node				// �﷨���ڵ�����
{
	token_type op_code;			// PLUS, FUNC, CONST_ID, ...
	union
	{
		struct { tree_node *left, *right; } tag_op;
		struct { tree_node *child; func_ptr math_func_ptr; } tag_func;
		double tag_const;
		double * tag_parameter;
	} content;
} *tree_node_ptr;

//--------------------------------- �﷨������������
class parser_class {

protected:
	double    	parameter;	// ����T�Ĵ洢�ռ�
	token_type	color_type;  // ��ɫ�Ǻ�
	token_rec 	token;		// �Ǻ�
	tree_node_ptr 	start_ptr,	// ��ͼ�����ʽ���﷨��
		end_ptr, 	// ��ͼ�յ���ʽ���﷨��
		step_ptr,	// �������ʽ���﷨��
		x_ptr,		// ��ĺ�������ʽ���﷨��
		y_ptr,		// ��ĺ�������ʽ���﷨��
		angle_ptr,	// ��ת�Ƕȱ��ʽ���﷨��
		width_ptr,  // ������ϸ���ʽ���﷨��
		r_ptr,		// r
		g_ptr,		// g
		b_ptr;		// b
	bool anim;
	scanner_class 	Scanner;	// �ʷ�����������

	// --------------- ��������
	void fetch_token();						// ��ȡ�Ǻ�
	void match_token(token_type the_token);			// ƥ��Ǻ�

	tree_node_ptr make_tree_node(token_type opcode, ...);	// �����﷨��
	void print_syntax_tree(tree_node *root, int indent);		// ��ӡ�﷨��

	void syntax_error(int case_of);				// ָ���﷨����(����error_msg)	
	void error_msg(int line, char *descrip, char *string);	// ��Semantics������Ϊ���ڴ�ӡ��ʽ


	// ---------------���ս���ĵݹ��ӳ���
	void program();
	void statement();
	// ����������Semantics_class�������ã���ʵ���﷨�Ƶ�����
	virtual void for_statement();
	virtual void origin_statement();
	virtual void rot_statement();
	virtual void width_statement();
	virtual void scale_statement();
	virtual void color_statement();
	virtual void bgcolor_statement();
	virtual void colors();
	virtual void color();
	virtual void anim_statement();
	virtual void speed_statement();
	virtual void view_origin_statement();
	virtual void view_rotate_statement();
	virtual void view_scale_statement();

	tree_node_ptr expression();
	tree_node_ptr term();
	tree_node_ptr factor();
	tree_node_ptr component();
	tree_node_ptr atom();

public:
	parser_class() 				// ��������ʱ���ó�ֵ
	{
		parameter = 0;
		start_ptr = NULL;
		end_ptr = NULL;
		step_ptr = NULL;
		x_ptr = NULL;
		y_ptr = NULL;
	}
	~parser_class() {}
	virtual void parser(const char * file_name);		// �﷨�������ӿ�

private:
	// �������������﷨�������ĸ��ٵ���, ��Semantics_class��������Ϊ��������
	void enter(char * x);
	void back(char * x);
	void call_match(char * x);
	void tree_trace(tree_node_ptr x);
};