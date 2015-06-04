#include "parser.h"
#include <iostream>
using namespace std;

// ------------------------ 通过词法分析器接口get_token获取一个记号
void parser_class::fetch_token()
{
	token = Scanner.get_token();
	if (token.type == token_type::ERRTOKEN)	syntax_error(1);
}

// ------------------------ 匹配记号
void parser_class::match_token(token_type the_token)
{
	if (token.type != the_token) syntax_error(2);
	fetch_token();
}

// ------------------------ 语法错误处理
void parser_class::syntax_error(int case_of)
{
	switch (case_of)
	{
	case 1:	error_msg(Scanner.line_no, " 非法记号 ", token.lexeme);
		break;
	case 2: error_msg(Scanner.line_no, token.lexeme, " 不是预期记号");
		break;
	}
}

// ------------------------ 打印错误信息
void parser_class::error_msg(int line, char *descrip, char *string)
{
	cout << "Line No " << line << ": " << descrip << string << endl;
	Scanner.close_scanner();
	exit(1);
}

// ------------------------ 先序遍历并打印表达式的语法树
void parser_class::print_syntax_tree(tree_node_ptr root, int indent)
{
	int		temp;

	for (temp = 1; temp <= indent; temp++) cout << "	";	// 缩进
	switch (root->op_code)					// 打印根节点
	{
	case token_type::PLUS:      cout << "+" << endl;	break;
	case token_type::MINUS:     cout << "-" << endl;	break;
	case token_type::MUL:       cout << "*" << endl;	break;
	case token_type::DIV:       cout << "/" << endl;	break;
	case token_type::POWER:     cout << "**" << endl;	break;
	case token_type::FUNC:      cout << root->content.tag_func.math_func_ptr << endl;	break;
	case token_type::CONST_ID:  cout << root->content.tag_const << endl;			break;
	case token_type::T:         cout << "T" << endl;					break;
	default:        cout << "非法的树节点!" << endl;			exit(0);
	}
	if (root->op_code == token_type::CONST_ID || root->op_code == token_type::T) return;		// 叶子节点返回
	if (root->op_code == token_type::FUNC)						// 递归打印一个孩子的节点
		print_syntax_tree(root->content.tag_func.child, indent + 1);
	else									// 递归打印两个孩子的节点
	{
		print_syntax_tree(root->content.tag_op.left, indent + 1);
		print_syntax_tree(root->content.tag_op.right, indent + 1);
	}
}

// ------------------------ 绘图语言解释器入口（与主程序的外部接口）
void parser_class::parser(const char * file_name)
{
	enter("parser");
	if (!(Scanner.init_scanner(file_name)))		// 初始化词法分析器
	{
		cout << "打开文件错误 !" << endl;
		return;
	}
	fetch_token();					// 获取第一个记号
	program();					// 递归下降分析
	Scanner.close_scanner();			// 关闭词法分析器
	back("parser");
	return;
}

// ------------------------ program 的递归子程序
void parser_class::program()
{
	enter("program");
	while (token.type != token_type::NONTOKEN)
	{
		statement();
		match_token(token_type::SEMICO);
	}
	back("program");
}

// ------------------------ statement 的递归子程序
void parser_class::statement()
{
	enter("statement");
	switch (token.type)
	{
	case token_type::ORIGIN:	origin_statement();	break;
	case token_type::SCALE:	scale_statement();	break;
	case token_type::ROT:	rot_statement();	break;
	case token_type::FOR:	for_statement();	break;
	case token_type::COLOR:	color_statement();  	break;
	case token_type::WIDTH: width_statement();		break;
	case token_type::BACKGROUND: bgcolor_statement(); break;
	case token_type::ANIMATION:	anim_statement(); break;
	case token_type::SPEED:	speed_statement(); break;
	case token_type::VIEW_ORIGIN: view_origin_statement(); break;
	case token_type::VIEW_ROTATE: view_rotate_statement(); break;
	case token_type::VIEW_SCALE: view_scale_statement(); break;
	default:	syntax_error(2);
	}
	back("statement");
}

// ------------------------ origin_statement 的递归子程序
void parser_class::origin_statement()
{
	enter("origin_statement");
	call_match("ORIGIN");		match_token(token_type::ORIGIN);
	call_match("IS");		match_token(token_type::IS);
	call_match("(");		match_token(token_type::L_BRACKET);
	x_ptr = expression();
	call_match(",");		match_token(token_type::COMMA);
	y_ptr = expression();
	call_match(")");		match_token(token_type::R_BRACKET);
	back("origin_statement");
}

// ------------------------ scale_statement 的递归子程序
void parser_class::scale_statement()
{
	enter("scale_statement");
	call_match("SCALE");		match_token(token_type::SCALE);
	call_match("IS");		match_token(token_type::IS);
	call_match("(");		match_token(token_type::L_BRACKET);
	x_ptr = expression();
	call_match(",");		match_token(token_type::COMMA);
	y_ptr = expression();
	call_match(")");		match_token(token_type::R_BRACKET);
	back("scale_statement");
}

// ------------------------ rot_statement 的递归子程序
void parser_class::rot_statement()
{
	enter("rot_statement");
	call_match("ROT");		match_token(token_type::ROT);
	call_match("IS");		match_token(token_type::IS);
	angle_ptr = expression();
	back("rot_statement");
}

// ------------------------ color_statement 的递归子程序
void parser_class::width_statement() {
	enter("width_statement");
	match_token(token_type::WIDTH);
	match_token(token_type::IS);
	width_ptr = expression();
	back("width_statement");
}
// ------------------------ color_statement 的递归子程序
void parser_class::color_statement()
{
	enter("color_statement");
	match_token(token_type::COLOR);
	match_token(token_type::IS);
	if (token.type == token_type::L_BRACKET) {
		colors();
	}
	else {
		color();
	}
	back("color_statement");
}
// ------------------------ bgcolor_statement递归子程序
void parser_class::bgcolor_statement() {
	enter("bgcolor_statement");
	match_token(token_type::BACKGROUND);
	match_token(token_type::IS);
	if (token.type == token_type::L_BRACKET) {
		colors();
	}
	else {
		color();
	}
	back("bgcolor_statement");
}
// ------------------------ color 递归子程序
void parser_class::color() {
	enter("specify a color with name");
	color_type = token.type;
	// consume the token
	match_token(token.type);
	back("specify a color with name");
}
// ------------------------ colors 递归子程序
void parser_class::colors() {
	enter("specify a color with 3 parameters");
	match_token(token_type::L_BRACKET);
	r_ptr = expression();
	match_token(token_type::COMMA);
	g_ptr = expression();
	match_token(token_type::COMMA);
	b_ptr = expression();
	match_token(token_type::R_BRACKET);
	back("specify a color with 3 parameters");
}
// animation statement
void parser_class::anim_statement() {
	enter("anim_statement");
	match_token(token_type::ANIMATION);
	if (token.type == token_type::ON) {
		std::cout << "animation is on" << std::endl;
		anim = true;
		match_token(token_type::ON);
	}
	else {
		std::cout << "animation is off" << std::endl;
		anim = false;
		match_token(token_type::OFF);
	}

}
// speed_statement
void parser_class::speed_statement() {
	enter("speed_statement");
	match_token(token_type::SPEED);
	match_token(token_type::IS);
	x_ptr = expression();
	back("speed_statement");
}

// view-origin view_origin is (3, 4);
void parser_class::view_origin_statement() {
	enter("view_origin_statment");
	match_token(token_type::VIEW_ORIGIN);
	match_token(token_type::IS);
	match_token(token_type::L_BRACKET);
	x_ptr = expression();
	match_token(token_type::COMMA);
	y_ptr = expression();
	match_token(token_type::R_BRACKET);
	back("view_origin_statment");
}
// view_rotate view_rotate is pi;
void parser_class::view_rotate_statement() {
	enter("view_rotate_statement");
	match_token(token_type::VIEW_ROTATE);
	match_token(token_type::IS);
	angle_ptr = expression();
	back("view_rotate_statement");
}
// view_scale view_scale is (x, y);
void parser_class::view_scale_statement() {
	enter("view_scale_statement");
	match_token(token_type::VIEW_SCALE);
	match_token(token_type::IS);
	match_token(token_type::L_BRACKET);
	x_ptr = expression();
	match_token(token_type::COMMA);
	y_ptr = expression();
	match_token(token_type::R_BRACKET);
	back("view_scale_statement");
}
// ------------------------ for_statement 的递归子程序
void parser_class::for_statement()
{
	enter("for_statement");
	call_match("FOR");			match_token(token_type::FOR);
	call_match("T");			match_token(token_type::T);
	call_match("FROM");			match_token(token_type::FROM);
	start_ptr = expression();	// 构造参数起始表达式语法树
	call_match("TO");			match_token(token_type::TO);
	end_ptr = expression();		// 构造参数终结表达式语法树
	call_match("STEP");			match_token(token_type::STEP);
	step_ptr = expression();	// 构造参数步长表达式语法树
	call_match("DRAW");			match_token(token_type::DRAW);
	call_match("(");			match_token(token_type::L_BRACKET);
	x_ptr = expression();		// 构造横坐标表达式语法树
	call_match(",");			match_token(token_type::COMMA);
	y_ptr = expression(); 		// 构造纵坐标表达式语法树
	call_match(")");			match_token(token_type::R_BRACKET);
	back("for_statement");
}

// ------------------------ expression 的递归子程序
tree_node_ptr parser_class::expression()
{
	tree_node_ptr left, right;			// 左右子树节点的指针
	token_type token_tmp;				// 当前记号

	enter("expression");
	left = term();					// 分析左操作数且得到其语法树
	while (token.type == token_type::PLUS || token.type == token_type::MINUS)
	{
		token_tmp = token.type;
		match_token(token_tmp);
		right = term();				// 分析右操作数且得到其语法树
		left = make_tree_node(token_tmp, left, right);
		// 构造运算的语法树，结果为左子树
	}
	tree_trace(left);				// 打印表达式的语法树
	back("expression");
	return left;					// 返回最终表达式的语法树
}

// ------------------------ term 的递归子程序
tree_node_ptr  parser_class::term()
{
	tree_node_ptr left, right;
	token_type token_tmp;

	left = factor();
	while (token.type == token_type::MUL || token.type == token_type::DIV)
	{
		token_tmp = token.type;
		match_token(token_tmp);
		right = factor();
		left = make_tree_node(token_tmp, left, right);
	}
	return left;
}

// ------------------------ factor 的递归子程序
tree_node_ptr parser_class::factor()
{
	tree_node_ptr left, right;

	if (token.type == token_type::PLUS) 			// 匹配一元加运算
	{
		match_token(token_type::PLUS);
		right = factor();		//  表达式退化为仅有右操作数的表达式
	}
	else if (token.type == token_type::MINUS)		// 匹配一元减运算
	{
		match_token(token_type::MINUS); 		// 表达式转化为二元减运算的表达式
		right = factor();
		left = new tree_node;
		left->op_code = token_type::CONST_ID;
		left->content.tag_const = 0.0;
		right = make_tree_node(token_type::MINUS, left, right);
	}
	else right = component();		// 匹配非终结符component
	return right;
}

// ------------------------ component 的递归子程序
tree_node_ptr parser_class::component()
{
	tree_node_ptr left, right;

	left = atom();
	if (token.type == token_type::POWER)
	{
		match_token(token_type::POWER);
		right = component();	// 递归调用component以实现POWER的右结合性质
		left = make_tree_node(token_type::POWER, left, right);
	}
	return left;
}

// ------------------------ atom 的递归子程序
tree_node_ptr parser_class::atom()
{
	struct token_rec t = token;
	tree_node_ptr t_ptr = NULL, tmp;

	switch (token.type)
	{
	case token_type::CONST_ID:
		match_token(token_type::CONST_ID);
		t_ptr = make_tree_node(token_type::CONST_ID, t.value);
		break;
	case token_type::T:
		match_token(token_type::T);
		t_ptr = make_tree_node(token_type::T);
		break;
	case token_type::FUNC:
		match_token(token_type::FUNC);
		match_token(token_type::L_BRACKET);
		tmp = expression();
		t_ptr = make_tree_node(token_type::FUNC, t.func_ptr, tmp);
		match_token(token_type::R_BRACKET);
		break;
	case token_type::L_BRACKET:
		match_token(token_type::L_BRACKET);
		t_ptr = expression();
		match_token(token_type::R_BRACKET);
		break;
	default:
		syntax_error(2);
	}
	return t_ptr;
}

// ----------------- 生成语法树的一个节点
tree_node_ptr parser_class::make_tree_node(token_type opcode, ...)
{
	tree_node_ptr t_ptr = new (struct tree_node);		// 分配节点存储空间
	t_ptr->op_code = opcode;				// 接收记号的类别
	va_list arg_ptr;
	va_start(arg_ptr, opcode);
	switch (opcode)						// 根据记号的类别构造不同的节点
	{
	case token_type::CONST_ID:					// 常数节点
		t_ptr->content.tag_const = (double)va_arg(arg_ptr, double);
		break;
	case token_type::T:						// 参数节点
		t_ptr->content.tag_parameter = &parameter;
		break;
	case token_type::FUNC:					// 函数调用节点
		t_ptr->content.tag_func.math_func_ptr = (func_ptr)va_arg(arg_ptr, func_ptr);
		t_ptr->content.tag_func.child
			= tree_node_ptr va_arg(arg_ptr, tree_node_ptr);
		break;
	default:					// 二元运算节点
		t_ptr->content.tag_op.left
			= tree_node_ptr va_arg(arg_ptr, tree_node_ptr);
		t_ptr->content.tag_op.right
			= tree_node_ptr va_arg(arg_ptr, tree_node_ptr);
		break;
	}
	va_end(arg_ptr);
	return t_ptr;
}

// ---------------------- 用于语法分析器中的跟踪调试
void parser_class::enter(char * x)		{ cout << "enter in  " << x << endl; }
void parser_class::back(char * x)		{ cout << "exit from  " << x << endl; }
void parser_class::call_match(char * x)		{ cout << "matchtoken	" << x << endl; }
void parser_class::tree_trace(tree_node_ptr x)	{ print_syntax_tree(x, 1); }