#ifndef SANDBOX_H_
#define SANDBOX_H_
#include <vector>
#include <algorithm>
#include <gl/glut.h>
#include "instruction.h"
class sandbox {
public:
	bool finished;				// 是否已经播放结束
	bool anim;					// 是否已动画形式播放，动画只播放一遍
	sandbox(const std::vector<instruction>& v) : v(v) {
		width = 1;
		played_flame = 0;
		origin_x = 0.0; origin_y = 0.0;
		rot_angle = 0.0;
		scale_x = 1.0; scale_y = 1.0;
		in_loop = false;
		finished = false;
		has_propressed = false;
		total_draw_instructions = 0;
		speed = 1;
		view_origin_x = 0.0; view_origin_y = 0.0;
		view_rot_angle = 0.0;
		view_scale_x = 1.0; view_scale_y = 1.0;
		static_replayed = false;
	}
	void preprocess() {
		// 先获取必要的一些参数
		const instruction& first = v.at(0);
		if (first.name == inst::ANIMATION && first.b1) {
			anim = true;
			const instruction& second = v.at(1);
			if (second.name == inst::SPEED) {
				speed = second.i1;
			}
			else {
				speed = 1;
			}
		}
		else {
			anim = false;
		}
		// 获取指令中一共有多少条draw数据
		std::for_each(v.begin(), v.end(), [&](const instruction& v){
			if (v.name == inst::DRAW) {
				total_draw_instructions++;
			}
		});
		std::cout << "ani:" << anim << " speed:" << speed << std::endl;
	}
	void play();
	void changed_size(GLsizei w, GLsizei h);

	bool static_replayed;

private:
	const std::vector<instruction>& v;
	int width, speed; // 当前的线宽和速度
	int played_flame; // 已经播放的帧数
	double origin_x, origin_y;	// 当前平移点
	double rot_angle;			// 当前角度
	double scale_x, scale_y;	// 当前的缩放比例
	bool in_loop;				// 是否在循环中等待出来

	double view_origin_x, view_origin_y;
	double view_rot_angle;
	double view_scale_x, view_scale_y;
	
	double view_target_origin_x, view_target_origin_y;
	double view_target_rot_angle;
	double view_target_scale_x, view_target_scale_y;

	double view_delta_ox, view_delta_oy;
	double view_delta_r;
	double view_delta_sx, view_delta_sy;
	
	bool in_gradual_origin, in_gradual_rot, in_gradual_scale;
	
	bool has_propressed; 
	int total_draw_instructions;

	void convert(double, double, double&, double&);
	void set_defaults();

	void init_gradual(int times);
	void process_gradual();
	void end_gradual();
	void apply_global_parameters();

	int w_w, w_h; // window width & height

	
};
#endif