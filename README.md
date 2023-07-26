# 摆烂, 不做了
预编译头和[zxing-cpp](https://github.com/zxing-cpp/zxing-cpp)一起用, Android构建不通过
> ```
> C:\Qt\6.5.2\android_arm64_v8a\include\QtCore\qchar.h:9: error: 'functional' file not found
> In file included from src\pchQML.h:4:
> In file included from C:\Qt\6.5.2\android_arm64_v8a\include\QtWidgets/QApplication:1:
> In file included from C:\Qt\6.5.2\android_arm64_v8a\include\QtWidgets/qapplication.h:8:
> In file included from C:\Qt\6.5.2\android_arm64_v8a\include/QtCore/qcoreapplication.h:8:
> In file included from C:\Qt\6.5.2\android_arm64_v8a\include/QtCore/qstring.h:14:
> C:\Qt\6.5.2\android_arm64_v8a\include/QtCore/qchar.h:9:10: fatal error: 'functional' file not found
> #include <functional> // for std::hash
>          ^~~~~~~~~~~~
> ```
# 智能题卡

## 如何使用
可以通过新教育题卡二维码或编号获取答案

## 开发

### 编译

#### 编译环境
>Windows: Qt 6.5.1 + MSVC2019

>Android: Qt 6.5.1 + NDK 25.1.8937393 + JDK 17

#### 如何编译

```bash
cd ZhiNengTiKa
git submodule update --init --recursive
```

使用 Qt Creator 打开 `ZhiNengTiKa.pro` 或 `ZhiNengTiKaQML.pro`
## ⚡ Visitor count

![](https://profile-counter.glitch.me/LFWQSP2641-ZhiNengTiKa/count.svg)