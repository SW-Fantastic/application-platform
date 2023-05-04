package org.swdc.platform.test;

import org.swdc.platforms.*;
import org.swdc.platforms.memory.MemoryAllocator;
import org.swdc.platforms.memory.MemoryConstants;
import org.swdc.platforms.memory.MemoryReference;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.List;

public class WindowsBinaryUtilTests {

    public static void main(String[] args) throws Exception {
       // System.load(new File("platforms/Platform-x64.dll").getAbsolutePath());
        /*File target = new File("platforms/screen-capture-recorder-x64.dll");
        boolean isSuccess = WindowsBinaryUtils.dllRegister(target);
        System.err.println("com is register " + (isSuccess ? "success" : "failed"));
        isSuccess = WindowsBinaryUtils.dllUnRegister(target);
        System.err.println("com is un-register " + (isSuccess ? "success" : "failed"))*/

        NativePlatform.initializePlatform(new File("./platforms"));

        ByteBuffer byteBuffer = Unsafe.malloc(1024L * 1024 );
        ByteBuffer newBuffer = Unsafe.malloc(1024L * 1024  * 4);

        boolean copied = Unsafe.memcpy(newBuffer, 1024*8, byteBuffer, 0, 1024);
        System.err.println(copied);

        Unsafe.free(byteBuffer);
        Unsafe.free(newBuffer);

        MemoryAllocator allocator = new MemoryAllocator(
                new File("platforms/pagefile.memory"),
                500,
                MemoryConstants.Mb
        );
        File readMe = new File("platforms/README.md");
        FileInputStream fin = new FileInputStream(readMe);
        byte[] data = fin.readAllBytes();

        MemoryReference ref = allocator.allocate(data.length);
        ref.write(data);
        ref.inactive();
        byte[] content = ref.read();

        String ctx = new String(data, StandardCharsets.UTF_8);
        String ctxRead = new String(content,StandardCharsets.UTF_8);

        System.err.println(ctx.equals(ctxRead));

        ref.free();
        allocator.close();

        System.err.println(Unsafe.sizeOf(NTypes.Long));

        RandomAccessFile rd = new RandomAccessFile("Test.data", "rw");
        rd.setLength(1024 * 1024 * 16);
        rd.close();

        File test = new File("Test.data");
        ByteBuffer mapped = Unsafe.memoryMap(test.getAbsolutePath(), MMapType.READ_WRITE, 0, 1024);
        ByteBuffer mapped2 = Unsafe.memoryMap(test.getAbsolutePath(), MMapType.READ_WRITE, 5000, 1024);
        System.err.println(mapped != null);
        System.err.println(mapped2 != null);
        System.err.println(Unsafe.memorySyncFile(mapped2));

        System.err.println("allocation alignment : " + Unsafe.getMemoryAlignment());
        byte[] hBuf = new byte[1024];
        mapped.get(hBuf);
        boolean unlinked = Unsafe.freeMemoryMap(mapped);
        System.err.println(unlinked);
    }

}
