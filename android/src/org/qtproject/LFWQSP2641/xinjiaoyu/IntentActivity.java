package org.qtproject.LFWQSP2641.xinjiaoyu;

import org.qtproject.qt.android.bindings.QtActivity;
import java.io.File;
import android.net.Uri;
import android.content.Intent;

public class IntentActivity extends org.qtproject.qt.android.bindings.QtActivity
{
//    public static void installApk(String filePath,QtActivity activity)
//    {
//        Intent intent = new Intent();
//        // 执行动作
//        intent.setAction(Intent.ACTION_VIEW);
//        File file = new File(filePath);
//        // 执行的数据类型
//        intent.setDataAndType(Uri.fromFile(file),"application/vnd.android.package-archive");
//        activity.startActivity(intent);
//    };
    public static void openUrl(String url,QtActivity activity)
    {
        Intent intent = new Intent();
        // 执行动作
        intent.setAction(Intent.ACTION_VIEW);
        // 执行的数据类型
        intent.setDataAndType(Uri.parse(url),"text/html");
        activity.startActivity(intent);
    };
};
