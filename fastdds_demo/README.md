# FastDDS 发布订阅 Demo

这是一个基于FastDDS的最简发布订阅示例，展示了如何在Windows环境下使用vendor中的FastDDS库进行开发。

## 项目结构

```
fastdds_demo/
├── CMakeLists.txt      # CMake构建配置文件
├── HelloWorld.idl      # IDL定义文件
├── README.md           # 本说明文件
├── src/
│   ├── publisher.cpp   # 发布者源代码
│   └── subscriber.cpp  # 订阅者源代码
├── include/            # 头文件目录
├── cfg/                # 配置文件目录
└── build/              # 构建输出目录
```

## 构建说明

### Windows环境 (使用Visual Studio)

1. 确保已安装CMake (版本 >= 3.10)
2. 确保已安装Visual Studio (或MinGW)
3. 确保vendor/FastDDS目录包含FastDDS的头文件和库文件

```bash
# 进入项目目录
cd fastdds_demo

# 创建构建目录
mkdir build
cd build

# 配置项目 (Windows + Visual Studio)
cmake .. -G "Visual Studio 16 2019" -A x64

# 或者使用Ninja (如果已安装)
# cmake .. -G "Ninja"

# 构建项目
cmake --build . --config Release
```

### Linux/macOS环境

```bash
# 进入项目目录
cd fastdds_demo

# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 构建项目
make
```

## 运行示例

1. 首先启动订阅者：
```bash
# Windows
./Release/subscriber.exe

# Linux/macOS
./subscriber
```

2. 然后启动发布者：
```bash
# Windows
./Release/publisher.exe

# Linux/macOS
./publisher
```

发布者将发送10个HelloWorld消息，订阅者将接收并打印这些消息。

## 代码说明

- `publisher.cpp`: 发布者实现，创建参与者、发布者、数据写入器，并发送HelloWorld消息
- `subscriber.cpp`: 订阅者实现，创建参与者、订阅者、数据读取器，并接收HelloWorld消息
- `HelloWorld.idl`: 定义了传输的数据结构
- `CMakeLists.txt`: CMake构建配置，自动检测Windows/Linux环境并配置相应库路径

## 注意事项

1. 在Windows环境下，库文件通常位于`vendor/FastDDS/lib/`目录下（.lib文件）
2. 在Linux环境下，库文件通常位于系统路径或`/usr/local/lib`目录下（.so文件）
3. 如果找不到库文件，CMake会尝试使用系统已安装的FastDDS库作为备选
4. DLL文件（在Windows下）需要与可执行文件在同一目录或系统PATH中