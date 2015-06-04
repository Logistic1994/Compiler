#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_
#include <iostream>
enum class inst {
	ANIMATION, SPEED, BGCOLOR, ORIGIN, ROT, SCALE, BEGINDRAW, ENDDRAW, COLOR, WIDTH, DRAW, VIEW_ORIGIN, VIEW_ROTATE, VIEW_SCALE
};
class instruction {
public:
	instruction(inst name) :name(name) {}
	instruction(inst name, bool b1) : name(name), b1(b1) {}
	instruction(inst name, int i1) : name(name), i1(i1) {}
	instruction(inst name, double x1) : name(name), x1(x1) {}
	instruction(inst name, double x1, double x2) : name(name), x1(x1), x2(x2) {}
	instruction(inst name, double x1, double x2, double x3) : name(name), x1(x1), x2(x2), x3(x3) {}
	
	friend std::ostream& operator<<(std::ostream &os, const instruction& i) {
		switch (i.name) {
		case inst::ANIMATION:
			return os << "ANIMATION " << (i.b1?"true":"false") << std::endl;
		case inst::SPEED:
			//std::cout << "SPEED" << i.i1 << std::endl;
			return os << "SPEED " << i.i1 << std::endl;
		case inst::BGCOLOR:
			return os << "BGCOLOR " << i.x1 << "," << i.x2 << "," << i.x3 << std::endl;
		case inst::ORIGIN:
			return os << "ORIGIN " << i.x1 << "," << i.x2 << std::endl;
		case inst::ROT:
			return os << "ROT " << i.x1 << std::endl;
		case inst::SCALE:
			return os << "SCALE " << i.x1 << "," << i.x2 << std::endl;
		case inst::BEGINDRAW:
			return os << "BEGINDRAW " << i.i1 << std::endl;
		case inst::ENDDRAW:
			return os << "ENDDRAW" << std::endl;
		case inst::COLOR:
			return os << "COLOR " << i.x1 << "," << i.x2 << "," << i.x3 << std::endl;
		case inst::WIDTH:
			//std::cout << "WIDTH" << i.i1 << std::endl;
			return os << "WIDTH " << i.i1 << std::endl;
		case inst::DRAW:
			return os << "DRAW " << i.x1 << "," << i.x2 << std::endl;
		case inst::VIEW_ORIGIN:
			return os << "VIEW_ORIGIN " << i.x1 << "," << i.x2 << std::endl;
		case inst::VIEW_ROTATE:
			return os << "VIEW_ROTATE " << i.x1 << std::endl;
		case inst::VIEW_SCALE:
			return os << "VIEW_SCALE " << i.x1 << "," << i.x2 << std::endl;
		}
		return os;
	}

	inst name;
	double x1, x2, x3;
	bool b1;
	int i1;
};
#endif