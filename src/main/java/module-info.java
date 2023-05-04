module swdc.platform {

    requires org.dom4j;
    requires java.xml;

    exports org.swdc.platforms;
    exports org.swdc.platforms.memory;

    opens libplatformnatives;
}