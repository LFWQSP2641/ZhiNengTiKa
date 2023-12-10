package com.LFWQSP2641.ZhiNengTiKa;

import org.qtproject.qt.android.bindings.QtActivity;
import java.lang.Runnable;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.lang.reflect.Field;
import androidx.core.app.NotificationManagerCompat;
import android.widget.Toast;

public class ToastUtils extends org.qtproject.qt.android.bindings.QtActivity {
    private static Object iNotificationManagerObj;

    /**
     * @param activity
     * @param message
     */
    public static void show(QtActivity activity, String message) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                show(activity, message, Toast.LENGTH_SHORT);
            }
        });
    }

    /**
     * @param activity
     * @param message
     */
    public static void show(QtActivity activity, String message, int duration) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (message == null || message.length() == 0) {
                    return;
                }
                //后setText 兼容小米默认会显示app名称的问题
                Toast toast = Toast.makeText(activity, null, duration);
                toast.setText(message);
                if (isNotificationEnabled(activity)) {
                    toast.show();
                } else {
                    showSystemToast(toast);
                }
            }
        });
    }

    /**
     * 显示系统Toast
     */
    private static void showSystemToast(Toast toast) {
        try {
            Method getServiceMethod = Toast.class.getDeclaredMethod("getService");
            getServiceMethod.setAccessible(true);
            //hook INotificationManager
            if (iNotificationManagerObj == null) {
                iNotificationManagerObj = getServiceMethod.invoke(null);

                Class iNotificationManagerCls = Class.forName("android.app.INotificationManager");
                Object iNotificationManagerProxy = Proxy.newProxyInstance(toast.getClass().getClassLoader(), new Class[]{iNotificationManagerCls}, new InvocationHandler() {
                    @Override
                    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
                        //强制使用系统Toast
                        if ("enqueueToast".equals(method.getName())
                                || "enqueueToastEx".equals(method.getName())) {  //华为p20 pro上为enqueueToastEx
                            args[0] = "android";
                        }
                        return method.invoke(iNotificationManagerObj, args);
                    }
                });
                Field sServiceFiled = Toast.class.getDeclaredField("sService");
                sServiceFiled.setAccessible(true);
                sServiceFiled.set(null, iNotificationManagerProxy);
            }
            toast.show();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 消息通知是否开启
     *
     * @return
     */
    private static boolean isNotificationEnabled(QtActivity activity) {
        NotificationManagerCompat notificationManagerCompat = NotificationManagerCompat.from(activity);
        boolean areNotificationsEnabled = notificationManagerCompat.areNotificationsEnabled();
        return areNotificationsEnabled;
    }
}
