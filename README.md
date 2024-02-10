English | [中文](README-CN.md)

## Introduction

A tool for mapping the contents of a dictionary-like container (that supports value retrieval through the '[]' operator) with strings as keys to an aggregate class.

A typical application pattern is similar to the @ConfigurationProperties annotation in the Spring framework for Java, which maps the contents of a configuration file to a struct that stores the corresponding data.

## Building and Installation

This project uses SCons as its build tool.

To install, simply run the following command in the root directory of the project:

```bash
scons install
```

For integrating this project to your own project, a SConscript file has been provided. You just need to recieve the library object returned by the SConscript and link it.

The tool depends on Boost.PFR. If you want to use yaml.h, also depends on yaml-cpp.

## Usage

#### 1. Modify the definition

When defining a struct, for example:

```cpp
struct Data{
  string user;
  int port{3306};
  int address;
  int password;
} data;
```

Need to be changed to:

```cpp
STRUCT_WITH_PROPERTIES(Data,
  string user;
  int port{3306};
  int address;
  int password;
) data;
```

The modified code defines a type called Data and an object called data. The internal properties and their initialization are the same as when the struct is defined directly.

#### 2. Presets

The library plan will predefine several header files to complete common mappings. (Although currently only supports yaml)
For example, if you want to map from a yaml file, you only need to include json.h first. Then, assuming that there is a node containing file content and a target struct, you only need to write:

```cpp
yaml2struct(node, config);
```

Supports strings, numbers, arrays, and objects. For arrays, only single-type array elements can be read. For objects, nesting is supported; simply nest the usage of STRUCT_WITH_PROPERTIES accordingly when defining the struct :

```cpp
STRUCT_WITH_PROPERTIES(Outer,
    STRUCT_WITH_PROPERTIES(Inner,
        // ...
    ) inner;
) outer;
```

#### 3. Customize

If you want to customize the mapping of other containers, you need to use the `toStruct` function, which accepts three parameters:
| Parameter | Function |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------------- |
| source | The container to be read |
| target | The target structure to write |
| fetch | A lambda expression that accepts two parameters. The first is the data object corresponding to the property in the container, and the second is a reference to the property int the struct. You should read data and write it to the property here. |

By customizing fetch, you can complete the mapping of custom containers. For more details, refer to the usage in [yaml.h](yaml.h).
