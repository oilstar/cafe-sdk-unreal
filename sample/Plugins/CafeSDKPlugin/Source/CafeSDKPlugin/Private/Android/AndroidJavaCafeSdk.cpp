// Copyright 2016 NAVER Corp. All rights reserved.

#include "CafeSDKPluginPrivatePCH.h"
#include "AndroidJavaCafeSdk.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"

FAndroidJavaCafeSdk::FAndroidJavaCafeSdk()
    : FJavaClassObject(GetClassName(), "()V")
    , InitMethod(GetClassMethod("init", "(Ljava/lang/String;Ljava/lang/String;I)V"))
{
}

void FAndroidJavaCafeSdk::Init(FString ClientId, FString ClientSecret, int32 CafeId) const
{
    CallMethod<void>(InitMethod,
        FJavaClassObject::GetJString(ClientId),
        FJavaClassObject::GetJString(ClientSecret),
        CafeId
        );
}

FName FAndroidJavaCafeSdk::GetClassName()
{
    if (FAndroidMisc::GetAndroidBuildVersion() >= 1)
    {
        return FName("cafesdk.CafeSdk");
    }
    else
    {
        return FName("");
    }
}

extern "C" void Java_cafesdk_CafeSdk_nativeOnSdkStarted(JNIEnv* jenv, jobject thiz)
{
    FCafeSDKPluginModule::OnCafeSdkStarted.Broadcast();
}

extern "C" void Java_cafesdk_CafeSdk_nativeOnSdkStopped(JNIEnv* jenv, jobject thiz)
{
    FCafeSDKPluginModule::OnCafeSdkStopped.Broadcast();
}

extern "C" void Java_cafesdk_CafeSdk_nativeOnClickAppSchemeBanner(JNIEnv* jenv, jobject thiz, jstring AppScheme)
{
    const char* AppSchemeChars = jenv->GetStringUTFChars(AppScheme, 0);
    
    FString appScheme = UTF8_TO_TCHAR(AppSchemeChars);
    FCafeSDKPluginModule::OnCafeSdkClickAppSchemeBanner.Broadcast(appScheme);
    
    jenv->ReleaseStringUTFChars(AppScheme, AppSchemeChars);
}

extern "C" void Java_cafesdk_CafeSdk_nativeOnJoined(JNIEnv* jenv, jobject thiz)
{
    FCafeSDKPluginModule::OnCafeSdkJoined.Broadcast();
}

extern "C" void Java_cafesdk_CafeSdk_nativeOnPostedArticle(JNIEnv* jenv, jobject thiz, jint MenuId)
{
    FCafeSDKPluginModule::OnCafeSdkPostedArticle.Broadcast(MenuId);
}

extern "C" void Java_cafesdk_CafeSdk_nativeOnPostedComment(JNIEnv* jenv, jobject thiz, jint ArticleId)
{
    FCafeSDKPluginModule::OnCafeSdkPostedComment.Broadcast(ArticleId);
}