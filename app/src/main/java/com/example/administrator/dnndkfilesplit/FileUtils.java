package com.example.administrator.dnndkfilesplit;


public class FileUtils {

    public static native void diff(String path, String pattern_Path, int num);
    public static native void patch(String path, String pattern_Path, int num);
    static {
        System.loadLibrary("native-lib");
    }

}
