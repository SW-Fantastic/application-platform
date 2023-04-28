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
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_memcpy
  (JNIEnv *, jclass, jobject);

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    free
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_swdc_platforms_Unsafe_free
  (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif