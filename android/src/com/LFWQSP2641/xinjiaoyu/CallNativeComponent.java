package com.LFWQSP2641.ZhiNengTiKa;

import org.qtproject.qt.android.bindings.QtActivity;
import java.io.File;
import android.net.Uri;
import android.content.Intent;
import android.os.Build;
import androidx.core.content.FileProvider;
import android.provider.MediaStore;
import java.io.IOException;

public class CallNativeComponent extends org.qtproject.qt.android.bindings.QtActivity
{
    public static void callNativeCamera(QtActivity activity)
    {
        File outputImage = new File(Util.getCacheDir(activity), "callNativeCamera_result.jpg");
        Uri imageUri;
        if (outputImage.exists()) {
            outputImage.delete();
        }
        try {
            outputImage.createNewFile();
            //  如果运行设备的系统高于Android 7.0
            //  就调用FileProvider的getUriForFile()方法将File对象转换成一个封装过的Uri对象。
            //  该方法接收3个参数：Context对象， 任意唯一的字符串， 创建的File对象。
            //  这样做的原因：Android 7.0 开始，直接使用本地真实路径的Uri是被认为是不安全的，会抛出FileUriExposedException异常；
            //      而FileProvider是一种特殊的ContentProvider，他使用了和ContentProvider类似的机制对数据进行保护，可以选择性地将封装过的Uri共享给外部。
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                imageUri = FileProvider.getUriForFile(activity, "com.LFWQSP2641.ZhiNengTiKa.permissiontest.fileprovider", outputImage);
            } else {
                //  否则，就调用Uri的fromFile()方法将File对象转换成Uri对象
                imageUri = Uri.fromFile(outputImage);
            }
            //  启动相机
            Intent intent = new Intent("android.media.action.IMAGE_CAPTURE");
            //  指定图片的输出地址,这样拍下的照片会被输出到output_image.jpg中。
            intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
            // startActivityForResult(intent, 1);
            activity.startActivity(intent);
        } catch (IOException e) {
            e.printStackTrace();
        }
    };
//不会回调,摆了
//    public static void callNativeGallery(QtActivity activity)
//    {
//        Intent intent = new Intent(Intent.ACTION_PICK,MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
//        intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, "image/*");
//        activity.startActivity(intent);
//    }
};
