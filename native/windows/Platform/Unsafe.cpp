#include "jni.h"
#include "org_swdc_platforms_Unsafe.h"
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "Base64.h"

typedef unsigned __int8 uint8_t;


/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    malloc
 * Signature: (J)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_malloc
(JNIEnv* env, jclass, jlong size) {

    void* mem = malloc(size);
    if (mem == NULL) {
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
(JNIEnv* env, jclass, jobject bytebuffer, jlong size) {

    if (bytebuffer == NULL) {
        return NULL;
    }

    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    long oldSize = env->GetDirectBufferCapacity(bytebuffer);

    if (size != oldSize) {
        void* target = realloc(ptr, size);
        return env->NewDirectByteBuffer(target, size);
    }
    return bytebuffer;
}

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    memcpy
 * Signature: (Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;
 */

JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_memcpy__Ljava_nio_ByteBuffer_2
(JNIEnv* env, jclass, jobject bytebuffer) {

    if (bytebuffer == NULL) {
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
(JNIEnv* env, jclass, jobject bytebuffer, jint offset, jint length) {
    if (bytebuffer == NULL) {
        return NULL;
    }
    uint8_t* ptr = (uint8_t*)env->GetDirectBufferAddress(bytebuffer);
    long size = env->GetDirectBufferCapacity(bytebuffer);
    if (size < length || ptr + offset + length > ptr + size) {
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
(JNIEnv* env, jclass, jobject destBuffer, jint destOffset, jobject srcBuffer, jint srcOffset, jint length) {

    if (destBuffer == NULL || srcBuffer == NULL) {
        return JNI_FALSE;
    }

    long destSize = env->GetDirectBufferCapacity(destBuffer);
    long srcSize = env->GetDirectBufferCapacity(srcBuffer);

    uint8_t* ptrDest = (uint8_t*)env->GetDirectBufferAddress(destBuffer);
    uint8_t* ptrSrc = (uint8_t*)env->GetDirectBufferAddress(srcBuffer);

    if (ptrDest + destOffset + length > ptrDest + destSize) {
        return JNI_FALSE;
    }

    if (ptrSrc + srcOffset + length > ptrSrc + srcSize) {
        return JNI_FALSE;
    }

    memcpy(ptrDest + destOffset, ptrSrc + srcOffset, length);
    return JNI_TRUE;
}

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    mmap
 * Signature: (Ljava/lang/String;IIJ)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_swdc_platforms_Unsafe_mmap
(JNIEnv* env, jclass, jstring filePath, jint type, jint offset, jlong length) {

    LPWSTR fp = (LPWSTR)env->GetStringChars(filePath, 0);

    int sizeOfPath = 6 + lstrlenW(fp);
    LPWSTR fpName = new wchar_t[sizeOfPath + 6];
    Base64_Encode(fpName + 6, fp, sizeOfPath);
    memcpy(fpName, "Local\\",6);

    DWORD protect = PAGE_READONLY;
    switch (type) {
    case 0:
        protect = PAGE_READONLY;
        break;
    case 1:
    case 2:
        protect = PAGE_READWRITE;
        break;
    }

    HANDLE mapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL,fpName);
    if (mapping == NULL || mapping == INVALID_HANDLE_VALUE) {
        HANDLE hFile = CreateFile( 
            fp,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
            NULL, OPEN_EXISTING, 
            FILE_FLAG_RANDOM_ACCESS,
            NULL 
        );
        if (hFile == NULL || hFile == INVALID_HANDLE_VALUE) { 
            delete[] fpName;
            return NULL;
        }

        mapping = CreateFileMapping(hFile, NULL, protect, 0, 0, fpName);
        if (mapping == NULL || mapping == INVALID_HANDLE_VALUE) {
            delete[] fpName;
            return NULL;
        }
        CloseHandle(hFile);
    }
    DWORD acc = FILE_MAP_READ;
    switch (type){
        case 0:
            acc = FILE_MAP_READ;
            break;
        case 1:
            acc = FILE_MAP_WRITE;
            break;
        case 2:
            acc = FILE_MAP_READ | FILE_MAP_WRITE;
            break;
    }

    ULARGE_INTEGER ulargeOffset;
    ulargeOffset.QuadPart = offset;

    SYSTEM_INFO info;
    GetSystemInfo(&info);

    // 这个是内存分配粒度，以这个为基础进行Offset的对齐。
    DWORD allocationSize = info.dwAllocationGranularity;
    unsigned int uOffset = (unsigned int)offset;
    if (uOffset % allocationSize > 0) {
        int offsetLength = uOffset % allocationSize;
        uOffset = uOffset - offsetLength;
        length = length + offsetLength;
        ulargeOffset.QuadPart = uOffset;
    }

    HANDLE mapped = MapViewOfFile(mapping, acc, ulargeOffset.HighPart, ulargeOffset.LowPart, length);
    CloseHandle(mapping);
    delete[] fpName;
    if (mapped == NULL || mapped == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    return env->NewDirectByteBuffer(mapped, length);
}

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    msync
 * Signature: (Ljava/nio/ByteBuffer;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_msync
(JNIEnv* env, jclass, jobject bytebuffer) {

    if (bytebuffer == NULL) {
        return JNI_FALSE;
    }
    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    long size = env->GetDirectBufferCapacity(bytebuffer);
    FlushViewOfFile(ptr, size);
    return JNI_TRUE;
}

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    munmap
 * Signature: (Ljava/nio/ByteBuffer;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_swdc_platforms_Unsafe_munmap
(JNIEnv* env, jclass, jobject bytebuffer) {

    if (bytebuffer == NULL) {
        return JNI_FALSE;
    }
    void* ptr = env->GetDirectBufferAddress(bytebuffer);
    int rst = UnmapViewOfFile(ptr);
    return rst == 0 ? JNI_FALSE : JNI_TRUE;
}


/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    free
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_swdc_platforms_Unsafe_free
(JNIEnv* env, jclass, jobject bytebuffer) {

    if (bytebuffer == NULL) {
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

/*
 * Class:     org_swdc_platforms_Unsafe
 * Method:    getMemoryAlignment
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_swdc_platforms_Unsafe_getMemoryAlignment
(JNIEnv*, jclass) {

    SYSTEM_INFO info;
    GetSystemInfo(&info);

    return (jlong)static_cast<long>(info.dwAllocationGranularity);
}