package org.swdc.platforms;

import java.io.File;
import java.util.Arrays;
import java.util.List;

public class WindowsBinaryUtils {

    /**
     * 注册Dll，等效于Regsvr32.
     * @param path Dll/Ocx的绝对路径
     * @return 是否注册成功
     */
    private static native boolean dllRegisterServer(String path);

    /**
     * 解除Dll的注册，等效于Regsvr32 /u
     * @param path Dll/Ocx路径
     * @return 是否卸载成功
     */
    private static native boolean dllUnRegisterServer(String path);

    /**
     * 读取系统的DNS地址
     * @return 系统使用的DNS服务器地址。
     */
    private static native String[] getSystemDNSAddress();

    /**
     * 注册Dll，等效于Regsvr32.
     * @param file Dll/Ocx文件
     * @return 是否注册成功
     */
    public static boolean dllRegister(File file) {
        if (file == null || !file.exists() || file.isDirectory()) {
            return false;
        }
        return dllRegisterServer(file.getAbsolutePath());
    }

    /**
     * 解除Dll的注册，等效于Regsvr32 /u
     * @param file Dll/Ocx文件
     * @return 是否卸载成功
     */
    public static boolean dllUnRegister(File file) {
        if (file == null || !file.exists() || file.isDirectory()) {
            return false;
        }
        return dllUnRegisterServer(file.getAbsolutePath());
    }

    /**
     * 读取本系统正在使用的DNS服务器的IP地址
     * @return DNS服务器地址（IPV4）
     */
    public static List<String> getSystemDNSServerAddress() {
        return Arrays.asList(getSystemDNSAddress());
    }

}
