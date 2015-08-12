#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//在c中打印logcat日志
#include <android/log.h>
#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void showJavaDialog(JNIEnv* env, jobject obj,char * msg) {
	//怎么把这些提示信息在ui中显示出来呢?这就是c调用java.实质上还是用反射
	//加载类
	//jclass      (*FindClass)(JNIEnv*, const char*);
	jclass clazz = (*env)->FindClass(env, "com/tencent/stimulatepayinmoblie/MainActivity");
	//查找方法
	//jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
	//方法签名,通过javap来获得. (Ljava/lang/String;)V
	jmethodID methodID = (*env)->GetMethodID(env, clazz, "showDialog",
			"(Ljava/lang/String;)V");
	//调用方法
	//void        (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
	(*env)->CallVoidMethod(env, obj, methodID, msg);
}
void dismissJavaDialog(JNIEnv* env, jobject obj){
	jclass clazz = (*env)->FindClass(env, "com/tencent/stimulatepayinmoblie/MainActivity");
	jmethodID methodID = (*env)->GetMethodID(env, clazz, "dismissDialog",
				"()V");
	(*env)->CallVoidMethod(env, obj, methodID);
}

/*
 * Class:     com_tencent_stimulatepayinmoblie_MainActivity
 * Method:    safePay
 * Signature: (Ljava/lang/String;Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL Java_com_tencent_stimulatepayinmoblie_MainActivity_safePay
  (JNIEnv * env, jobject obj, jstring username, jstring password, jfloat money){
	int result=0;
	//只是模拟支付,如果传进来的用户名密码支付金额都满足条件,那么久提示支付成功
	//没有联网校验什么的
	//先把传进来的额字符串转化成c中char *
	//void        (*GetStringUTFRegion)(JNIEnv*, jstring, jsize, jsize, char*);

	//jsize       (*GetStringLength)(JNIEnv*, jstring);
	//获取申请空间的长度
	int len=(*env)->GetStringLength(env,username);
	//为这个字符数组申请内存空间
	char *cusername=(char *)malloc(len+1);
	(*env)->GetStringUTFRegion(env,username,0,len,cusername);

	int len1=(*env)->GetStringLength(env,password);
	char *cpassword=(char *)malloc(len1+1);
	(*env)->GetStringUTFRegion(env,password,0,len1,cpassword);
	//LOGD("username=%s",username);
	//这里使用了没有转换的java的字符串
	LOGD("username=%s",cusername);

	LOGD("password=%s",cpassword);
	LOGD("money=%f",money);
	//怎么把这些提示信息在ui中显示出来呢?这就是c调用java.实质上还是用反射
		//加载类
		//jclass      (*FindClass)(JNIEnv*, const char*);
	showJavaDialog(env, obj,"正在加密用户名和密码");
	sleep(2);
	showJavaDialog(env, obj,"正在检查支付环境");
	sleep(2);
	showJavaDialog(env, obj,"正在连接服务器");
	sleep(2);
	dismissJavaDialog(env,obj);
	//LOGI("正在加密用户名和密码");
	//LOGI("正在检查支付环境");
	//LOGI("正在连接服务器");
	if(strcmp(cusername,"abc")==0&&strcmp(cpassword,"123")==0)
	{
		if(money<5000){
			result=200;
		}else{
			result=403;
		}
	}else{
		result=404;
	}
	free(cusername);
	free(cpassword);
	return result;
}
