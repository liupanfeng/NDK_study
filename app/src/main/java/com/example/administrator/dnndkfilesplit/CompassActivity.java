package com.example.administrator.dnndkfilesplit;

import android.graphics.Bitmap;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import java.io.File;

public class CompassActivity extends AppCompatActivity {

    //获取sdcard路径
    private static final String SD_CARD_PATH= Environment.getExternalStorageDirectory().getAbsolutePath();

    private static final String pathPic=SD_CARD_PATH+"/"+"girl.jpeg"; //待压缩的图片地址

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_compass);
    }

    //质量压缩
    public void massCompress(View v){

    }

    //尺寸压缩
    public void sizeCompress(View v){

    }

    /**
     * 质量压缩
     * @param bmp
     * @param file
     */
    public static void compressImageToFile(Bitmap bmp, File file) {

    }

    /**
     * 尺寸压缩
     * @param bmp
     * @param file
     */
    public static void compressBitmapToFile(Bitmap bmp, File file){

    }
}
