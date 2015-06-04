# Compiler

#### 简要说明
本作品将绘图指令转变为图像。

绘图指令有：
```
animation on; // 是否以动画的形式播放
speed 1; // 播放速度
background is (255, 0, 0); // 以红色为背景色
// background is red;
origin is (x, y); // 以(x,y)绘图的原点
rot is x;  // 以x（弧度）将坐标系作逆时针旋转
scale is (x, y); // 以(x,y)进行坐标缩放
view_origin is (x, y); // 摄像机视角在接下来一条for指令中移动到(x,y)去
view_scale is (x, y); // 摄像机视角在接下来一条for指令中放缩为(x,y)
width is 2; // 线条宽度为2
color is [red/(255, 0, 0)]; // 线条颜色为红色
for T from 0 to 100 draw (t, sin(t)); // 绘图指令
```

其中`animation`指令和`speed`指令如果存在，必须放在头部。`background`指令必须放在绘图指令`draw`之前。

使用方式为:
``` bash
Compiler draw1.txt
```
#### 编译需求
需要opengl的支持，主要是glut库。

#### 算法实现
本例中使用递归下降的语法分析器。
首先将绘图指令转换为内部指令`instruction`，然后使用内部指令进行绘图。可以在`semantic_class`中进行内部指令输出。

#### 其他说明
本作品为西安电子科技大学编译原理课设作业。