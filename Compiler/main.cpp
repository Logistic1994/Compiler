// ----------------------------- main.cpp ---------------------------------
#include <iostream>
#include <fstream>
#include <gl/glut.h>
#include "semantics.h"
#include "sandbox.h"

sandbox* pBox;

// ------------------------- 检查源程序文件是否合法函数
bool checkSrcFile(const char* lpszCmdParam)
{
	FILE * file = NULL;

	if (strlen(lpszCmdParam) == 0)
	{
		std::cout << "未指定源程序文件 !" << std::endl;
		return false;
	}
	if ((file = fopen(lpszCmdParam, "r")) == NULL)
	{
		std::cout << "打开源程序文件出错 !" << std::endl;
		return false;
	}
	else fclose(file);
	return true;
}


void draw() {
	/*if (!pBox->anim && !pBox->static_replayed) {
		pBox->play();
	}*/
	pBox->play();
	glutSwapBuffers();
	std::cout << pBox->finished << std::endl;
}

// 当窗口发生变化的时候，我这里要求保持缩放不变化
void changeSize(GLsizei w, GLsizei h) {
	pBox->changed_size(w, h);
}
void onTimer(int value) {
	if (pBox->finished) {
		return;
	}
	else {
		glutPostRedisplay();
		glutTimerFunc(16, onTimer, 1);
	}
}
int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "参数应该有一个" << std::endl;
		return 0;
	}
	const char* src = argv[1];
	if (!checkSrcFile(src)) {
		return 0;
	}
	std::ofstream of("insts.txt", std::ofstream::out);
	semantics_class semantics;
	semantics.parser(src);
	semantics.show_instructions(of);
	sandbox box = sandbox(semantics.get_instructions());
	box.preprocess();
	pBox = &box;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Compiler");
	glutDisplayFunc(draw);
	glutReshapeFunc(changeSize);
	if (pBox->anim) {
		glutTimerFunc(16, onTimer, 1);
	}
	glutMainLoop();
	return 0;
}