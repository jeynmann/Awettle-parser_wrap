# paser_wrap

#### 介绍

简化两个优秀的json 解析库的 API, 降低使用成本, 提高开发效率.

- 戳: <http://rapidjson.org/zh-cn/md_doc_pointer_8zh-cn.html#JsonPointer>
- 戳: <https://github.com/simdjson/simdjson>

优势:

1. 不调用 operator [] / find() 接口(接口复杂度o(n)), 假定数据量为 m, 解析复杂度由 o(mn) 降低至 [o(min(m,n)), o(max(m,n))];
2. 解析数据不需要 if/for 控制块;
3. 数据和 Parser 可以包装到一起;
4. 通过重构 TypeWrapper, 实现自定义类型解析, 从而更快地组建 parser;
5. 提供了一种范式, 可以套用到其它格式解析;

劣势:

1. 仅保留了原库少部分功能

#### 软件架构

#### 安装教程

``` bash
mkdir build
cd build
cmake ..
make
make install
```

#### 使用说明

``` c++
struct Data{
    int i{};
    float f[2]{};
    const char* c{};
    std::vector<std::string> sv;
};

Data dat;
ObjectParser ps;
ps.inser_builtin("sm_key1", dat.i);
ps.inser_builtin("sm_key2", dat.f);
ps.inser_builtin("sm_key3", dat.c);
ps.inser_builtin("sm_key4", dat.sv);
ps(doc);

```

#### 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

