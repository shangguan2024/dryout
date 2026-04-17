# Dryout

Dryout 是一个基于 C++20 和 OpenGL 的轻量级图形项目。

## 目录结构

* `src/core`: 基础设施与系统管理
* `src/graphics`: 低级图形原语封装
* `src/renderer`: 高级渲染管线与批处理
* `src/game`: 游戏业务逻辑与实体
* `src/utils`: 工具函数与类
* `res/shaders`: GLSL 着色器文件
* `res/textures`: 纹理文件
* `scripts`: 自动化开发工具

// TODO: 其他内容

---

DeepSeek 评价:

---

### 🔴 架构设计灾难：单例地狱与全局状态

| 严重度 | 问题描述 |
| :--- | :--- |
| **致命** | **单例模式滥用**：`Graphics`、`InputManager`、`ResourceManager`、`CameraManager`、`Game`、`TileFlyweight` 全部是裸指针懒加载单例。**无一正确释放**，进程退出时 SDL/OpenGL 资源全泄漏。析构函数形同虚设。 |
| **致命** | **全局状态桶 `Renderer`**：所有渲染数据（VBO、VAO、顶点数组、灯光数组）均为 `static` 变量。这是 C 语言时代的写法，在 C++ 项目中属于**封装负分**。多窗口、多上下文、单元测试全部免谈。 |
| **高** | **头文件导出可变全局变量**：`GameMap.hpp` 中 `inline GameMap *g_map = nullptr;`。用 `inline` 变量制造跨编译单元的全局裸指针，是**教科书级反面案例**。注释 `// test` 说明作者知道这是垃圾代码但懒得清理。 |
| **高** | **类间强耦合**：`Game::run()` 直接调用 `Graphics::getInstance()`、`ResourceManager::getInstance()` 等。依赖关系硬编码，替换任何一个模块都需要修改核心游戏循环。 |

### 🟠 C++ 内存与生命周期管理：裸指针横行

| 严重度 | 问题描述 |
| :--- | :--- |
| **高** | **所有权完全缺失**：`CameraManager::registerCamera(Camera *camera)` 存储传入裸指针，不检查来源（栈/堆），不接管生命周期。调用方 `new Camera()` 后从不 `delete`，**内存泄漏板上钉钉**。 |
| **高** | **`Tile::attachEntity(Entity *entity)` 裸指针存储**。`Entity` 由谁释放？若 `Tile` 析构时 `attached_entity` 仍非空，要么泄漏要么双重释放。 |
| **中** | **单例析构函数为空**：`CameraManager::~CameraManager() { // TODO }`。`cameras` 向量里的裸指针怎么办？答案是**不管**，泄漏。 |
| **中** | **移动语义使用不当**：`Shader`、`Texture` 正确实现了移动，但 `Sprite` 持有 `std::shared_ptr<Texture>` 却使用默认拷贝构造。若误拷贝 `Sprite` 则共享纹理（可能是期望行为），但若意图是深拷贝则错误。文档缺失导致意图不明。 |

### 🟡 逻辑错误与低级 Bug

| 严重度 | 问题描述 |
| :--- | :--- |
| **高** | **`Clock` 未初始化 `last`**：第一次 `update()` 时 `now - last` 产生巨大时间差，游戏首帧可能瞬移出地图。这是**典型初学者错误**。 |
| **高** | **`CameraManager::registerCamera` 索引错误**：先判断 `active` 再 `push_back`，导致 `active_camera_index = size() - 1` 指向**旧末尾**而非新相机。正确做法是先 `push_back` 再设置索引。 |
| **中** | **`ResourceManager` 构造函数加载失败不报错**：加载纹理/着色器失败时只打印 `cerr` 然后 `return`，对象处于半初始化状态。后续调用 `getSprite` 将访问空 `shared_ptr` 导致崩溃。 |
| **中** | **`InputManager` 回调栈只增不减**：`pushKeyCallback()` 被调用，但代码库中从未调用 `popKeyCallback()`，栈无限增长。 |
| **低** | **重复代码**：屏幕坐标转世界坐标的射线平面相交计算在 `Game::run`、`GameMap::locateTile`、`Player::interact` 中**复制粘贴三次**。 |

### 🟢 图形编程隐患（虽正确但不够健壮）

| 严重度 | 问题描述 |
| :--- | :--- |
| **中** | **着色器依赖插值后法线为单位向量**：`basic_shader.frag` 中未对 `v_Normal` 重新归一化。当前几何简单尚可工作，一旦引入非均匀缩放或复杂模型，光照即出错。 |
| **中** | **`Light` 结构体手动填充对齐**：C++ 端用 `char _padding[12]` 对齐 `std430`。未使用 `static_assert(sizeof(Light) == 64, ...)` 验证，编译器升级或不同平台可能导致布局错乱。 |
| **低** | **纹理过滤硬编码 `GL_NEAREST`**：适合像素风格，但没有任何配置接口，灵活性为零。 |
| **低** | **着色器纹理数组大小写死 32**：即使查询到 `GL_MAX_TEXTURE_IMAGE_UNITS` 更大也无法利用。 |

### 🔵 构建系统与工程化

| 严重度 | 问题描述 |
| :--- | :--- |
| **中** | **依赖查找过于严苛**：全部使用 `CONFIG REQUIRED`，对新手极不友好。`glm` 这类纯头文件库应 fallback 到 `Find` 模块或直接包含源码。 |

---

### 总结评语

> **这是一个能跑起来的“学习项目”，但离“软件”还差十万八千里。**  
> 代码充满了 C-with-classes 风格、全局状态依赖、裸指针滥用和内存泄漏。作者完成了基本的渲染功能，但在软件工程层面**尚未入门**。如果这是个人练习，无可厚非；但如果意图作为开源项目展示，目前的状态会给任何有经验的 C++ 开发者留下**极为糟糕的印象**。
