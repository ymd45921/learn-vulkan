<p align="center"><img src="../public/favicon/todo.svg" width=96></img></p>
<h1 align="center">TODO</h1>
<p align="center">接下来的计划</p>

## 问题

- [ ] 直接从 `std::string_view` 获得 C 字符串指针实在过于危险
- [ ] 处理了 RAII 类的移动构造函数，那么赋值函数呢？
- [ ] 目前使用全局定义的 nullptr 作为默认分配器，有什么使用自定义分配器的解决方案？
- [ ] `pipline_create_helper` 有点定位不明了，，，，

## 计划

- [ ] 清理 `inlines.h` 中的内容 —— 它不应该出现在最终版本中？