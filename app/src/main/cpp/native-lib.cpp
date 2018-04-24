#include <jni.h>
#include <string>

//#include<cstdlib>
#include<stdlib.h>
#include <android/log.h>
#include <string.h>
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#include <assert.h>

#define LOG_TAG "System.out.cpp"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)



extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//访问java中的非静态方法
extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_acessMethod(JNIEnv *env,
                                                                             jobject instance) {
    //首先获取jclass
    jclass jclz=env->GetObjectClass(instance);
    //获取待访问的方法id
    jmethodID mid=env->GetMethodID(jclz,"getRandomInt","()I"); //这个方法是无参方法，如果需要带参数 ，方法签名和下面的调用有些区别
    //访问java中的方法
    jint random=env->CallIntMethod(instance,mid);
    //打印获取到的数据
    LOGD("C++ log: %d \n", random);
}

//jni 访问 java中的静态方法
extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_accessStaticMethod(JNIEnv *env,
                                                                             jobject instance) {

    //jvm 内存有限，如果有人调用静态方法，虚拟机首先判断，内存里边是否已经加载过这个类，如果没有加载，虚拟机会首先加载，，加载成功，就会顺利调用
    //如果，没有加载成功，则会报一个错误：java.lang.classNotFoundException 异常，这个异常在反射的时候会经常看到。
    //在jni中访问静态方法，
    //获取到jclass  通过jobject来搜索 class 如果找到了，就讲找到的class 转变成jclass 这一步的加载，jvm其实是做了很多工作的。
    LOGD("acessStaticMethod ");
    jclass jclz=env->GetObjectClass(instance);
    LOGD("jclz ");
    //然后需要调用方法，肯定是需要jmethodId的 访问方法肯定需要方法id的

    jclass targetClass=(jclass)env->NewGlobalRef(jclz);
    if(targetClass==0){
        return;
    }
    //缓存对象
    jobject targetObject=(jobject)env->NewGlobalRef(instance);
    if(targetObject==0){
        return;
    }

    jmethodID jmid=env->GetStaticMethodID(jclz,"getRandomUUID","()Ljava/lang/String;");
    LOGD("jmid ");
    //然后就是调用,获取uuid 一般uuid是数据库的id或者文件的名字
    jstring uuid= (jstring) env->CallStaticObjectMethod(jclz, jmid);
    //将jstring 转换成char *
    char * uuid_c= (char *) env->GetStringUTFChars(uuid, NULL);
    LOGD("uuid is %s",uuid_c);
    char filename[100];
    sprintf(filename,"/storage/sdcard0/360/%s.txt",uuid_c);
    //这个函数是格式化字符串 将其格式化成文件路径
    // 这里要在手机上创建文件，所以需要申请读写文件权限
    LOGD("uuid is %s",filename);
    FILE *fp=fopen(filename,"w");
    LOGD("fopen成功");
    fputs("i love weiwei!",fp);
    LOGD("写入成功");
    fclose(fp);
    LOGD("关闭文件句柄成功");
}

//访问构造方法
extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_accessConstructor(JNIEnv *env,
                                                                             jobject instance) {
    //通过类的路径，来从jvm里边找到对象类
    //获取类 两种方式，，一个是根据对象获取，一个是根据名字获取，跟java的两种方式类似 Date.class.getName()  date.getClass().getName();
    //通过获得java对象，来调用java中的方法  jni调用java方法，一般都是需要jclass 和方法ID  然后就行调用就行  只是方法不同，获取方法id的方法不同。
    jclass jclz=env->FindClass("java/util/Date");
    LOGD("jclz");
    //然后就是方法ID
    jmethodID jmid=env->GetMethodID(jclz,"<init>","()V");
    LOGD("jmid");
    jobject date_obj=env->NewObject(jclz,jmid); //通过初始化构造方法，来获取类对象      
    LOGD("date_obj");
    //获取getTime() 方法的签名
    jmethodID jmid_time=env->GetMethodID(jclz,"getTime","()J");
    LOGD("jmid_time");
    jlong time=env->CallLongMethod(date_obj,jmid_time);
    LOGD("time");
    return date_obj;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_chinessChars(JNIEnv *env,
                                                                        jobject instance,
                                                                        jstring input) {
    jboolean iscp;
    //c语言String就是char类型指针 char *
    char * str = (char *) env->GetStringUTFChars(input, &iscp);
    LOGD("String is :%s",str);
    //出现乱码，是因为java里边使用的utf-16的编码方式  16bit 无论是中文还是英文都是使用的两个字节
    //而在jni里边使用的utf-8编码方式 这个是一个可变字长的编码方式  英文占一个字节  中文占用三个字节
    // c/c++使用的是ascii编码方式   中文的编码方式是GB2312 中文用两个字节表示一个汉字
    if (iscp==JNI_TRUE){ //说明jvm开辟了新内存，需要释放
        LOGD("iscopy is JNI_TRUE");
        env->ReleaseStringUTFChars(input, str);     //释放字符串，配套使用
    } else if (iscp==JNI_FALSE){
        LOGD("iscopy is JNI_FALSE");
    }

    //经过测试，在jni在studio中，并不会出现中文乱码问题



    return NULL;
}



void insertSort(int a[], int n)
{
    for(int i= 1; i<n; i++){
        if(a[i] < a[i-1]){               //若第i个元素大于i-1元素，直接插入。小于的话，移动有序表后插入
            int j= i-1;   //表中最后一个数据
            int x = a[i];        //复制为哨兵，即存储待排序元素
            a[i] = a[i-1];           //先后移一个元素 (因为a[i]就是X，所以不怕丢失)
            while(j>=0 && x < a[j]){  //查找在有序表的插入位置  (遍历表)
                a[j+1] = a[j];
                j--;         //元素后移
            }
            a[j+1] = x;      //插入到正确位置
        }
    }

}

//对这个int数组进行排序
extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_giveArray(JNIEnv *env, jobject instance,
                                                                        jintArray jIntArr) {

    //首先，转换成int类型指针 java传给jin的数据，使用之前都是需要转换才可以使用。
    jint *arr = env->GetIntArrayElements(jIntArr, NULL);
    if (arr==NULL){//为了程序的健壮性，必须进行判断空
        return;
    }
    int length=env->GetArrayLength(jIntArr);
    //void * 类型的指针是c语言模仿面向对象的一种做法

//    qsort(arr,length, sizeof(jint),compare);
//    qsort(0,0,0,0);
    insertSort(arr,length);
    env->ReleaseIntArrayElements(jIntArr, arr, JNI_COMMIT);
}


int compare (jint* a,jint* b){
    return *a-*b;
}



extern "C"  //这一行太关键了，，因为这都是c语法，所以需要加这个才行
JNIEXPORT jobjectArray JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_initStringArray(JNIEnv *env,
                                                                           jobject instance,
                                                                           jint size) {
    //注意返回类型是jobjectArray  java需要的返回值是String数组 ，这个数组属于引用类型数组，，只要是引用类型数组，在jni中都用jobjectArray类型表示
    //创建jobjectArray
    LOGD("initStringArray");
    jobjectArray jArray;
    jclass jclz;
    int i;
    jclz =env->FindClass("java/lang/String");   //加载这个引用类型的jclass
    if (jclz==NULL){    //引用类型的数据都需要判空 为了程序的健壮性
        return NULL;
    }
    jArray=env->NewObjectArray(size,jclz,instance); //初始化引用对象数组
    if (jArray==NULL){
        return NULL;
    }

    for (int i = 0; i <size ; i++) {
        //c字符串
        char* cStr= (char *) malloc(256);   //初始化一个c的字符串
        memset(cStr,0,256);         //将字符串初始化
        //将int转化成char
        sprintf(cStr,"hello num is %d\n",i);    //格式化字符串，将格式化好的字符串赋给第一个参数
        //将c String转化成jString
        jstring jStr=env->NewStringUTF(cStr);
        if (jStr==NULL){
            return NULL;
        }
        //将jstring赋值给数组
        env->SetObjectArrayElement(jArray,i,jStr);
        free(cStr);
        cStr=NULL;
        env->DeleteLocalRef(jStr); //释放引用类型
    }

    return jArray;


}

//JNI 引用
//局部引用  android系统，在同一个时刻，创建512个局部引用。
//定义方式多样FindClass NewObject GetObjectClass NewCharArray NewLocalRef
//释放方式：1.方法调用完JVM会自动释放 2DeleteLocalRef
//不能在多线程里边使用
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_localRef(JNIEnv *env, jobject instance) {

    int i=0;
    for (int i = 0; i <5 ; i++) {
        jclass jclz=env->FindClass("java/util/Date");   //获取jclass对象
        jmethodID jmid=env->GetMethodID(jclz,"init","()V");
        jobject jobj=env->NewObject(jclz,jmid);     //创建一个Date类型的局部引用
        //使用局部引用

        //TODO

        //释放局部引用
        env->DeleteLocalRef(jclz);
        env->DeleteLocalRef(jobj);

    }

}




//全局引用
//跨进程使用 垮方法使用
//NewGlobalRef 是创建全局引用的唯一方法
jstring str_global;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_createGlobalRef(JNIEnv *env,
                                                                           jobject instance) {

     jobject jobj=env->NewStringUTF("JNI is intersting");
      str_global= (jstring) env->NewGlobalRef(jobj);      //这个是声明全局变量的唯一方法
}

//使用全局引用
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_getGlobalRef(JNIEnv *env,
                                                                        jobject instance) {

    return str_global;
}

//释放全局引用
extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_delGlobalRef(JNIEnv *env,
                                                                        jobject instance) {


    env->DeleteGlobalRef(str_global);

}

//弱引用
//跨进程  垮方法使用  它不会阻止GC
jclass j_weak_cls;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_createWeakRef(JNIEnv *env,
                                                                         jobject instance) {

    jclass jclz=env->FindClass("java/lang/String");
    j_weak_cls= (jclass) env->NewWeakGlobalRef(jclz);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_exception(JNIEnv *env,
                                                                     jobject instance) {
    LOGD("exception");
    jclass jclz=env->GetObjectClass(instance);
    jfieldID fid=env->GetFieldID(jclz,"key","Ljava/lang/String;");  //这一步会出现异常  但是在C语言中，即便出现异常信息，也是会进行下一步的调用  但是，在java中就不能继续进行下面的逻辑了
    LOGD("exception start");
    jthrowable ex=env->ExceptionOccurred();
    if (ex!=NULL){//表示确实有异常信息
        jclass newExc;
        env->ExceptionClear();  //好强大啊，，不会影响代码执行，，
        //然后将异常信息反馈到java层
        //IllegalArgumentException 非法参数异常
        newExc=env->FindClass("java/lang/IllegalArgumentException");
        if(newExc==NULL){
            LOGD("exception 获取error!");
            return ;
        }
        env->ThrowNew(newExc,"Throw Exception From JNI :GetFieldID Exception ");
    }

}

//局部静态变量进行缓存
extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_cached(JNIEnv *env, jobject instance) {

    jclass jclz=env->GetObjectClass(instance);
    static jfieldID fid=NULL;
    if (fid==NULL){
        fid=env->GetFieldID(jclz,"key","Ljava/lang/String;");
        LOGD("getFieldId");
    }

}

/*全局变量缓存*/
jfieldID field_glb;

long get_file_path(const char *path);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_cachedGlobal(JNIEnv *env,
                                                                        jobject instance) {
    jclass jclz=env->GetObjectClass(instance);
    if (field_glb==NULL){
        field_glb=env->GetFieldID(jclz,"key","Ljava/lang/String;");
    }

}

//将缓存策略和弱引用联合引用的问题
JNIEXPORT jstring JNICALL
Java_com_example_administrator_dnndkfilesplit_MainActivity_AcessCacheNewString(JNIEnv *env,
                                                                               jobject instance) {

    //定义一个静态的局部变量
    static jclass cls_string=NULL;
    if(cls_string==NULL){
        //给局部静态变量赋一个局部引用
        cls_string=env->FindClass("");
    }


    return env->NewStringUTF("");
}


long get_file_path(const char *path) {
    FILE * fp=fopen(path,"rb");     //以读的方式打开一个文件，文件必须存在，只进行读
    fseek(fp,0,SEEK_END);
    long ret=ftell(fp);
    fclose(fp);
    return ret;
}

//获取文件大小
long  get_file_size(const char* path) {
    FILE *fp = fopen(path, "rb"); //打开一个文件， 文件必须存在，只运行读
    if (fp==NULL){
        LOGD("get_file_size:原文件打开error");
        return 0;
    }else{
        fseek(fp, 0, SEEK_END);
        long ret = ftell(fp);
        fclose(fp);
        return ret;
    }
}

extern "C"
JNIEXPORT void JNICALL native_diff(JNIEnv *env, jclass type,
                                                             jstring path_, jstring pattern_Path_,
                                                             jint file_num) {
    //首先将jstring转换成char*
    const char *path = env->GetStringUTFChars(path_, 0);
    const char *pattern_Path = env->GetStringUTFChars(pattern_Path_, 0);
    LOGD("JNI native diff Begin");
    //申请一个二维数组 用于存放子文件的名字  二级指针就是二维数组
    char ** patches= (char **) malloc(sizeof(char*) * file_num);
    for (int i = 0; i < file_num; i++) {
        //为每一个文件名字申请地址
        LOGD("char = %d char * = %d", sizeof(char), sizeof(char*));
        patches[i]= (char *) malloc(sizeof(char) * 100);
        // 需要分割的文件 vab.mp4
        // 每个子文件名称 vab_n.mp4
        sprintf(patches[i],pattern_Path,i); //对字符数组进行格式化，将格式结果存放在第一个里边，对第二个进行格式化，使用第三个参数进行传参
        LOGD("path path is %s",patches[i]);
    }

    //获取被拆分文件的大小
    int file_size=get_file_size(path);
    if(file_size==0){
        LOGD("获取原文件大小失败");
        return ;
    }
    LOGD("file size is %d",file_size);
    FILE* fpr=fopen(path,"rb"); //以读的方式打开  打开 原文件

    /*
     * 判断文件是否能被file_num 整除
     * 能整除就平分
     * 不能整除就先评分file_num-1
     * */
    if (file_size% file_num==0){   //能平分的情况
        LOGD("均分");
        int part=file_size / file_num;
        LOGD("part is %d",part);
        for (int i = 0; i <file_num ; i++) {
            FILE* fpw=fopen(patches[i],"wb");   //以只写的方式打开，如果存在就讲文件删除  只负责写数据
            for (int j = 0; j < part; j++) {    //这里分别向各个部分进行写数据  但是 如何记录写数据的起点  是不是根据文件的游标的移动
                fputc(fgetc(fpr), fpw);
            }
            fclose(fpw); //关闭文件指针
        }

    }else{//不能平分
        LOGD("不均分");
        int part = file_size / (file_num - 1);
        LOGD("part is %d",part);
        for (int i = 0; i < file_num-1; i++) {
            FILE * fpw=fopen(patches[i],"wb");  //待写入的文件指针
            for (int j = 0; j <part ; j++) {
                fputc(fgetc(fpr),fpw);
            }
            fclose(fpw);
        }
        FILE * fpw=fopen(patches[file_num-1],"wb");
        for (int i = 0; i <file_size%(file_num-1) ; i++) {
            fputc(fgetc(fpr),fpw);
        }
        fclose(fpw);
    }
    fclose(fpr);
    for (int i =0; i< file_num; i++) {
        free(patches[i]);
    }
    free(patches);
    env->ReleaseStringUTFChars(path_, path);
    env->ReleaseStringUTFChars(pattern_Path_, pattern_Path);
}



extern "C"
JNIEXPORT void JNICALL native_patch(JNIEnv *env, jclass type,
                                   jstring merge_path, jstring pattern_Path_,
                                   jint file_num) {
    //首先将jstring转换成char*
    const char *path = env->GetStringUTFChars(merge_path, 0);
    const char *pattern_Path = env->GetStringUTFChars(pattern_Path_, 0);
    LOGD("JNI native patch Begin");
    //申请一个二维数组 用于存放子文件的名字  二级指针就是二维数组
    char ** patches= (char **) malloc(sizeof(char*) * file_num);
    for (int i = 0; i < file_num; i++) {
        //为每一个文件名字申请地址
        LOGD("char = %d char * = %d", sizeof(char), sizeof(char*));
        patches[i]= (char *) malloc(sizeof(char) * 100);
        // 需要分割的文件 vab.mp4
        // 每个子文件名称 vab_n.mp4
        sprintf(patches[i],pattern_Path,i); //对字符数组进行格式化，将格式结果存放在第一个里边，对第二个进行格式化，使用第三个参数进行传参
        LOGD("path path is %s",patches[i]);
    }

    FILE * fpw=fopen(path,"wb");    //在这个文件里边写数据
    for (int i = 0; i < file_num; i++) {
        FILE *fpr=fopen(patches[i],"rb");
        int fileSize=get_file_size(patches[i]);
        for (int i = 0; i <fileSize ; ++i) {
            fputc(fgetc(fpr),fpw);
        }
        fclose(fpr);
    }
    fclose(fpw);
    for (int i = 0; i <file_num ; i++) {
        free(patches[i]);       //每一个malloc对应一个free
    }
    env->ReleaseStringUTFChars(merge_path, path);
    env->ReleaseStringUTFChars(pattern_Path_, pattern_Path);
}
_JavaVM * g_jvm=NULL;
jobject g_obj=NULL;

/*创建函数指针*/
void* thread_fun(void * arg) {
    JNIEnv *env;
    jclass jclz;
    jmethodID mid,mid1;

    if ((*g_jvm).AttachCurrentThread(&env,NULL)!=JNI_OK){
        LOGD("%s AttachCurrentThread error failed",__FUNCTION__);
        return NULL;
    }

    jclz=env->GetObjectClass(g_obj);
    if(jclz==NULL){
        LOGD("findClass error");
        return NULL;
    }

    LOGI("call back begin");
    mid=env->GetStaticMethodID(jclz,"fromJni","(I)V");
    if(mid==NULL){
        LOGD("getStaticMethodId error");
        return NULL;
    }

    env->CallStaticVoidMethod(jclz,mid,(int)arg);

    mid1 = (env) -> GetMethodID( jclz, "form_JNI_Again", "(I)V");
    if (mid1 == NULL) {
        LOGI("GetMethodID error....");
      return NULL;
    }
    (env) ->CallVoidMethod(g_obj, mid1 ,(int)arg);

    if((*g_jvm).DetachCurrentThread()!=JNI_OK){
        LOGD("%s DetachCurrentThread error",__FUNCTION__);
    }

    pthread_exit(0);
}



//jni创建线程
JNIEXPORT void JNICALL native_newThead(JNIEnv *env,jclass jclz){
    LOGD("newThread begin");
    int i;
    pthread_t pt[5];

    for (int i = 0; i < 5; i++) {
        //在jni中创建子线程
        pthread_create(&pt[i],NULL,&thread_fun,(void*)i);
    }
}

//
JNIEXPORT void JNICALL native_setJniEnv(JNIEnv *env,jobject jobj){
    LOGD("native_setJniEnv");
    //保存JVM
    env->GetJavaVM(&g_jvm);
    //保持activity对象
    g_obj=env->NewGlobalRef(jobj);
}

static const JNINativeMethod gMethods[]={
        {"diff","(Ljava/lang/String;Ljava/lang/String;I)V",(void*)native_diff},
        {"patch","(Ljava/lang/String;Ljava/lang/String;I)V",(void*)native_patch}
};



static int registerNatives(JNIEnv* env){
    LOGD("registerNatives Begin ！");
    jclass jclz=env->FindClass("com/example/administrator/dnndkfilesplit/FileUtils");
    if (jclz==NULL){
        LOGD("jclass is NULL");
        return JNI_FALSE;
    }
    if (env->RegisterNatives(jclz,gMethods,NELEM(gMethods))<0){
        LOGD("RegistterNatives error");
        return JNI_FALSE;
    }
    return JNI_TRUE;
};

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){
    LOGI("jni_OnLoad begin");

    JNIEnv* env=NULL;
    jint result=-1;
    if ((vm)->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGI("ERROR: GetEnv failed\n");
        return -1;
    }
    assert(env != NULL);
    registerNatives(env);
    return JNI_VERSION_1_4;
}

extern "C"
int add(int a ,int b){
    LOGD("a is %d,b is %d",a,b);
    return a+b;
}