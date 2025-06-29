# 1,设置工程名称
project( "singel_thread_task_schedule" )

# 2,设置 CMake 最低版本号，我电脑装的是3.5
cmake_minimum_required( VERSION 3.2 )

# 3,设定编译参数
set(CMAKE_CXX_STANDARD    23)  # 指定 C++ 版本
set(CMAKE_BUILD_TYPE "Debug")  # 调试使用Debug，可以查看中间变量；发布使用Release，运行速度快

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 4,设定源码列表,查找指定目录下的所有源文件,并将名称保存到 DIR_SRCS 变量中
aux_source_directory(./src/ DIR_SRC)
aux_source_directory(./src/infra DIR_SRC)
aux_source_directory(./src/thread_one DIR_SRC)

# 5,设定头文件路径
include_directories(./include/
                    ./include/infra
                    ./include/thread_one)

# 6，把源码编译成一个可执行文件，文件名为main（可以随便取名），会保存在当前目录下
add_executable( main ${DIR_SRC} )
