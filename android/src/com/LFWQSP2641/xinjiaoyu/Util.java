package com.LFWQSP2641.xinjiaoyu;

import org.qtproject.qt.android.bindings.QtActivity;

public class Util extends org.qtproject.qt.android.bindings.QtActivity
{
    public static String getCacheDir(QtActivity activity)
    {
        return activity.getCacheDir().getAbsolutePath();
    }
}
