package org.swdc.platforms;

public enum MMapType {

    READ_ONLY(0),
    WRITE_ONLY(1),
    READ_WRITE(2);
    private int type;
    MMapType(int type) {
        this.type = type;
    }

    public int getType() {
        return type;
    }
}
