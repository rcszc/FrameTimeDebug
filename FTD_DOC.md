# FrameTimeDebug
> Dev: RCSZ, LoC: 1403, T: 2024_08_18.

Update(Version): 20240818.0503

## 捕获端 (Capture)

捕获的数据打包写入文件为独立的子线程, 但是当每帧的捕获点增多时依然会额外消耗许多性能. 采样点主要捕获信息为: 当前进程操作内存(+-), 当前采样点域上下文耗时(最高纳秒精度) 等. 打包帧捕获信息为: 当前进程使用内存, 当前帧时间(相对于开始运行时) 等.

__捕获类使用:__
```cpp
#include "psag_ft_debug.hpp"
```
```cpp
#if ENABLE_DEBUG_MODE
		FTDcapture::CaptureContext CapPoint; // 创建捕获点对象

		CapPoint.CaptureSettingFilter(FTD_TagLv3); // 设置等级(1-5)
		CapPoint.CaptureSettingPointer(this);      // 指针(用于后期识别可不设置)
		CapPoint.CaptureSettingTagging("...");     // 标注(注释可不设置)
		CapPoint.CaptureBegin();                   // 开始捕获
#endif

// 作用域结束后, 析构时处理捕获的信息, 开始向处理线程提交数据.
```
内部定义模式 (在Debug模式下并且开启"ENABLE_FTD"):
```cpp
#if (defined(_DEBUG) || defined(DEBUG)) && ENABLE_FTD
#define ENABLE_DEBUG_MODE 1
#else
#define ENABLE_DEBUG_MODE 0
#endif
```

__捕获全局更新:__

- .ftbf (二进制) 帧数据文件
- .ftbp (二进制) 采样点数据文件
- .ftbt (文本) 文本注释文件

```cpp
// 创建处理线程(全局), 输入文件夹路径和文件名, 会自动在目录下创建3个数据文件.
FTDthread::FTDprocessingThread.CreateProcessingThread("../", "TEST");

// 帧更新(全局), 在你的 游戏/程序 中主线程渲染结束时调用, 以收集当前捕获的所有点信息.
{
	FTDthread::FTDprocessingThread.MainUpdateFrame();
}

// 删除处理线程(全局), Debug结束.
FTDthread::FTDprocessingThread.DeleteProcessingThread();
```

## 视图编辑器端 (Editor)

编辑器使用ImGui核心UI编写, 进行收集完成后的数据显示, 写的比较简陋, 数据量非常大时会加载缓慢已经卡顿.

__加载数据&绘制编辑器:__
```cpp
#include "psag_ft_debug_view.h"
```
```cpp
FTDViewLoader::DataFilesLoader* TestLoader = nullptr; // 文件加载器.
FTDViewEditor::DebugViewEditor* TestEditor = nullptr; // 编辑器.

// 你的ImGui(ForOpenGL?)渲染程序 初始化时
{
    // 构造时输入文件夹路径以及文件名.
    TestLoader = new FTDViewLoader::DataFilesLoader("../", "TEST");

    // 如果加载器无错误信息, 把加载器对象传入编辑器的构造中.
    // 注意不能销毁加载器, 数据始终储存在加载器.
    if (TestLoader->LoaderErrorInfo.empty())
	    TestEditor = new FTDViewEditor::DebugViewEditor(TestLoader);
    else
	    cout << TestLoader->LoaderErrorInfo << endl;
}

// 渲染循环
{
    // 输入编辑器(ImGui)窗口名称.
    TestEditor->RenderingViewEditor("My FTD Editor");

    TestEditor->EditorLerpSpeed; // 编辑器动画部件插值速度, float
    TestEditor->EditorWinFixed;  // 编辑器窗口固定标志, bool
}

// 结束销毁
{
    // 主要销毁顺序.
    delete TestEditor; TestEditor = nullptr;
    delete TestLoader; TestLoader = nullptr;
}
```

> 其他的就不过多讲解, 非常简单可以读源码, 我技术菜见谅~
---
```END```