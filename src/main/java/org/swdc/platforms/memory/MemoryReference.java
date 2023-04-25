package org.swdc.platforms.memory;


import org.swdc.platforms.Unsafe;

import java.nio.ByteBuffer;

public class MemoryReference {

    private final int[] blcoks;

    private final MemoryAllocator allocator;

    private final int length;

    private ByteBuffer data;

    MemoryReference(MemoryAllocator allocator, int[] blocks,int length) {
        this.blcoks = blocks;
        this.allocator = allocator;
        this.length = length;
        this.data = Unsafe.malloc(length);
    }

    public Boolean inactive() {
        try {
            allocator.write(blcoks,read());
            Unsafe.free(data);
            data = null;
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public byte[] read() {
        if (data == null) {
            try {
                byte[] data = allocator.read(blcoks,length);
                ByteBuffer buf = Unsafe.malloc(length);
                buf.put(data);
                this.data = buf;
                return data;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        byte[] loaded = new byte[length];
        data.position(0);
        data.get(loaded,0,length);
        return loaded;
    }

    public Boolean write(byte[] data) {
        try {
            if (this.data != null) {
                this.data.position(0);
                this.data.put(data);
            } else {
                allocator.write(blcoks,data);
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public ByteBuffer getRef() {
        return data;
    }

    public void free() {
        if (data != null) {
            Unsafe.free(data);
        }
        allocator.free(blcoks);
    }

}
