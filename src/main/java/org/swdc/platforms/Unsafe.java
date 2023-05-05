package org.swdc.platforms;

import java.io.File;
import java.nio.ByteBuffer;
import java.util.ArrayList;

/**
 * 提供手动内存管理
 */
public class Unsafe {

    private static final ArrayList<ByteBuffer> mapped = new ArrayList<>();

    /**
     * 申请内存，返回一个ByteBuffer，你必须手动通过free释放它。
     *
     * @param size 内存的大小
     * @return 申请到的内存
     */
    public static native ByteBuffer malloc(long size);

    /**
     * 重新申请内存
     * 请注意，此操作将会导致作为本来的buffer失效，请在任何情况下不要继续使用原始的Buffer，而是使用
     * 本方法返回的新buffer，另外，buffer的position会被reset。
     *
     * 原始的Buffer失效代表着此Buffer不需要进行free，请对realloc后的新的Buffer执行free。
     *
     * 你不应该对通过memoryMap得到的Buffer进行此操作。
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
     * 复制指定的内存位置。
     * 返回的是新申请的内存，你必须通过free方法释放它。
     *
     * @param source 数据来源
     * @param offset 开始复制的位置
     * @param length 需要复制的长度
     * @return 复制得到的新内存
     */
    public static native ByteBuffer memcpy(ByteBuffer source, int offset, int length);

    /**
     * 在两块内存区域之间复制数据
     *
     * @param dest 目标内存
     * @param destOffset 模板内存的起始位置
     * @param source 源内存
     * @param offset 源内存的起始位置
     * @param length 复制数据的长度
     * @return 操作是否成功
     */
    public static native boolean memcpy(ByteBuffer dest, int destOffset, ByteBuffer source, int offset, int length);

    /**
     * mmap文件内存映射操作。
     *
     * 你必须需要通过munmap释放映射的内存。
     *
     * @param filePath 要映射的文件路径（绝对路径）
     * @param type 映射类型
     * @param offset 从多少字节开始
     * @param length 映射的长度
     * @return 映射的ByteBuffer。
     */
    private static native ByteBuffer mmap(String filePath, int type, int offset, long length);

    /**
     * mmap文件内存映射操作。
     * 你必须需要通过freeMemoryMap释放映射的内存,
     * 你不能通过free方法释放它。
     *
     * @param filePath 要映射的文件路径（绝对路径）
     * @param type 映射类型
     * @param offset 从多少字节开始
     * @param length 映射的长度
     * @return 映射的ByteBuffer。
     */
    public static ByteBuffer memoryMap(String filePath, MMapType type, int offset, long length) {
        File file = new File(filePath);
        if (!file.exists() || file.length() < offset + length) {
            return null;
        }
        ByteBuffer result = mmap(file.getAbsolutePath(), type.getType(),offset , length);
        if (result != null) {
            mapped.add(result);
            return result;
        }
        return null;
    }

    /**
     * 将内存映射的区域刷新到文件中。
     *
     * @param byteBuffer
     * @return
     */
    private static native boolean msync(ByteBuffer byteBuffer);

    /**
     * 将内存映射的区域刷新到文件中。
     *
     * @param byteBuffer 文件映射的内存区域
     * @return 是否执行成功
     */
    public static boolean memorySyncFile(ByteBuffer byteBuffer) {
        if (Unsafe.mapped.contains(byteBuffer)) {
            return msync(byteBuffer);
        }
        return false;
    }

    /**
     * 解除文件内存映射。
     *
     * @param byteBuffer 文件-内存映射返回的ByteBuffer。
     * @return 资源释放是否成功
     */
    private static native boolean munmap(ByteBuffer byteBuffer);

    /**
     * 解除文件内存映射。
     *
     * @param byteBuffer 文件-内存映射返回的ByteBuffer。
     * @return 资源释放是否成功
     */
    public static boolean freeMemoryMap(ByteBuffer byteBuffer) {
        if (Unsafe.mapped.contains(byteBuffer)) {
            mapped.remove(byteBuffer);
            return munmap(byteBuffer);
        }
        return false;
    }

    /**
     * 释放内存。
     * 你不应该对memoryMap得到的ByteBuffer执行此操作。
     *
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

    /**
     * 获取系统的内存分配粒度。
     * 内存-文件映射将会以一定的方式对齐到这个数值。
     *
     * 文件-内存映射中文件的Offset将会对齐到此值，如果提供的Offset未对齐，
     * 将会自动对齐到此值，并且拓展它的Length。
     *
     * @return
     */
    public static native long getMemoryAlignment();

}
