#include <jni.h>
#include <string>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>

//使用命名空间 cv 这样才能调用Mat

using namespace cv;


//bitmap 转 mat
void bitmapToMat(JNIEnv *env, Mat &mat, jobject bitmap) {
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    //锁定bitmap 画布
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    //指定 mat的宽，高和type BGRA
    mat.create(info.height, info.width, CV_8UC4);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        //对应的mat 是 CV_8UC4
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        temp.copyTo(mat);
        //把数据temp赋值到 mat里面
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        //对应的mat 是 CV_8UC2
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        //CV_8UC2 => CV_8UC4
        cvtColor(temp, mat, COLOR_BGR5652BGRA);
    }

    //其他要自己去转

    //解锁画布
    AndroidBitmap_unlockPixels(env, bitmap);

}

void mat2Bitmap(JNIEnv *env, Mat mat, jobject bitmap) {
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    //锁定bitmap 画布
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    //指定 mat的宽，高和type BGRA

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        if (mat.type() == CV_8UC4) {
            mat.copyTo(temp);
        } else if (mat.type() == CV_8UC2) {
            cvtColor(mat, temp, COLOR_BGR5652BGRA);
        } else if (mat.type() == CV_8UC1) {  //灰度图像
            cvtColor(mat, temp, COLOR_GRAY2BGRA);
        }
        mat.copyTo(temp);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        //对应的mat 是 CV_8UC2
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        //CV_8UC2 => CV_8UC4
        if (mat.type() == CV_8UC4) {
            cvtColor(mat, temp, COLOR_BGRA2BGR565);
        } else if (mat.type() == CV_8UC2) {

            mat.copyTo(temp);
        } else if (mat.type() == CV_8UC1) {  //灰度图像
            cvtColor(mat, temp, COLOR_GRAY2BGR565);
        }
    }

    //其他要自己去转

    //解锁画布
    AndroidBitmap_unlockPixels(env, bitmap);


}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_facemanager_FaceDetection_faceDetectionSaceInfo(JNIEnv *env, jobject thiz,
                                                                 jobject bitmap) {
    // TODO: implement faceDetectionSaceInfo()

    //检测人脸 openvc 有一个非常关键的类 Mat opencv 是C 和 c++写的，只会处理Mat,但是android 里面只有bitmap
    //所以第一步就是把bitmap 转化为Mat  Mat 是一个矩阵
    Mat mat;
    bitmapToMat(env, mat, bitmap);


    //处理灰度图  设置成灰度图提高效率  为什么
    Mat greyMat;
    cvtColor(mat, greyMat, COLOR_BGRA2GRAY);

    //把mat 放回到bitmap设置回去

    mat2Bitmap(env, greyMat, bitmap);

    //保存人脸信息

    //灰度图算法 在java中将一个图片转成灰度图 怎么做
    //RGB 0-255 0-255 0-255 (一张彩色图）
    //灰度图 R+G+B=255 =0.33R+0.33G+0.33B =0.3R+0.59G+0.11B(更加适合人的眼睛）
    //灰度图 所含有的信息（取值范围0-255）不高

}


