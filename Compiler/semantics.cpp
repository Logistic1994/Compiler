#include "semantics.h"

// ------------------------ ���㱻���Ƶ������
void semantics_class::cal_coord(tree_node_ptr 	x_ptr,
	tree_node_ptr y_ptr,
	double &x_ret,
	double &y_ret) {
	// ������ʽ��ֵ���õ����ԭʼ����
	x_ret = get_tree_value(x_ptr);
	y_ret = get_tree_value(y_ptr);
}

// ------------------------ ѭ�����Ƶ�����
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
	// todo: ����ľ���Ĳ�����һ����ȷ
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

// ------------------------ ������ʽ��ֵ
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

// ------------------------ ɾ��һ���﷨��
void semantics_class::delete_tree(tree_node_ptr root)
{
	if (root == NULL) return;
	switch (root->op_code)
	{
	case token_type::PLUS:			// �������ӵ��ڲ��ڵ�
	case token_type::MINUS:
	case token_type::MUL:
	case token_type::DIV:
	case token_type::POWER:
		delete_tree(root->content.tag_op.left);
		delete_tree(root->content.tag_op.right);
		break;
	case token_type::FUNC: 			// һ�����ӵ��ڲ��ڵ�
		delete_tree(root->content.tag_func.child);
		break;
	default: 			// Ҷ�ӽڵ�
		break;
	}
	delete(root);				// ɾ���ڵ�
}

// ------------------------ ����һ����
void semantics_class::draw_pixel(double x, double y)
{
	v.push_back(instruction(inst::DRAW, x, y));
}

// ------------------------ origin_statement�ĵݹ��ӳ���
void semantics_class::origin_statement()
{
	parser_class::origin_statement();		// �﷨������ȡ�﷨������parser�е����Ա��棩

	Origin_x = get_tree_value(x_ptr);		// �����﷨������������ƽ�ƾ���
	delete_tree(x_ptr);

	Origin_y = get_tree_value(y_ptr);		// �����﷨�������������ƽ�ƾ���
	delete_tree(y_ptr);
	v.push_back(instruction(inst::ORIGIN, Origin_x, Origin_y));
}

// ------------------------ scale_statement�ĵݹ��ӳ���
void semantics_class::scale_statement()
{
	parser_class::scale_statement();		// �﷨������ȡ�﷨������parser�е����Ա��棩

	Scale_x = get_tree_value(x_ptr);		// �����﷨�����������ı�������
	delete_tree(x_ptr);

	Scale_y = get_tree_value(y_ptr);		// �����﷨������������ı�������
	delete_tree(y_ptr);
	v.push_back(instruction(inst::SCALE, Scale_x, Scale_y));
}

// ------------------------ rot_statement�ĵݹ��ӳ���
void semantics_class::rot_statement()
{
	parser_class::rot_statement();			// �﷨������ȡ�﷨������parser�е����Ա��棩
	rot_angle = get_tree_value(angle_ptr);		// �����﷨��������ת�Ƕ�
	delete_tree(angle_ptr);
	v.push_back(instruction(inst::ROT, rot_angle));
}

// ------------------------ width_statement �ݹ��ӳ���
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
// ------------------------ color_statement �ĵݹ��ӳ���
void semantics_class::color_statement() {
	parser_class::color_statement();
	v.push_back(instruction(inst::COLOR, color_r, color_g, color_b));
}
// ------------------------ bgcolor_statement�ĵݹ��ӳ���
void semantics_class::bgcolor_statement() {
	parser_class::bgcolor_statement();
	v.push_back(instruction(inst::BGCOLOR, color_r, color_g, color_b));
}
// ------------------------ colors �ĵݹ��ӳ���
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
// ------------------------ color�ĵݹ��ӳ���
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
// ------------------------ for_statement �ĵݹ��ӳ���
void semantics_class::for_statement()
{
	double start_val, end_val, step_val;		// ��ͼ��㡢�յ㡢����

	parser_class::for_statement();			// �﷨������ȡ�����ʽ���﷨������parser�е����Ա��棩

	start_val = get_tree_value(start_ptr);		// ���������ʽ��ֵ
	end_val = get_tree_value(end_ptr);		// �����յ���ʽ��ֵ
	step_val = get_tree_value(step_ptr);		// ���㲽�����ʽ��ֵ

	draw_loop(start_val, end_val, step_val, x_ptr, y_ptr); // ��ͼ

	delete_tree(start_ptr);					// �ͷſռ�
	delete_tree(end_ptr);
	delete_tree(step_ptr);
	delete_tree(x_ptr);
	delete_tree(y_ptr);
}