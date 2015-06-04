#include <vector>
#include <algorithm>
#include <fstream>
#include "parser.h"
#include "instruction.h"
//---------------- ������ͼ�������, ����parser_class��������
class semantics_class : public parser_class {
protected:
	double 	Origin_x, Origin_y,			// �ᡢ��ƽ�ƾ���
		Scale_x, Scale_y, 				// �ᡢ�ݱ�������
		rot_angle;						// ��ת�Ƕ�
	double color_r, color_g, color_b;	// ������ɫ
	int			line_width; // ������ϸ
	std::vector<instruction> v;
public:
	semantics_class()					// ��������ʱ��������ֵ
	{
		Origin_x = 0;	Origin_y = 0;	// ԭ����(0, 0)
		Scale_x = 1;	Scale_y = 1;	// �ᡢ�����������1:1
		rot_angle = 0;					// ��ת�Ƕ���0
		line_width = 1;
		color_r = 0.0; color_g = 0.0; color_b = 0.0;
	};
	~semantics_class() {};

	void parser(const char * file_name) override {
		v.clear();
		parser_class::parser(file_name);
	}

	void show_instructions(std::ostream& o) {
		std::for_each(v.begin(), v.end(), [&](const instruction& v) {o << v; });
	}
	const std::vector<instruction>& get_instructions() {
		return v;
	}

private:
	// ���û���parser_class�еķ����������﷨�����Ļ�����ʵ���﷨�Ƶ�����
	void for_statement() override;
	void origin_statement() override;
	void rot_statement() override;
	void scale_statement() override;
	void width_statement() override;
	void color_statement() override;
	void bgcolor_statement() override;
	void color() override;
	void colors() override;
	void anim_statement() override;
	void speed_statement() override;
	void view_origin_statement() override;
	void view_rotate_statement() override;
	void view_scale_statement() override;

	// semantics_class�еķ������������﷨�Ƶ���������������庯��
	double get_tree_value(tree_node_ptr root);			// ��ñ��ʽ��ֵ
	void cal_coord(tree_node_ptr hor_ptr,				// ����������
		tree_node_ptr ver_ptr,
		double &hor_val,
		double &ver_val);
	void draw_pixel(double x, double y);				// ����һ����
	void draw_loop(double start_val,					// ͼ�λ���
		double end_val,
		double step_val,
		tree_node_ptr x_ptr,
		tree_node_ptr y_ptr);
	void delete_tree(tree_node_ptr root);				// ɾ��һ����
};