# GUI编程 \#1
## 类的说明
### superellipse
有$a,b,m,n,N,K$六个成员变量用来表示超椭圆的参数以及绘图的参数，$points$成员数组用来保存点的坐标。

在构造函数`superellipse(double a, double b, double m, double n, int N, int k, int width, int height);`中，除了需要传入题目要求的$a,b,m,n,N$之外，还**必须**人为指定每个点连接的点数$K$，以及**绘图窗口的宽 $width$ 和高 $height$**。在`main.cpp`中默认了绘图窗口的大小为 $800\times600$。

### poly
有一个成员数组$points$用来保存点的坐标。

在构造函数`poly(std::vector<std::pair<double, double>> vc, int width, int height);`中，除了传入题目要求的点的坐标之外，还需要指定**绘图窗口的宽 $width$ 和高 $height$**。在`main.cpp`中默认了绘图窗口的大小为 $800\times600$。

私有成员函数`isLine`用来判断传入的点是否成一条直线，`isValidPoly`用来判断传入的点是否构成一个合法的多边形。

## 运行说明
本地的电脑类型是MacOS Sonoma M1。通过`brew install fltk`，安装fltk至`/opt/homebrew/Cellar/fltk`路径下，可以通过`chmod +x ./run.sh && ./run.sh`运行该脚本，可以得到目标输出。运行前请务必切换至当前文件夹，即`/fltk_w12`。

如果是其他系统，可以把 `-I /opt/homebrew/Cellar/fltk/1.3.8_1/include`中的库文件地址换成自己的地址，然后再运行`run.sh`脚本。

因为在MacOS中`g++`是`clang++`的别名，因此在`run.sh`中使用了`clang++`编译器。如果因为编译器的原因导致结果的不同，麻烦告知我本人，我可以带着电脑线下来验收。

同时在`main.cpp`中用到了C++20中的标准，请保证本地C++编译器支持C++20的标准。