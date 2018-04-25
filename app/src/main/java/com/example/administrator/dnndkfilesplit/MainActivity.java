package com.example.administrator.dnndkfilesplit;

import android.content.Intent;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.Date;
import java.util.Random;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private static final String TAG="System.out.cpp";

    // Used to load the 'native-lib' library on application startup.
    //获取sdcard路径
    private static final String SD_CARD_PATH= Environment.getExternalStorageDirectory().getAbsolutePath();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
//        TextView tv = (TextView) findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());
//        acessMethod();
//        accessStaticMethod();
//        accessConstructor();
//        chinessChars("a-飞燕");
//        int[] arr={3,23,59,6,15};
//        giveArray(arr);
//        for (int i=0;i<arr.length;i++){
//            Log.d("arr:",arr[i]+"");
//        }
//        String[] strArray=initStringArray(6);
//        for (String str:strArray){
//            Log.d("MainActivity:---",str);
//        }

//        createGlobalRef();
//        Log.d("globalRef:",getGlobalRef());
//        try{
////            exception();
//            cached();
//        }catch (Exception e){//这个exception 来自JNI
//            Log.d(TAG,e.getMessage());
//        }


    }

    public native String stringFromJNI();

    /**
     * c访问java方法
     */
    public native void acessMethod();

    /**
     * c访问java静态方法
     */
    public native void accessStaticMethod();

    public native Date accessConstructor();

    public native String chinessChars(String str);

    /**
     * @param arrr
     * JNI访问数组
     */
    public native void giveArray(int[] arrr);

    public native String[] initStringArray(int size);
    /*局部引用*/
    public native void localRef();

    /*JNI 创建全局引用*/
    public native void createGlobalRef();
    //获取全局应用
    public native String getGlobalRef();
    /*释放全局引用*/
    public native void delGlobalRef();
    /*创建弱引用*/
    public native void createWeakRef();
    /*局部变量缓存*/
    public native void cached();
    /*全局变量缓存*/
    public native void cachedGlobal();

    public native String AcessCacheNewString();

    public native void exception();
    public int showToast(){
        Toast.makeText(getApplicationContext(),"showToast",Toast.LENGTH_SHORT).show();
        return 0;
    }

    public static String getRandomUUID(){
       return UUID.randomUUID().toString();
    }

    public void diff(View v){
        Log.v(TAG,"diff begin");
        String path=SD_CARD_PATH+ "/"+"vab.mp4";
        String patter_path=SD_CARD_PATH+"/"+"vab_%d.mp4";
        FileUtils.diff(path,patter_path,4);
    }

    public void patch(View v){
        Log.v(TAG,"patch begin");
        String path=SD_CARD_PATH+ "/"+"vab_merge.mp4";      //最终合并生成得文件
        String patter_path=SD_CARD_PATH+"/"+"vab_%d.mp4";   //待合并的文件
        FileUtils.patch(path,patter_path,4);
    }

    public void jniThread(View v){
        Log.d(TAG,"jni thread begin!!");
        newJniThread();
    }

    //创建JNI线程
    private native void newJniThread();
    /*设置Env*/
    private native void setJniEnv();

    /*带调用的java静态方法*/
    private static void fromJni(int i){
        Log.d(TAG,"fromJni:"+i);
    }

    /*待调用的java对象方法*/
    private void from_JNI_Again(int i){
        Log.d(TAG,"from_JNI_Again"+i);
    }


    public void compassPic(View view){
        Intent intent=new Intent();
    }

}
