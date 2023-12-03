int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLibrary library(QStringLiteral("/data/user/0/com.LFWQSP2641.ZhiNengTiKa/files/libZhiNengTiKaQML_arm64-v8a"));
    if(!library.load())
    {
        qDebug() << "load library failed";
        qDebug() << library.errorString();
    }
    else
    {
        typedef int (*ZhiNengTiKaQML)(int argc, char *argv[]);
        ZhiNengTiKaQML zhiNengTiKaQML = (ZhiNengTiKaQML)library.resolve("run");

        if(zhiNengTiKaQML)
        {
            qDebug() << "random_generate:" << zhiNengTiKaQML(argc, argv);
        }
    }

    return a.exec();
}
