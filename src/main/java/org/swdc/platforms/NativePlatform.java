package org.swdc.platforms;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.List;

public class NativePlatform {

    private static List<String> arch64 = Arrays.asList(
            "amd64","x64","x86_64"
    );

    public static void initializePlatform(File platformFolder) {
        if (!platformFolder.exists()) {
            if(!platformFolder.mkdirs()) {
                throw new RuntimeException("can not create native library folder: " + platformFolder.getAbsolutePath());
            }
        }
        String osArch = System.getProperty("os.arch");
        if (arch64.contains(osArch.toLowerCase())) {
            osArch = "x64";
        }

        File libFile = null;
        String osName = System.getProperty("os.name").trim().toLowerCase();

        String resourceName = null;
        if (osName.contains("windows")) {
            resourceName = "/libplatformnatives/" + "Platform-" + osArch + ".dll";
            libFile = new File(platformFolder.getAbsolutePath() + File.separator + "Platform.dll");
        } else if (osName.contains("mac")) {
            resourceName = "/libplatformnatives/" + "libPlatform-" + osArch + ".dylib";
            String url = NativePlatform.class.getProtectionDomain().getCodeSource().getLocation().getFile();
            String base = URLDecoder.decode(url, StandardCharsets.UTF_8);
            if (base.indexOf(".app") > 0) {
                // 位于MacOS的Bundle（.app软件包）内部，特殊处理以获取正确的路径。
                String location = base.substring(0,base.indexOf(".app")) + ".app/Contents/";
                Path target = new File(location).toPath();
                libFile = target.resolve(platformFolder.getName() + File.separator + "libPlatform.dylib").toFile();
            } else {
                libFile = new File(platformFolder.getAbsolutePath() + File.separator + "libPlatform.dylib");
            }
        } else if (osName.contains("linux")) {
            resourceName = "/libplatformnatives/" + "libPlatform-" + osArch + ".so";
            libFile = new File(platformFolder.getAbsolutePath() + File.separator + "libPlatform.so");
        }
        if (libFile == null) {
            throw new RuntimeException("this library does not support your system.");
        } else {
            if (!libFile.exists()) {
                try {
                    InputStream in = NativePlatform.class.getModule()
                            .getResourceAsStream(resourceName);
                    FileOutputStream fos = new FileOutputStream(libFile);
                    in.transferTo(fos);
                    in.close();
                    fos.close();
                    System.load(libFile.getAbsolutePath());
                } catch (Exception e) {
                    throw new RuntimeException("failed to extract native library.",e);
                }
            } else {
                System.load(libFile.getAbsolutePath());
            }
        }
    }

}
