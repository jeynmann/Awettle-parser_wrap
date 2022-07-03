# paser_wrap

#### Description

Simplifized the API of two excellent JSON parse libraries.

-click:< <http://rapidjson.org/zh-cn/md_doc_pointer_8zh-cn.html#JsonPointer> >

-click:< <https://github.com/simdjson/simdjson> >

Advantages:

1. no calling to operator [] or find() (whose complexity is O(n)). Assuming that the amount of data is m, the total complexity is reduced from O(MN) to [O(min(m, n)), O(max(m, n))];

2. no if/for control block when parses data;

3. data and parser can be packaged together easily;

4. user-defined type can be parsed easily by implement the class template of $ValueParser$;

5. gives a common way to wrap all kinds of parsers;

Disadvantages:

1. only wraps a few functions of the parse libraries

#### Software Architecture

Software architecture description

#### Installation

``` bash
mkdir build
cd build
cmake ..
make
make install
```

#### Instructions

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

#### Contribution

1. Fork the repository
2. Create Feat_xxx branch
3. Commit your code
4. Create Pull Request

