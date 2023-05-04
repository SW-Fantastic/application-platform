#include "jni.h"
#include "Unsafe.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

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
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_memcpy_2
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
 * Method:    memcpy
 * Signature: (Ljava/nio/ByteBuffer;II)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_memcpy__Ljava_nio_ByteBuffer_2II
  (JNIEnv * env, jclass, jobject bytebuffer, jint offset, jint length) {

    if(bytebuffer == NULL) {
        return NULL;
    }

    uint8_t* ptr = (uint8_t*)env->GetDirectBufferAddress(bytebuffer);
    long size = env->GetDirectBufferCapacity(bytebuffer);
    if(size < length || ptr + offset + length > ptr + size) {
      return NULL;
    }
    void* target = malloc(length);
    memcpy(target, ptr + offset, length);
    return env->NewDirectByteBuffer(target, length);
  }

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    memcpy
 * Signature: (Ljava/nio/ByteBuffer;ILjava/nio/ByteBuffer;II)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_memcpy__Ljava_nio_ByteBuffer_2ILjava_nio_ByteBuffer_2II
  (JNIEnv * env, jclass, jobject destBuffer, jint destOffset, jobject srcBuffer, jint srcOffset, jint length){

    if(destBuffer == NULL || srcBuffer == NULL) {
        return JNI_FALSE;
    }

    long destSize = env->GetDirectBufferCapacity(destBuffer);
    long srcSize = env->GetDirectBufferCapacity(srcBuffer);

    uint8_t* ptrDest = (uint8_t*)env->GetDirectBufferAddress(destBuffer);
    uint8_t* ptrSrc = (uint8_t*)env->GetDirectBufferAddress(srcBuffer);

    if(ptrDest + destOffset + length > ptrDest + destSize) {
      return JNI_FALSE;
    }

    if(ptrSrc + srcOffset + length > ptrSrc + srcSize) {
      return JNI_FALSE;
    }

    memcpy(ptrDest + destOffset, ptrSrc + srcOffset, length);
    return JNI_TRUE;
  }

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    mmap
 * Signature: (Ljava/lang/String;III)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_mmap
  (JNIEnv * env, jclass, jstring path, jint type, jint offset, jlong length) {

    if(path == NULL || offset < 0 || length <= 0) {
      return NULL;
    }
    const char* filePath = env->GetStringUTFChars(path,NULL);
    int fd = open(filePath, O_RDWR);
    env->ReleaseStringUTFChars(path,filePath);

    if(fd == -1) {
      return NULL;
    }

    int flags = 0;
    switch (type){
      case 0:
        flags = PROT_READ;
        break;
      case 1:
        flags = PROT_WRITE;
        break;
      case 2:
        flags = PROT_READ|PROT_WRITE;
        break;
    }
    int pageSize = sysconf(_SC_PAGE_SIZE);
    if(length % pageSize > 0) {
      length = length + (pageSize - (length % pageSize));
    }

    void* target = mmap(NULL,length,flags, MAP_SHARED,fd,offset);
    close(fd);
    if(target != MAP_FAILED) {
      return env->NewDirectByteBuffer(target, length);
    }
    return NULL;
}
/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    msync
 * Signature: (Ljava/nio/ByteBuffer;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_msync
  (JNIEnv * env, jclass, jobject bytebuffer) {
    if(bytebuffer == NULL) {
        return JNI_FALSE;
    }
    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    long size = env->GetDirectBufferCapacity(bytebuffer);
    msync(ptr,size,MS_SYNC);
    return JNI_TRUE;
  }

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    munmap
 * Signature: (Ljava/nio/ByteBuffer;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_munmap
  (JNIEnv * env, jclass, jobject bytebuffer) {
    
    if(bytebuffer == NULL) {
        return JNI_FALSE;
    }
    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    long size = env->GetDirectBufferCapacity(bytebuffer);
    munmap(ptr,size);
    return JNI_TRUE;
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


/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    sizeOf
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_org_swdc_platforms_Unsafe_sizeOf
(JNIEnv* env, jclass clazz, jint type) {

    switch (type)
    {
    case 0:
        return sizeof(int);
    case 1:
        return sizeof(short);
    case 2:
        return sizeof(long);
    case 3:
        return sizeof(char);
    case 4:
        return sizeof(intptr_t);
    default:
        return 0;
    }

}
