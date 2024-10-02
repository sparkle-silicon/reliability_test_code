# 本仓库为开发板基础版固件代码,由固件开发人员共同维护

### 强烈建议大家提交前使用"make clean"清除工程中的文件

## 说明

使用UART0作为CMD和正常打印输出，使用UART1作为调试器交互入口；

关于子模块（子树）：

添加子模块：


```bash
 git submodule add https://github.com/*****.git <name> #获取github路径链接到name中
 git submodule update --init --recursive #（获取仓库内容）
 git submodule update --recursive --remote ||git submodule update --recursive || git pull --recurse-submodules #更新仓库
 git submodule update #更新仓库
 git submodule update --remote #更新为远程项目最新版本
 git submodule init	# 初始化子模块
 git submodule update	# 更新子模块
 git clone --recursive <project url>
 git rm --cached <name> #删除仓库
 rm -rf <name> #删除仓库
 rm -rf .git/modules/GWToolkit
```
