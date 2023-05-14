package com.LFWQSP2641.ZhiNengTiKa;

import org.qtproject.qt.android.bindings.QtActivity;
import android.os.Build;
import android.provider.Settings;

public class Util extends org.qtproject.qt.android.bindings.QtActivity
{
    public static String getCacheDir(QtActivity activity)
    {
        return activity.getCacheDir().getAbsolutePath();
    }
    public static String getAndroidId(QtActivity activity)
    {
        return Settings.Secure.getString(activity.getContentResolver(), Settings.Secure.ANDROID_ID);
    }
}
