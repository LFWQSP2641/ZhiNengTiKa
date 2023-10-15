# 智能题卡

## 如何使用
可以通过新教育题卡二维码或编号获取答案

## 开发

### 编译

#### 编译环境
>Windows: Qt 6.5.1 + MSVC2019

>Android: Qt 6.5.1 + NDK 25.1.8937393 + JDK 17

如果使用Qt 6.5.1, 6.5.2, 6.6.0进行Android平台的编译, 需要将[Qt6AndroidMultimedia.jar](./Qt6AndroidMultimedia.jar)(根据提交 [Android-ffmpeg: handle maxImages acquired problem](https://codereview.qt-project.org/gitweb?p=qt/qtmultimedia.git;a=commit;h=c5e5d619107568050d9857d5149bd1b7558b904b) 进行的修改编译)替换Qt自带的文件


#### 如何编译

```bash
cd ZhiNengTiKa
git submodule update --init --recursive
```

使用 Qt Creator 打开 `ZhiNengTiKa.pro` 或 `ZhiNengTiKaQML.pro`
## ⚡ Visitor count

![](https://profile-counter.glitch.me/LFWQSP2641-ZhiNengTiKa/count.svg)