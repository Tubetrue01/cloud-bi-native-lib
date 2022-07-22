//
// Created by Tubetrue01 on 2022/7/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <ev.h>
#include "org_cloud_web_natives_NativeTools.h"


JavaVM *g_VM;

typedef struct
{
    jobject callback;
    jstring jpath;
} callback_t;

typedef struct
{
    ev_stat file_stat;
    callback_t *callback_pack;
} watch_ctx_t;

static void doListener(callback_t *);


static void
file_cb(struct ev_loop *loop, ev_stat *w, int revents)
{
    watch_ctx_t *watch_ctx = (watch_ctx_t *) w;
    if (w->attr.st_nlink)
    {
        doListener(watch_ctx->callback_pack);
    } else
    {
        puts("不期待的文件状态发生！");
        free(watch_ctx->callback_pack);
        free(watch_ctx);
        exit(EXIT_FAILURE);
    }
}

JNIEXPORT void JNICALL
Java_org_cloud_web_natives_NativeTools_watchFile(JNIEnv *env, jclass jc, jstring jpath, jobject callback)
{
    (*env)->GetJavaVM(env, &g_VM);

    callback_t *callback_pack = malloc(sizeof(callback_t));

    callback_pack->jpath = (*env)->NewWeakGlobalRef(env, jpath);
    callback_pack->callback = (*env)->NewWeakGlobalRef(env, callback);

    const char *filePath = (*env)->GetStringUTFChars(env, jpath, NULL);
    printf("ready to listen file : %s\n", filePath);

    struct ev_loop *loop = EV_DEFAULT;
    watch_ctx_t *watch_ctx = malloc(sizeof(watch_ctx_t));
    watch_ctx->callback_pack = callback_pack;

    ev_stat_init (&watch_ctx->file_stat, file_cb, filePath, 0.);
    ev_stat_start(loop, &watch_ctx->file_stat);
    ev_run(loop, 0);
}


static void doListener(callback_t *callback_pack)
{
    if (callback_pack == NULL)
    {
        return;
    }

    JNIEnv *env;
    int getEnvStat = (*g_VM)->GetEnv(g_VM, (void **) &env, JNI_VERSION_1_8), needDetach = JNI_FALSE;

    if (getEnvStat == JNI_EDETACHED)
    {
        if ((*g_VM)->AttachCurrentThread(g_VM, (void **) &env, NULL) != 0)
        {
            return;
        }
        needDetach = JNI_TRUE;
    }

    jclass jclazz = (*env)->GetObjectClass(env, callback_pack->callback);

    if (jclazz == NULL)
    {
        (*env)->ThrowNew(env, jclazz, "Unable to find class");
        (*g_VM)->DetachCurrentThread(g_VM);
        return;
    }

    jmethodID callbackFn = (*env)->GetMethodID(env, jclazz, "accept", "(Ljava/lang/Object;)V");

    if (callbackFn == 0)
    {
        (*env)->ThrowNew(env, jclazz, "Unable to find this method");
        return;
    }

    jint ret = (*env)->CallIntMethod(env, callback_pack->callback, callbackFn, callback_pack->jpath);

    if (needDetach)
    {
        (*g_VM)->DetachCurrentThread(g_VM);
    }
}
