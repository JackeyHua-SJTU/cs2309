# GUI编程 \#1
## 类的说明
### superellipse
有 $a,b,m,n,N, k, width, height$ 八个成员变量用来表示超椭圆的参数以及绘图的参数，$points$成员数组用来保存点的坐标。

在构造函数`superellipse(double a, double b, double m, double n, int N, int width, int height);`中，除了需要传入题目要求的 $a,b,m,n,N$ 之外，还 **必须** 指定 **绘图窗口的宽 $width$ 和高 $height$**，在这种情况下，默认会把每个点与其他所有的点都连接一条有向边。在构造函数`superellipse(double a, double b, double m, double n, int N, int k, int width, int height);`中可以指定每个点的连接数`k`，程序会在其余的点集中随机选取 $k$ 个点与其相连。在`main.cpp`中默认了绘图窗口的大小为 $800\times600$。

`draw()`中选择在 $x$ 坐标上间隔均匀，但在 $y$ 坐标上正负交替，即相邻点的 $x$ 坐标之差相等，$y$坐标一个大于0，在 $x$ 轴上方，另外一个小于0，在 $x$ 轴下方。这样可以使绘制出的图更加美观。在本函数中将每一个点 **与其他所有的点相连接**，构成一张完全图。


### poly
有一个成员数组 $points$ 用来保存点的坐标。

在构造函数`poly(std::vector<std::pair<double, double>> vc, int width, int height);`中，除了传入题目要求的点的坐标之外，还需要指定 **绘图窗口的宽 $width$ 和高 $height$**。在`main.cpp`中默认了绘图窗口的大小为 $800\times600$。

`isValidPoly(p1, p2, q1, q2)`用来判断线段 $p_1p_2$ 和线段 $q_1q_2$ 是否相交，包括规范相交和非规范相交。

## 命令行输入说明
### superellipse类的输入
输入一行参数，用空格隔开，必须输入的是 $a,b,m,n,N$，可选的输入是 $k, width, height$。如果不输入 $k$，则默认为 $k=N - 1$，即每个点与其他所有的点都连接一条有向边。如果不输入 $width$ 和 $height$，则默认为 $width=800, height=600$。

### poly类的输入
**必须在第一行输入指定窗口的大小，即输入width和height**。接下来输入点的坐标，每行输入一个点的坐标，用空格隔开，直至输入完毕。可以按 `Ctrl+D` 结束输入。

## 异常说明
superellipse类在遇到输入异常的时候会抛出对应的runtime_error异常，然后在`main.cpp`中进行捕捉并且输出。

poly类在遇到输入异常或者输入的点集无法构成多边形的情况时，会将私有变量`valid`置为`false`，并且在`main.cpp`通过`isValid()`接口来判断当前实例是否合法，如果不合法的话通过标准输出流报错。

## 运行说明
本地的电脑类型是MacOS Sonoma M1。通过`brew install fltk`，安装fltk至`/opt/homebrew/Cellar/fltk`路径下。通过`chmod +x ./run.sh && ./run.sh`运行该脚本，可以得到目标输出。运行前请务必切换至当前文件夹，即`/fltk_w12`。

如果是其他系统，可以把 `-I /opt/homebrew/Cellar/fltk/1.3.8_1/include`中的头文件地址换成自己的地址，然后再运行`run.sh`脚本。

因为在MacOS中`g++`是`clang++`的别名，因此在`run.sh`中使用了`clang++`编译器。如果因为编译器的原因导致结果的不同，麻烦告知我本人，我可以带着电脑线下来验收。

同时在`main.cpp`中用到了C++20中的标准，请保证本地C++编译器支持C++20的标准。

## 结果说明
在当前路径下的`result`文件夹下，是`main.cpp`中测试数据的运行结果，测试的代码在`main.cpp`的23行开始的和98行开始的注释中。在`main.cpp`中提供的是命令行输入的版本。