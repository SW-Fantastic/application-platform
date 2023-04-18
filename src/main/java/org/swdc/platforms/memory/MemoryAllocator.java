package org.swdc.platforms.memory;


import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class MemoryAllocator implements Closeable {

    private int blockSize;

    private List<Integer> freeBlocks;

    private RandomAccessFile pageFile;

    private File thePageFile;

    public MemoryAllocator(File pageFile,int blockCount, int blockSize) throws Exception {
        if (blockSize <= 0) {
            throw new AllocateFailedException("invalid block size");
        }
        if (pageFile.exists()) {
            pageFile.delete();
        }
        this.thePageFile = pageFile;
        this.pageFile = new RandomAccessFile(pageFile.getAbsolutePath(),"rw");
        this.pageFile.setLength((long) blockCount * blockSize);
        this.pageFile.seek(0);

        this.blockSize = blockSize;
        this.freeBlocks = new ArrayList<>();
        for (int idx = 0; idx < blockCount; idx ++) {
            freeBlocks.add(idx);
        }
    }

    public byte[] read(int[] blocks, int length) throws IOException {
        if (blockSize == 0) {
            throw new AllocateFailedException("allocator has closed");
        }
        int readed = length;
        byte[] target = new byte[readed];
        for (int idx = 0; idx < blocks.length; idx ++) {
            int count = Math.min(readed,blockSize);
            pageFile.seek((long) blocks[idx] * blockSize);
            pageFile.read(target,idx * blockSize,count);
            readed = readed - count;
        }
        return target;
    }

    public void free(int[] blocks) {
        if (blockSize == 0) {
            throw new AllocateFailedException("allocator has closed");
        }
        for (int block: blocks) {
            freeBlocks.add(block);
        }
    }

    public void write(int[] blocks, byte[] data) throws IOException {
        if (blockSize == 0) {
            throw new AllocateFailedException("allocator has closed");
        }
        int counts = data.length / blockSize;
        if(data.length % blockSize > 0) {
            counts = counts + 1;
        }
        if (counts > blocks.length) {
            throw new AllocateFailedException("out of memory");
        }
        int written = data.length;
        for (int idx = 0; idx < counts; idx ++) {
            int bytesCount = Math.min(written, blockSize);
            this.pageFile.seek((long) blocks[idx] * blockSize);
            this.pageFile.write(data,idx * blockSize , Math.min(written, blockSize));
            written = written - bytesCount;
        }
    }

    public MemoryReference allocate(int memorySize) {
        if (blockSize == 0) {
            throw new AllocateFailedException("allocator has closed");
        }

        int count = memorySize / blockSize;
        if(memorySize % blockSize > 0) {
            count = count + 1;
        }
        if (count > freeBlocks.size()) {
            throw new AllocateFailedException("Out of memory");
        }
        int[] blocks = new int[count];
        for (int idx = 0; idx < count; idx ++) {
            blocks[idx] = freeBlocks.remove(idx);
        }
        return new MemoryReference(this,blocks,memorySize);
    }

    @Override
    public void close() throws IOException {
        if (blockSize == 0) {
            throw new AllocateFailedException("Allocator has closed.");
        }
        this.pageFile.close();
        this.blockSize = 0;
        this.freeBlocks.clear();
        this.thePageFile.delete();
    }
}
