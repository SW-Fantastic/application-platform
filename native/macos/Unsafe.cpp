#include "jni.h"
#include "Unsafe.h"
#include <stdlib.h>
#include <string.h>

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    malloc
 * Signature: (J)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_malloc
  (JNIEnv * env, jclass, jlong size) {
      
      void* mem = malloc(size);
      if(mem == NULL) {
        return NULL;
      }
      return env->NewDirectByteBuffer(mem, size);
  }

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    relloc
 * Signature: (Ljava/nio/ByteBuffer;J)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_relloc
  (JNIEnv * env, jclass, jobject bytebuffer, jlong size) {

    if(bytebuffer == NULL) {
        return NULL;
    }

    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    long oldSize = env->GetDirectBufferCapacity(bytebuffer);

    if(size != oldSize) {
        void* target = realloc(ptr,size);
        return env->NewDirectByteBuffer(target, size);
    }
    return bytebuffer;
  }

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    memcpy
 * Signature: (Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_memcpy
  (JNIEnv * env, jclass, jobject bytebuffer) {

    if(bytebuffer == NULL) {
        return NULL;
    }

    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    long size = env->GetDirectBufferCapacity(bytebuffer);

    void* target = malloc(size);
    memcpy(target, ptr, size);
    return env->NewDirectByteBuffer(target, size);
  }

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    free
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_swdc_platforms_Unsafe_free
  (JNIEnv * env, jclass, jobject bytebuffer) {

    if(bytebuffer == NULL) {
        return;
    }

    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    free(ptr);
  }
