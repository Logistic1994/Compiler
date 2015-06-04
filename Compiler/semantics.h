#include <vector>
#include <algorithm>
#include <fstream>
#include "parser.h"
#include "instruction.h"
//---------------- 函数绘图类的声明, 它是parser_class的派生类
class semantics_class : public parser_class {
protected:
	double 	Origin_x, Origin_y,			// 横、纵平移距离
		Scale_x, Scale_y, 				// 横、纵比例因子
		rot_angle;						// 旋转角度
	double color_r, color_g, color_b;	// 线条颜色
	int			line_width; // 线条粗细
	std::vector<instruction> v;
public:
	semantics_class()					// 声明对象时置下述初值
	{
		Origin_x = 0;	Origin_y = 0;	// 原点是(0, 0)
		Scale_x = 1;	Scale_y = 1;	// 横、纵坐标比例是1:1
		rot_angle = 0;					// 旋转角度是0
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
	// 重置基类parser_class中的方法，以在语法分析的基础上实现语法制导翻译
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

	// semantics_class中的方法，它们是语法制导翻译中所需的语义函数
	double get_tree_value(tree_node_ptr root);			// 获得表达式的值
	void cal_coord(tree_node_ptr hor_ptr,				// 计算点的坐标
		tree_node_ptr ver_ptr,
		double &hor_val,
		double &ver_val);
	void draw_pixel(double x, double y);				// 绘制一个点
	void draw_loop(double start_val,					// 图形绘制
		double end_val,
		double step_val,
		tree_node_ptr x_ptr,
		tree_node_ptr y_ptr);
	void delete_tree(tree_node_ptr root);				// 删除一棵树
};