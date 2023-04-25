module swdc.platform {

    requires dom4j;

    exports org.swdc.platforms;
    exports org.swdc.platforms.memory;

    opens libplatformnatives;
}