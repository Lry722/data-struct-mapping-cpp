## 介绍

一个用于将以字符串为键的类似字典的（即支持通过`[]`运算符取值）的容器的内容映射到一个聚合类中的工具。

典型应用方式类似 Java 的 Spring 框架中的@ConfigurationProperties 注解，可以将配置文件的内容映射到一个保存对应数据的 struct 中。

## 构建和安装

本项目使用 SCons 进行构建。

要进行安装，只需要在项目目录下执行：

```bash
scons install
```

要作为你自己项目的一部分编译，也提供了 SConscript 文件，只需要接收其返回的 `libdsmap` 即可。

项目依赖于 Boost.PFR，如果要使用 yaml 映射，还依赖于 yaml-cpp。

## 使用方式

#### 1.修改定义

在定义一个 struct 时，例如：

```cpp
struct Data{
  string user;
  int port{3306};
  int address;
  int password;
} data;
```

需要改成：

```cpp
STRUCT_WITH_PROPERTIES(Data,
  string user;
  int port{3306};
  int address;
  int password;
) data;
```

修改后的代码会定义一个叫 Data 的类型，和一个 data 对象，对象内部的属性和初始化值与直接定义是一样的。

#### 2.预设方案

库中计划会预定义几个头文件，可以完成常见的映射。（~~虽然目前只支持 yaml~~）

例如，如果要从 yaml 文件映射，只需要先包含 yaml.h，然后假设有包含文件信息的 node 变量和目标结构体 config，只需要执行：

```cpp
yaml2struct(node, config);
```

支持字符串、数值、数组、对象。对于数组，只支持读取单一类型的数组元素。对于对象，支持任意嵌套，只需要在定义 struct 时相应的嵌套使用 `STRUCT_WITH_PROPERTIES`:

```cpp
STRUCT_WITH_PROPERTIES(Outer,
    STRUCT_WITH_PROPERTIES(Inner,
        // ...
    ) inner;
) outer;
```

#### 3. 自定义方案

如果要自定义其他容器的映射，则需要借助`toStruct`函数，该函数接受三个参数：

| 参数   | 作用                                                                                                                                     |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------------- |
| source | 要读取的容器                                                                                                                             |
| target | 映射的目标结构                                                                                                                           |
| fetch  | 一个 lambda 表达式，接受两个参数。第一个为容器中属性对应的数据对象，第二个为要写入的属性的引用。在其中完成数据对象的读取，并写入到属性中 |

通过自定义 fetch，即可完成自定义容器的映射，可以参考 yaml.h 中的使用方式。
