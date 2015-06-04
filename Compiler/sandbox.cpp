#include "sandbox.h"
#include <gl/glut.h>
// 另一种对于view系列指令支持的方法是
// 预处理的时候直接读出最终的origin, scale, rot
// 这样就可以实现在anim == off时，需要两遍绘图了
void sandbox::play() {
	set_defaults();
	int played_count = 0;
	bool should_stop = false;
	for (std::vector<instruction>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (should_stop) {
			break;
		}
		const instruction& the_inst = *it;
		// std::cout << the_inst;
		switch (the_inst.name) {
		case inst::BGCOLOR:
			if (played_count != 0) {
				std::cout << "BACKGROUND必须在绘图指令之前使用，此处被省略" << std::endl;
			}
			else {
				glClearColor(the_inst.x1, the_inst.x2, the_inst.x3, 0.5);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			break;
		case inst::ORIGIN:
			origin_x = the_inst.x1;
			origin_y = the_inst.x2;
			break;
		case inst::ROT:
			rot_angle = the_inst.x1;
			break;
		case inst::SCALE:
			scale_x = the_inst.x1;
			scale_y = the_inst.x2;
			break;
		case inst::BEGINDRAW:
			glBegin(GL_POINTS);
			in_loop = true;
			init_gradual(the_inst.i1);
			break;
		case inst::ENDDRAW:
			glEnd();
			in_loop = false;
			end_gradual();
			break;
		case inst::COLOR:
			glColor3d(the_inst.x1, the_inst.x2, the_inst.x3);
			break;
		case inst::WIDTH:
			width = the_inst.i1;
			break;
		case inst::DRAW:
			double x, y;
			convert(the_inst.x1, the_inst.x2, x, y);
			for (int i = -width / 2; i < (width + 1) / 2; i++) {
				for (int j = -width / 2; j < (width + 1) / 2; j++) {
					glVertex2d(x + i, y + j);
				}
			}
			if (anim) {
				played_count++;
				if (played_count == total_draw_instructions) {
					anim = false;
					finished = true;
					should_stop = true;
				}
				if (played_flame + speed == played_count) {
					played_flame = played_count;
					should_stop = true;
				}
			}
			process_gradual();
			break;
		case inst::VIEW_ORIGIN:
			view_target_origin_x = the_inst.x1;
			view_target_origin_y = the_inst.x2;
			in_gradual_origin = true;
			break;
		case inst::VIEW_ROTATE:
			view_target_rot_angle = the_inst.x1;
			in_gradual_rot = true;
			break;
		case inst::VIEW_SCALE:
			view_target_scale_x = the_inst.x1;
			view_target_scale_y = the_inst.x2;
			in_gradual_scale = true;
			break;
		default:
			break;
		}
	};
	if (in_loop) {
		glEnd();
		in_loop = false;
		end_gradual();
	}
	apply_global_parameters();
	if (!anim) {
		finished = true;
		if (!static_replayed) {
			static_replayed = true;
			this->play();
		}
	}
}

void sandbox::convert(double x, double y, double& newx, double& newy) {
	x *= scale_x;
	y *= scale_y;
	double x_tmp = x * cos(rot_angle) + y * sin(rot_angle);
	y = y * cos(rot_angle) - x * sin(rot_angle);
	x = x_tmp;
	x += origin_x;
	y += origin_y;
	newx = x;
	newy = y;
}

void sandbox::set_defaults() {
	// 只有anim 和speed是刚开始的时候就设定好的
	in_loop = false;
	width = 0;
	origin_x = 0.0;
	origin_y = 0.0;
	rot_angle = 0.0;
	scale_x = 1.0;
	scale_y = 1.0;
	view_origin_x = 0.0;
	view_origin_y = 0.0;
	view_rot_angle = 0.0;
	view_scale_x = 1.0;
	view_scale_y = 1.0;
	view_target_origin_x = view_target_origin_y = 0.0;
	view_target_rot_angle = 0.0;
	view_target_scale_x = view_target_scale_y = 0.0;
	in_gradual_origin = false;
	in_gradual_rot = false;
	in_gradual_scale = false;
	glColor3d(0.0, 0.0, 0.0);
}

void sandbox::process_gradual() {
	if (in_gradual_origin) {
		view_origin_x += view_delta_ox;
		view_origin_y += view_delta_oy;
	}
	if (in_gradual_rot) {
		view_rot_angle += view_delta_r;
	}
	if (in_gradual_scale) {
		view_scale_x += view_delta_sx;
		view_scale_y += view_delta_sy;
	}
}
void sandbox::init_gradual(int times) {
	if (in_gradual_origin) {
		view_delta_ox = (view_target_origin_x - view_origin_x) / times;
		view_delta_oy = (view_target_origin_y - view_origin_y) / times;
	}
	if (in_gradual_rot) {
		view_delta_r = (view_target_rot_angle - view_rot_angle) / times;
	}
	if (in_gradual_scale) {
		view_delta_sx = (view_target_scale_x - view_scale_x) / times;
		view_delta_sy = (view_target_scale_y - view_scale_y) / times;
	}
}
void sandbox::end_gradual() {
	in_gradual_origin = false;
	in_gradual_rot = false;
	in_gradual_scale = false;
}
void sandbox::apply_global_parameters() {
	double ox = view_origin_x; double oy = view_origin_y;
	double sx = view_scale_x; double sy = view_scale_y;
	glViewport(0, 0, w_w, w_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-w_w / 2/sx  + ox, w_w / 2/sx + ox, -w_h / 2/sy + oy, w_h / 2/sy + oy);
	std::cout << "oo" << -w_w / 2 / sx + ox << "," << w_w / 2 / sx + ox << ',' << -w_h / 2 / sy + oy << "," << w_h / 2 / sy + oy << std::endl;
	std::cout <<"apply"<< ox << " " << oy << " " << sx << " " << sy <<" "<<w_w<<" "<<w_h<< std::endl;
}
void sandbox::changed_size(GLsizei w, GLsizei h) {
	w_w = w;
	w_h = h;
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	double ox = view_origin_x; double oy = view_origin_y;
	double sx = view_scale_x; double sy = view_scale_y;
	if (!anim && static_replayed) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-w_w / 2 / sx + ox, w_w / 2 / sx + ox, -w_h / 2 / sy + oy, w_h / 2 / sy + oy);
	}
	std::cout <<"changedSize"<< ox << " " << oy << " " << sx << " " << sy << " " << w_w << " " << w_h << std::endl;
}