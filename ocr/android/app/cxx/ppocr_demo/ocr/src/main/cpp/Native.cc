// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Native.h"
#include "pipeline.h"
#include <android/log.h>
#include <android/bitmap.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_baidu_paddle_lite_demo_ocr_db_crnn_Native
 * Method:    nativeInit
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)J
 */
//JNIEXPORT jlong JNICALL
//Java_com_baidu_paddle_lite_demo_ppocr_1demo_Native_nativeInit(
//    JNIEnv *env, jclass thiz, jstring jDetModelPath, jstring jClsModelPath,
//    jstring jRecModelPath, jstring jConfigPath, jstring jLabelPath,
//    jint cpuThreadNum, jstring jCPUPowerMode) {
//  std::string detModelPath = jstring_to_cpp_string(env, jDetModelPath);
//  std::string clsModelPath = jstring_to_cpp_string(env, jClsModelPath);
//  std::string recModelPath = jstring_to_cpp_string(env, jRecModelPath);
//  std::string configPath = jstring_to_cpp_string(env, jConfigPath);
//  std::string labelPath = jstring_to_cpp_string(env, jLabelPath);
//  std::string cpuPowerMode = jstring_to_cpp_string(env, jCPUPowerMode);
//
//  return reinterpret_cast<jlong>(
//      new Pipeline(detModelPath, clsModelPath, recModelPath, cpuPowerMode,
//                   cpuThreadNum, configPath, labelPath));
//}

/*
 * Class:     com_baidu_paddle_lite_demo_ocr_db_crnn_Native
 * Method:    nativeRelease
 * Signature: (J)Z
 */
//JNIEXPORT jboolean JNICALL
//Java_com_baidu_paddle_lite_demo_ppocr_1demo_Native_nativeRelease(JNIEnv *env,
//                                                                 jclass thiz,
//                                                                 jlong ctx) {
//  if (ctx == 0) {
//    return JNI_FALSE;
//  }
//  Pipeline *pipeline = reinterpret_cast<Pipeline *>(ctx);
//  delete pipeline;
//  return JNI_TRUE;
//}

/*
 * Class:     com_baidu_paddle_lite_demo_ocr_db_crnn_Native
 * Method:    nativeProcess
 * Signature: (JIIIILjava/lang/String;)Z
 */
//JNIEXPORT jboolean JNICALL
//Java_com_baidu_paddle_lite_demo_ppocr_1demo_Native_nativeProcess(
//    JNIEnv *env, jclass thiz, jlong ctx, jint inTextureId, jint outTextureId,
//    jint textureWidth, jint textureHeight, jstring jsavedImagePath) {
//  if (ctx == 0) {
//    return JNI_FALSE;
//  }
//  std::string savedImagePath = jstring_to_cpp_string(env, jsavedImagePath);
//  Pipeline *pipeline = reinterpret_cast<Pipeline *>(ctx);
//  return pipeline->Process_val(inTextureId, outTextureId, textureWidth,
//                               textureHeight, savedImagePath);
//}

#ifdef __cplusplus
}
#endif

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_ocrtext_BocOcrPredictor_nativeInit(JNIEnv *env, jclass clazz,
                                                    jstring det_model_path, jstring cls_model_path,
                                                    jstring rec_model_path, jstring config_path,
                                                    jstring label_path, jint cput_thread_num,
                                                    jstring cpu_power_mode) {
    std::string detModelPath = jstring_to_cpp_string(env, det_model_path);
    std::string clsModelPath = jstring_to_cpp_string(env, cls_model_path);
    std::string recModelPath = jstring_to_cpp_string(env, rec_model_path);
    std::string configPath = jstring_to_cpp_string(env, config_path);
    std::string labelPath = jstring_to_cpp_string(env, label_path);
    std::string cpuPowerMode = jstring_to_cpp_string(env, cpu_power_mode);

    return reinterpret_cast<jlong>(
            new Pipeline(detModelPath, clsModelPath, recModelPath, cpuPowerMode,
                         cput_thread_num, configPath, labelPath));
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_ocrtext_BocOcrPredictor_nativeRelease(JNIEnv *env, jclass clazz, jlong ctx) {

    if (ctx == 0) {
        return JNI_FALSE;
    }
    Pipeline *pipeline = reinterpret_cast<Pipeline *>(ctx);
    delete pipeline;
    return JNI_TRUE;

}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_ocrtext_BocOcrPredictor_nativeProcess(JNIEnv *env, jclass clazz, jlong ctx,
                                                       jint in_texture_id, jint out_texture_id,
                                                       jint texture_width, jint texture_height,
                                                       jstring saved_image_path) {
    if (ctx == 0) {
        return JNI_FALSE;
    }
    std::string savedImagePath = jstring_to_cpp_string(env, saved_image_path);
    Pipeline *pipeline = reinterpret_cast<Pipeline *>(ctx);
    return pipeline->Process_val(in_texture_id, out_texture_id, texture_width,
                                 texture_height, savedImagePath);


}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_ocrtext_BocOcrPredictor_ocr(JNIEnv *env, jclass clazz, jlong ctx, jobject bitmap) {
    if (ctx == 0) {
        return JNI_FALSE;
    }
    Pipeline *pipeline = reinterpret_cast<Pipeline *>(ctx);
    AndroidBitmapInfo info;
    void* pixels = nullptr;
    cv::Mat mat;

    // 获取Bitmap信息
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) {
        return nullptr; // 获取信息失败
    }

    // 锁定Bitmap以获取像素数据
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) {
        return nullptr; // 锁定失败
    }

    // 根据Bitmap格式创建Mat对象
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        mat = cv::Mat(info.height, info.width, CV_8UC4, pixels);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        mat = cv::Mat(info.height, info.width, CV_8UC2, pixels);
        cv::cvtColor(mat, mat, cv::COLOR_BGR5652BGR);
    } else {
        AndroidBitmap_unlockPixels(env, bitmap); // 解锁
        return nullptr; // 不支持的格式
    }

    // 复制数据以确保在Bitmap解锁后仍然可用
    mat = mat.clone();

    // 解锁Bitmap
    AndroidBitmap_unlockPixels(env, bitmap);
    std::vector<std::pair<std::string, float>>  res=  pipeline->Process_single(mat);
    // 获取 ArrayList 类
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    if (arrayListClass == nullptr) {
        return nullptr; // 找不到 ArrayList 类
    }

    // 获取 ArrayList 的构造函数
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    if (arrayListConstructor == nullptr) {
        return nullptr; // 找不到构造函数
    }

    // 创建 ArrayList 实例
    jobject arrayList = env->NewObject(arrayListClass, arrayListConstructor);
    if (arrayList == nullptr) {
        return nullptr; // 创建失败
    }

    // 获取 ArrayList 的 add 方法
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    if (addMethod == nullptr) {
        return nullptr; // 找不到 add 方法
    }

    // 获取 String 类
    jclass stringClass = env->FindClass("java/lang/String");
    if (stringClass == nullptr) {
        return nullptr; // 找不到 String 类
    }

    for (int i = res.size() - 1; i >= 0; i--) {
        std::pair<std::string, float> single = res[i];
        // 创建 Java 的 String 对象
        jstring javaString = env->NewStringUTF(single.first.c_str());
        if (javaString == nullptr) {
            continue; // 创建失败，跳过
        }

        // 调用 ArrayList 的 add 方法
        env->CallBooleanMethod(arrayList, addMethod, javaString);

        // 释放 String 对象
        env->DeleteLocalRef(javaString);
    }

    return arrayList;

}