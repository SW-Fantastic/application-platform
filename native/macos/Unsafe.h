/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class org_swdc_platforms_Unsafe */

#ifndef _Included_org_swdc_platforms_Unsafe
#define _Included_org_swdc_platforms_Unsafe
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    malloc
 * Signature: (J)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_malloc
  (JNIEnv *, jclass, jlong);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    relloc
 * Signature: (Ljava/nio/ByteBuffer;J)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_relloc
  (JNIEnv *, jclass, jobject, jlong);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    memcpy
 * Signature: (Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_memcpy_2
  (JNIEnv *, jclass, jobject);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    memcpy
 * Signature: (Ljava/nio/ByteBuffer;II)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_memcpy__Ljava_nio_ByteBuffer_2II
  (JNIEnv *, jclass, jobject, jint, jint);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    memcpy
 * Signature: (Ljava/nio/ByteBuffer;ILjava/nio/ByteBuffer;II)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_memcpy__Ljava_nio_ByteBuffer_2ILjava_nio_ByteBuffer_2II
  (JNIEnv *, jclass, jobject, jint, jobject, jint, jint);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    mmap
 * Signature: (Ljava/lang/String;III)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_mmap
  (JNIEnv *, jclass, jstring, jint, jint, jlong);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    msync
 * Signature: (Ljava/nio/ByteBuffer;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_msync
  (JNIEnv *, jclass, jobject);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    munmap
 * Signature: (Ljava/nio/ByteBuffer;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_munmap
  (JNIEnv *, jclass, jobject);


/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    free
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_swdc_platforms_Unsafe_free
  (JNIEnv *, jclass, jobject);


/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    sizeOf
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_org_swdc_platforms_Unsafe_sizeOf
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif
