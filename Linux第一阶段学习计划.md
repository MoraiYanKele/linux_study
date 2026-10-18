# Linux 第一阶段学习计划：开发环境、编译与调试

适用背景：已经会 C，做过 STM32／RTOS，希望继续学习 BSP 和 Linux 驱动。
周期：7 个学习日，每天约 90～120 分钟。环境安装或排错可另外预留 1～2 天。
完成标志：能独立创建多文件 C 工程，通过 Make 编译，使用 GDB 定位错误，并用 Git 保存版本。

## 1. 如何使用这份计划

每天按“理解概念 20 分钟 → 动手 60～80 分钟 → 记录 10～20 分钟”进行。
命令逐条执行，观察输出；不要一次粘贴整页。代码可先照着输入，再关闭示例重做关键步骤。
已有经验的内容可以快进，但仍需完成当天验收。遇到问题记录完整命令、输出、当前目录和自己的判断。
本阶段在电脑上的 Linux 环境练习，暂时不需要购买开发板，不涉及内核驱动。

每天记录：

- 今天完成了什么：
- 哪个输出和预期不同：
- 如何确认原因：
- 最后如何解决：
- 明天需要继续验证什么：

## 2. 环境准备

如果使用 Windows，默认选择 Ubuntu 虚拟机；已有 Ubuntu 或 WSL 环境也可以直接完成本阶段。
虚拟机使用当前受支持的 Ubuntu LTS 桌面版，按官网安装向导操作。可从 2～4 个虚拟 CPU、4～8 GB 内存、40 GB 动态磁盘起步，具体按主机资源调整。
保留 Windows 原有开发环境即可。本阶段统一在 Linux 终端执行命令。
代码放在 Linux 用户目录，例如 ~/linux-study/stage1；WSL 用户也先放在 Linux 文件系统中。

终端提示符可能包含用户名、主机名、路径和 $，示例里只给需要输入的命令。
下面所有删除练习都限定在自己的练习目录中。先用 pwd 确认位置，明确目标后再删除；不用 sudo 运行编译器或练习程序。

## 3. 一周安排

| 学习日 | 主题 | 产出与验收 |
|---|---|---|
| 第 1 天 | 环境、包管理、首次编译 | hello.c 能编译运行，理解 sudo／apt 和 GCC 参数 |
| 第 2 天 | 文件、路径、权限、搜索 | 能在终端组织工程，找到文件和代码 |
| 第 3 天 | 输出、管道、进程、帮助 | 会保存输出、检查退出码、查看和停止进程 |
| 第 4 天 | 编译、目标文件、链接 | 三文件工程能分步构建，能解释编译与链接错误 |
| 第 5 天 | Make 与依赖 | 修改文件后只重新构建必要部分 |
| 第 6 天 | GDB | 用断点、变量和调用栈找到逻辑错误与崩溃 |
| 第 7 天 | Git 与独立验收 | 完成新工程，能构建、调试并保存版本 |

## 第 1 天：建立开发环境

### 学习目标

认识终端、Shell 和软件包；能够在 Linux 编译运行 C 程序。

终端是输入输出界面；Shell（例如 Bash）解释你输入的命令；gcc、git 等是被 Shell 启动的程序。
你输入 gcc main.c 时，Shell 启动编译器；输入 ./app 时，Shell 启动你的程序。

### 安装工具

在 Ubuntu 终端逐条执行：

```bash
sudo apt update
sudo apt install build-essential gdb git
```

| 部分 | 含义 |
|---|---|
| sudo | 根据权限配置，以管理员身份执行紧随其后的命令 |
| apt | Ubuntu／Debian 系列的软件包管理工具 |
| update | 更新软件包索引，不等于把所有软件升级 |
| install | 安装指定软件包及必要依赖 |
| build-essential | 基础构建工具集合，包含 GCC、G++、Make 等依赖 |
| gdb | 调试器 |
| git | 版本管理工具 |

输入 sudo 密码时通常不显示字符，这是正常现象。安装过程中阅读软件包列表和提示。
检查安装：

```bash
gcc --version
make --version
gdb --version
git --version
```

### 第一个程序

```bash
mkdir -p ~/linux-study/stage1
cd ~/linux-study/stage1
pwd
```

~ 表示当前用户的主目录；mkdir -p 创建目录并允许父目录不存在；cd 切换当前目录。
用熟悉的编辑器创建 hello.c。VS Code、图形文本编辑器都可以，暂时不用为了学习 Linux 更换编辑器。

```c
#include <stdio.h>

int main(void)
{
    printf("Hello Linux!\n");
    return 0;
}
```

编译运行：

```bash
gcc -Wall -Wextra -g -O0 hello.c -o hello
./hello
```

| 参数 | 用途 |
|---|---|
| -Wall -Wextra | 开启常用警告，并非所有警告 |
| -g | 生成调试信息 |
| -O0 | 关闭优化，方便入门调试 |
| -o hello | 输出文件命名为 hello |
| ./hello | 执行当前目录下的 hello |

### 练习与验收

- 修改输出，加入自己的名字；重新编译后运行。
- 故意删除一个分号，观察编译器指出的文件和行号，然后修复。
- 解释为什么修改源文件后，直接运行旧的 hello 不会出现新内容。
- 不看示例完成创建文件、编译、运行。

记录问题：sudo 为什么用于安装，编译时为什么通常不需要 sudo？

## 第 2 天：文件、路径、权限和搜索

### 必学命令

| 命令 | 用途 |
|---|---|
| pwd | 当前工作目录 |
| ls -l / ls -a | 详细信息／包含隐藏条目 |
| cd / cd .. / cd ~ | 切换目录／上一级／主目录 |
| mkdir | 创建目录 |
| cp / mv | 复制／移动或重命名 |
| rm | 删除文件 |
| cat / less | 输出小文件／分页阅读 |
| grep -n | 搜索文本并显示行号 |
| find | 按条件找文件 |
| chmod | 修改权限 |

绝对路径从 / 开始；相对路径以当前目录为起点；. 是当前目录，.. 是上一级目录。
Linux 区分文件名大小写，Main.c 和 main.c 是不同文件。

### 操作练习

```bash
cd ~/linux-study/stage1
mkdir -p practice/backup
cp hello.c practice/backup/hello_copy.c
mv practice/backup/hello_copy.c practice/backup/hello_saved.c
ls -l practice/backup
cat hello.c
grep -n "printf" hello.c
find . -name "*.c"
```

find 的 "*.c" 要加引号，让 find 处理匹配条件，而不是先由 Shell 展开。

阅读 ls -l 输出，辨认文件类型、所有者、大小和权限。权限 r/w/x 分别是读取、写入、执行；文件与目录的权限语义有所不同，先掌握普通文件。

创建一个小脚本 show.sh：

```bash
#!/bin/sh
printf 'Linux practice\n'
```

操作：

```bash
chmod u+x show.sh
./show.sh
ls -l show.sh
```

chmod u+x 为文件所有者增加执行权限。

### 验收

- 用绝对路径和相对路径各访问一次 hello.c。
- 找出练习目录下所有 .c 文件。
- 用 grep 搜索 printf 所在行。
- 删除 practice/backup/hello_saved.c，先确认自己删除的是副本。
- 能解释 ./show.sh 的 ./ 和执行权限。

## 第 3 天：输出、管道、进程和帮助

### 标准输入输出

程序通常有标准输入、标准输出和标准错误三个通道。

```bash
cd ~/linux-study/stage1
./hello > output.txt
./hello >> output.txt
cat output.txt
grep -n "printf" hello.c | cat
gcc missing.c -o missing 2> errors.txt
cat errors.txt
```

最后的 gcc 命令故意编译不存在的文件，用来练习保存错误信息。
> 覆盖标准输出，>> 追加标准输出，2> 保存标准错误，| 把左侧标准输出连接到右侧标准输入。

### 退出码

```bash
./hello
echo $?
gcc missing.c -o missing
echo $?
```

$? 是刚刚执行完的命令的退出状态。一般 0 表示成功，非 0 表示失败；具体意义由程序约定。
必须紧接着检查，执行别的命令后它就会变化。

### 进程练习

```bash
sleep 300 &
jobs
ps -ef
```

& 让命令在后台运行。观察输出中的进程 ID（PID）。
用 kill 后面接你刚才实际看到的 sleep PID，发送终止请求；不要照抄一个随意的数字。

再在前台执行 sleep 300，按 Ctrl+C 中断。理解 Ctrl+C 通常向前台进程组发送中断信号。
本阶段不用学习全部信号和作业控制。

### 学会查帮助

```bash
ls --help
man gcc
man 3 printf
```

man 中通常按 / 搜索，n 找下一个，q 退出。若系统缺少对应手册，可参考官方在线手册；没有手册不意味着工具没安装成功。

### 验收

- 将程序输出保存到文件。
- 能区分标准输出和标准错误。
- 查看命令退出码。
- 启动并终止自己创建的后台进程。
- 通过帮助文档查到一个未知选项。

## 第 4 天：理解编译和链接

### 工程代码

创建独立目录：

```bash
mkdir -p ~/linux-study/stage1/calc
cd ~/linux-study/stage1/calc
```

calc.h：

```c
#ifndef CALC_H
#define CALC_H

int add(int a, int b);

#endif
```

calc.c：

```c
#include "calc.h"

int add(int a, int b)
{
    return a + b;
}
```

main.c：

```c
#include <stdio.h>
#include "calc.h"

int main(void)
{
    int left = 3;
    int right = 5;
    int result = add(left, right);
    printf("result = %d\n", result);
    return 0;
}
```

### 分步构建

```bash
gcc -Wall -Wextra -g -O0 -c main.c -o main.o
gcc -Wall -Wextra -g -O0 -c calc.c -o calc.o
gcc main.o calc.o -o app
./app
```

预期输出：result = 8。
-c 生成目标文件并停止，不进行链接；.o 不能当作完整应用直接运行。
头文件提供声明；源文件提供实现；链接器把目标文件中的引用与定义联系起来。

编译过程可以先理解为：预处理 → 编译 → 汇编 → 链接。
观察中间产物：

```bash
gcc -E main.c -o main.i
gcc -S -O0 main.c -o main.s
less main.s
```

本阶段认识 main.i 是预处理结果、main.s 是汇编文本即可，不要求读懂全部内容。

### 两类错误练习

1. 在 calc.c 删除一个分号，重新编译，观察编译错误，然后修复。
2. 故意只执行 gcc main.o -o app，观察 undefined reference 链接错误。
3. 补上 calc.o，重新链接。

### 验收

- 独立构建三文件工程。
- 能解释声明存在，为什么链接时仍可能找不到函数实现。
- 能区分语法错误、缺失头文件和缺失函数定义。
- 解释这些工具与 Keil 工程构建的对应关系。

## 第 5 天：用 Make 管理依赖

### 完整 Makefile

在 calc 目录创建 Makefile。每条构建命令开头必须使用 Tab 字符。

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g -O0

app: main.o calc.o
	$(CC) main.o calc.o -o app

main.o: main.c calc.h
	$(CC) $(CFLAGS) -c main.c -o main.o

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c calc.c -o calc.o

.PHONY: clean
clean:
	rm -f app main.o calc.o
```

Makefile 规则的形式是“目标: 依赖”，下一行是生成目标的命令。
Make 主要通过依赖关系和文件时间戳决定是否重新构建。.PHONY 表示 clean 不是一个真实文件目标。

### 观察增量构建

```bash
make clean
make
./app
make
```

第二次 make 应无需重新编译。修改 calc.c，保存后再次 make：
应只重编译 calc.o，再链接 app。

修改 calc.h 并保存，再 make：
main.o 和 calc.o 都依赖这个头文件，因此都会重编译。

make -n 可预览当前需要执行的命令：

```bash
make -n
```

### 扩展练习

在 calc.h 声明 sub，在 calc.c 实现 sub，在 main.c 调用。
通过 make 构建，验证结果，不再手动逐个运行 GCC。

### 验收

- 不复制示例，写出能构建三文件工程的 Makefile。
- 能解释目标、依赖、命令和变量。
- 预测修改 .c 或 .h 后会重编译什么，并用输出验证。
- 能处理 missing separator：检查命令缩进是否为 Tab。

## 第 6 天：GDB 调试与故障定位

### 正常程序：断点、单步和调用栈

先执行 make，确保编译选项包含 -g -O0：

```bash
gdb ./app
```

以下命令输入在 GDB 内，不是 Shell：

```text
break main
run
next
```

重复 next，直到执行 add 的那一行之前。此时 left 和 right 已初始化。
再执行：

```text
print left
print right
step
info args
backtrace
next
finish
next
print result
continue
quit
```

观察源码位置再决定何时查看变量；在变量初始化前查看，数值不能用来判断程序逻辑。
不同版本显示格式可能不同，理解行为即可。

| 命令 | 作用 |
|---|---|
| break main | 在 main 设置断点 |
| run | 启动或重新运行 |
| next | 源码单步，通常不进入函数 |
| step | 源码单步，可进入函数 |
| print / p | 求值并显示变量或表达式 |
| info args / info locals | 参数／局部变量 |
| backtrace / bt | 调用栈 |
| finish | 运行到当前函数返回 |
| continue / c | 继续执行 |
| quit / q | 退出 |

### 逻辑错误练习

暂时把 add 改为 return a - b;，重新构建。
在 add 设置断点，查看输入，逐步执行，确认错误发生在计算而不是调用方。定位后恢复。

### 崩溃练习

另建 crash.c，故意包含空指针访问：

```c
#include <stdio.h>

static void update(int *value)
{
    *value = 10;
}

int main(void)
{
    int *p = NULL;
    update(p);
    return 0;
}
```

编译调试：

```bash
gcc -Wall -Wextra -g -O0 crash.c -o crash
gdb ./crash
```

GDB 内执行：

```text
run
backtrace
print value
frame 1
print p
quit
```

程序通常因 SIGSEGV 停止。用停住的行、空指针值和调用栈说明原因。
把 main 改为：

```c
int main(void)
{
    int value = 0;
    update(&value);
    printf("value = %d\n", value);
    return 0;
}
```

重新编译并运行，预期输出 value = 10。
解释 value 对象在 update 执行时仍有效，&value 提供了可写对象的地址。

### 验收

- 找到逻辑错误，不靠连续添加 printf 猜测。
- 根据 GDB 输出说明崩溃位置、参数和调用来源。
- 修复后重新构建，确认正常退出。
- 写一份“现象 → 证据 → 原因 → 修改 → 验证”的短记录。

## 第 7 天：Git 和独立综合验收

### 版本管理

在 calc 工程中：

```bash
git init
git status
```

创建 .gitignore：

```gitignore
*.o
*.i
*.s
app
crash
output.txt
errors.txt
```

若尚未设置提交身份，用自己的姓名和邮箱配置；下面是占位示例，需要替换。这里使用仓库级配置，避免影响其他仓库。

```bash
git config user.name "你的姓名"
git config user.email "你的邮箱"
git add main.c calc.c calc.h Makefile .gitignore
git diff --cached
git commit -m "Create Linux C practice project"
git log --oneline
```

修改 main.c 输出文字，观察：

```bash
git diff
git status
```

再添加并提交。理解工作区、暂存区、提交三者的关系。无需先上传远程仓库。

### 独立验收项目

新建 sensor-demo 目录，不看 calc 示例完成：

- sensor.h 声明 int sensor_read(void)。
- sensor.c 返回一个模拟数据，例如 25。
- main.c 调用并打印数据。
- Makefile 能构建 app 并执行 clean。
- 用 GDB 停在 sensor_read，查看调用栈和返回结果。
- 给项目添加 Git，提交源代码和 Makefile，忽略构建产物。

这是主机端的模拟练习，不是硬件驱动。练习重点是开发流程。

### 最终验收清单

- [ ] 能安装工具，解释 apt update 和 apt install。
- [ ] 能通过终端创建、复制、移动、搜索文件。
- [ ] 能解释绝对路径、相对路径和 ./。
- [ ] 能保存输出并检查退出码。
- [ ] 能区分编译错误和链接错误。
- [ ] 能手动构建多文件程序。
- [ ] 能写基本 Makefile 并验证增量构建。
- [ ] 能设置断点、单步、查看变量和调用栈。
- [ ] 能定位并修复一次程序崩溃。
- [ ] 能查看 Git 改动并完成提交。

关键项未完成就补一天练习；不用为了日历进度进入下一阶段。

## 4. 常见问题速查

| 现象 | 优先检查 |
|---|---|
| command not found | 命令拼写、工具是否安装、是否在正确环境 |
| No such file or directory | pwd、ls、文件名大小写和路径 |
| ./app: Permission denied | 是否有执行权限、所在文件系统是否限制执行 |
| missing separator | Makefile 命令缩进是否为 Tab |
| undefined reference | 实现是否存在、对应 .o 是否参与链接 |
| GDB 没有源码或变量信息 | 是否使用 -g、调试的是不是新构建的文件 |
| 修改后结果没变化 | 是否保存、重新构建，运行的路径是否正确 |
| apt 安装失败 | 先看具体报错，区分网络、软件源与包名问题 |

遇到问题时提供：运行环境、pwd 输出、完整命令、完整报错和相关文件。
不要把“重新安装整个系统”作为第一种排错方式。

## 5. 本阶段暂缓内容与后续衔接

先把上面的流程做熟。Shell 脚本高级语法、CMake、交叉编译、内核源码、设备树、Yocto 和复杂网络配置放到后面按需学习。
Vim 可继续用已有习惯，编辑器快捷键不作为本阶段验收项。
第一阶段完成后进入：进程／线程、用户态与内核态、系统调用、文件描述符和同步机制。这些会为驱动学习建立基础。

## 6. 参考资料

按具体问题查阅，不要求通读：

- Ubuntu 软件包管理：https://documentation.ubuntu.com/server/how-to/software/package-management/
- GCC 编译选项：https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html
- GNU Make 手册：https://www.gnu.org/software/make/manual/
- GDB 手册：https://sourceware.org/gdb/current/onlinedocs/gdb.html/
- Git 官方入门：https://git-scm.com/book/zh/v2
- Bootlin 嵌入式 Linux 免费教材：https://bootlin.com/docs/

来源用于核对工具机制；每日时间安排与练习工程是结合你的 C／RTOS 基础设计的学习建议。

