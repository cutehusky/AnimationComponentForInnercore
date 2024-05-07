package cutehusky.AnimationComponent;

import com.zhekasmirnov.innercore.mod.build.ModLoader;

import java.io.File;

public class PackLoader {
    public static void addMinecraftResourcePack(String dirName) {
        File file = new File(dirName);
        ModLoader.addMinecraftResourcePack(file);
        ModLoader.instance.loadResourceAndBehaviorPacks();
    }

    public static void addMinecraftBehaviorPack(String dirName) {
        File file = new File(dirName);
        ModLoader.addMinecraftBehaviorPack(file);
        ModLoader.instance.loadResourceAndBehaviorPacks();
    }
}
