#include "semantics.h"

// ------------------------ 计算被绘制点的坐标
void semantics_class::cal_coord(tree_node_ptr 	x_ptr,
	tree_node_ptr y_ptr,
	double &x_ret,
	double &y_ret) {
	// 计算表达式的值，得到点的原始坐标
	x_ret = get_tree_value(x_ptr);
	y_ret = get_tree_value(y_ptr);
}

// ------------------------ 循环绘制点坐标
void semantics_class::draw_loop
(double 		start_val,
double 		end_val,
double 		step_val,
tree_node_ptr 	x_ptr,
tree_node_ptr 	y_ptr)
{
	double x_val, y_val;
	// safety check
	if ((step_val > 0 && end_val < start_val) || (step_val < 0 && end_val > start_val) || step_val == 0.0) {
		parser_class::error_msg(parser_class::Scanner.line_no, "loop will be infinite!", "");
	}
	// todo: 这里的具体的步数不一定精确
	instruction inst_begin = instruction(inst::BEGINDRAW, int(abs((end_val - start_val)/step_val)) + 1);
	v.push_back(inst_begin);
	// int count = 0;
	if (step_val > 0.0) {
		for (parameter = start_val; parameter <= end_val; parameter += step_val)
		{
			cal_coord(x_ptr, y_ptr, x_val, y_val);
			draw_pixel(x_val, y_val);
			// count++;
		}
	}
	else {
		for (parameter = start_val; parameter >= end_val; parameter += step_val) {
			cal_coord(x_ptr, y_ptr, x_val, y_val);
			draw_pixel(x_val, y_val);
			// count++;
		}
	}
	//inst_begin.i1 = count;
	v.push_back(instruction(inst::ENDDRAW));
}

// ------------------------ 计算表达式的值
double semantics_class::get_tree_value(tree_node_ptr root)
{
	if (root == NULL) return 0.0;
	switch (root->op_code)
	{
	case token_type::PLUS:
		return get_tree_value(root->content.tag_op.left) + get_tree_value(root->content.tag_op.right);
	case token_type::MINUS:
		return get_tree_value(root->content.tag_op.left) - get_tree_value(root->content.tag_op.right);
	case token_type::MUL:
		return get_tree_value(root->content.tag_op.left) * get_tree_value(root->content.tag_op.right);
	case token_type::DIV:
		return get_tree_value(root->content.tag_op.left) / get_tree_value(root->content.tag_op.right);
	case token_type::POWER:
		return pow(get_tree_value(root->content.tag_op.left), get_tree_value(root->content.tag_op.right));
	case token_type::FUNC:
		return (*root->content.tag_func.math_func_ptr)	(get_tree_value(root->content.tag_func.child));
	case token_type::CONST_ID:
		return root->content.tag_const;
	case token_type::T:
		return *(root->content.tag_parameter);
	default:
		return 0.0;
	}
}

// ------------------------ 删除一棵语法树
void semantics_class::delete_tree(tree_node_ptr root)
{
	if (root == NULL) return;
	switch (root->op_code)
	{
	case token_type::PLUS:			// 两个孩子的内部节点
	case token_type::MINUS:
	case token_type::MUL:
	case token_type::DIV:
	case token_type::POWER:
		delete_tree(root->content.tag_op.left);
		delete_tree(root->content.tag_op.right);
		break;
	case token_type::FUNC: 			// 一个孩子的内部节点
		delete_tree(root->content.tag_func.child);
		break;
	default: 			// 叶子节点
		break;
	}
	delete(root);				// 删除节点
}

// ------------------------ 绘制一个点
void semantics_class::draw_pixel(double x, double y)
{
	v.push_back(instruction(inst::DRAW, x, y));
}

// ------------------------ origin_statement的递归子程序
void semantics_class::origin_statement()
{
	parser_class::origin_statement();		// 语法分析获取语法树（由parser中的属性保存）

	Origin_x = get_tree_value(x_ptr);		// 根据语法树计算横坐标的平移距离
	delete_tree(x_ptr);

	Origin_y = get_tree_value(y_ptr);		// 根据语法树计算纵坐标的平移距离
	delete_tree(y_ptr);
	v.push_back(instruction(inst::ORIGIN, Origin_x, Origin_y));
}

// ------------------------ scale_statement的递归子程序
void semantics_class::scale_statement()
{
	parser_class::scale_statement();		// 语法分析获取语法树（由parser中的属性保存）

	Scale_x = get_tree_value(x_ptr);		// 根据语法树计算横坐标的比例因子
	delete_tree(x_ptr);

	Scale_y = get_tree_value(y_ptr);		// 根据语法树计算纵坐标的比例因子
	delete_tree(y_ptr);
	v.push_back(instruction(inst::SCALE, Scale_x, Scale_y));
}

// ------------------------ rot_statement的递归子程序
void semantics_class::rot_statement()
{
	parser_class::rot_statement();			// 语法分析获取语法树（由parser中的属性保存）
	rot_angle = get_tree_value(angle_ptr);		// 根据语法树计算旋转角度
	delete_tree(angle_ptr);
	v.push_back(instruction(inst::ROT, rot_angle));
}

// ------------------------ width_statement 递归子程序
void semantics_class::width_statement() {
	parser_class::width_statement();
	double width = get_tree_value(width_ptr);
	delete_tree(width_ptr);
	if (width <= 1.0f) {
		line_width = 1;
	}
	else if (width >= 10.0f) {
		line_width = 10;
	}
	else {
		line_width = (int)width;
	}
	std::cout << "line width: Cast " << width << " to " << line_width << std::endl;
	v.push_back(instruction(inst::WIDTH, line_width));
}
// ------------------------ color_statement 的递归子程序
void semantics_class::color_statement() {
	parser_class::color_statement();
	v.push_back(instruction(inst::COLOR, color_r, color_g, color_b));
}
// ------------------------ bgcolor_statement的递归子程序
void semantics_class::bgcolor_statement() {
	parser_class::bgcolor_statement();
	v.push_back(instruction(inst::BGCOLOR, color_r, color_g, color_b));
}
// ------------------------ colors 的递归子程序
void semantics_class::colors() {
	parser_class::colors();
	double r = get_tree_value(r_ptr);
	double g = get_tree_value(g_ptr);
	double b = get_tree_value(b_ptr);
	if (r < 0.0) { r = 0.0; }
	if (r > 256.0) { r = 256.0; }
	if (g < 0.0) { g = 0.0; }
	if (g > 256.0) { g = 256.0; }
	if (b < 0.0) { b = 0.0; }
	if (b > 256.0) { b = 256.0; }
	color_r = r / 256.0;
	color_g = g / 256.0;
	color_b = b / 256.0;
	delete_tree(r_ptr);
	delete_tree(b_ptr);
	delete_tree(g_ptr);
}
// ------------------------ color的递归子程序
void semantics_class::color() {
	parser_class::color();
	if (color_type == token_type::RED) {
		color_r = 1.0; color_g = 0.0; color_b = 0.0;
	}
	else if (color_type == token_type::BLUE) {
		color_r = 0.0; color_g = 0.0; color_b = 1.0;
	}
	else if (color_type == token_type::GREEN) {
		color_r = 0.0; color_g = 1.0; color_b = 0.0;
	}
	else if (color_type == token_type::BLACK) {
		color_r = 0.0; color_g = 0.0; color_b = 0.0;
	}
}
// anim_statement
void semantics_class::anim_statement() {
	parser_class::anim_statement();
	v.push_back(instruction(inst::ANIMATION, anim));
}
// speed_statement
void semantics_class::speed_statement() {
	parser_class::speed_statement();
	double speed = get_tree_value(x_ptr);
	int si;
	if (speed <= 0) {
		si = 1;
	}
	else {
		si = (int)speed;
	}
	v.push_back(instruction(inst::SPEED, si));
	delete_tree(x_ptr);
}
void semantics_class::view_origin_statement() {
	parser_class::view_origin_statement();
	double x = get_tree_value(x_ptr);
	double y = get_tree_value(y_ptr);
	v.push_back(instruction(inst::VIEW_ORIGIN, x, y));
	delete_tree(x_ptr);
	delete_tree(y_ptr);
}
void semantics_class::view_rotate_statement() {
	parser_class::view_rotate_statement();
	double angle = get_tree_value(angle_ptr);
	v.push_back(instruction(inst::VIEW_ROTATE, angle));
	delete_tree(angle_ptr);
}
void semantics_class::view_scale_statement() {
	parser_class::view_scale_statement();
	double x = get_tree_value(x_ptr);
	double y = get_tree_value(y_ptr);
	delete_tree(x_ptr);
	delete_tree(y_ptr);
	if (x <= 0.0 || y <= 0.0) {
		parser_class::error_msg(parser_class::Scanner.line_no, "global view_scale cannot be smaller than 0!", "");
	}
	v.push_back(instruction(inst::VIEW_SCALE, x, y));
}
// ------------------------ for_statement 的递归子程序
void semantics_class::for_statement()
{
	double start_val, end_val, step_val;		// 绘图起点、终点、步长

	parser_class::for_statement();			// 语法分析获取各表达式的语法树（由parser中的属性保存）

	start_val = get_tree_value(start_ptr);		// 计算起点表达式的值
	end_val = get_tree_value(end_ptr);		// 计算终点表达式的值
	step_val = get_tree_value(step_ptr);		// 计算步长表达式的值

	draw_loop(start_val, end_val, step_val, x_ptr, y_ptr); // 绘图

	delete_tree(start_ptr);					// 释放空间
	delete_tree(end_ptr);
	delete_tree(step_ptr);
	delete_tree(x_ptr);
	delete_tree(y_ptr);
}