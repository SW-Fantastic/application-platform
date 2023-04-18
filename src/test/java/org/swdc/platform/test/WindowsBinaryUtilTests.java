package org.swdc.platform.test;

import org.swdc.platforms.Unsafe;
import org.swdc.platforms.WindowsBinaryUtils;
import org.swdc.platforms.memory.MemoryAllocator;
import org.swdc.platforms.memory.MemoryConstants;
import org.swdc.platforms.memory.MemoryReference;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.List;

public class WindowsBinaryUtilTests {

    public static void main(String[] args) throws Exception {
        System.load(new File("platforms/Platform.dll").getAbsolutePath());
        /*File target = new File("platforms/screen-capture-recorder-x64.dll");
        boolean isSuccess = WindowsBinaryUtils.dllRegister(target);
        System.err.println("com is register " + (isSuccess ? "success" : "failed"));
        isSuccess = WindowsBinaryUtils.dllUnRegister(target);
        System.err.println("com is un-register " + (isSuccess ? "success" : "failed"));*/

        /*ByteBuffer byteBuffer = Unsafe.malloc(1024L * 1024 * 1024 );
        Unsafe.free(byteBuffer);

        ByteBuffer newBuffer = Unsafe.malloc(1024L * 1024 * 1024 * 4);
        Unsafe.free(newBuffer);*/

        MemoryAllocator allocator = new MemoryAllocator(
                new File("platforms/pagefile.memory"),
                500,
                MemoryConstants.Mb
        );
        File readMe = new File("README.md");
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
    }

}
