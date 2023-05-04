package org.swdc.platforms;

import java.nio.ByteBuffer;

/**
 * 提供手动内存管理
 */
public class Unsafe {

    /**
     * 申请内存，返回一个ByteBuffer，你必须手动通过free释放它。
     *
     * @param size 内存的大小
     * @return 申请到的内存
     */
    public static native ByteBuffer malloc(long size);

    /**
     * 重新申请内存
     * 请注意，此操作将会导致作为参数的buffer失效，请在任何情况下不要继续使用原始的Buffer，而是使用
     * 本方法返回的新buffer，另外，buffer的position会被reset。
     *
     * 原始的Buffer失效代表着此Buffer不需要进行free，请对realloc后的新的Buffer执行free。
     *
     * @param byteBuffer 重新申请的内存缓冲区
     * @param newSize 新的大小
     * @return 调整大小后的新buffer。
     */
    public static native ByteBuffer relloc(ByteBuffer byteBuffer, long newSize);

    /**
     * 内存复制，复制指定的内存，返回一个新的内容相同的内存，
     * 返回的是一个新的内存区域，你必须通过free方法释放它。
     *
     * @param source 被复制的内容
     * @return 复制得到的新内存
     */
    public static native ByteBuffer memcpy(ByteBuffer source);


    /**
     * 释放内存。
     * @param byteBuffer 释放指定的内存。
     */
    public static native void free(ByteBuffer byteBuffer);


    /**
     * SizeOf
     * @param type 类型
     * @see NTypes
     * @return
     */
    private static native int sizeOf(int type);

    public static int sizeOf(NTypes types) {
        return sizeOf(types.getType());
    }
}
