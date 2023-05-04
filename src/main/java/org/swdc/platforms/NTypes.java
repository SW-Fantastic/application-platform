package org.swdc.platforms;

public enum NTypes {

    Int(0),
    Short(1),
    Long(2),
    Char(3),
    Pointer(4);

    private int type;
    NTypes(int type) {
        this.type = type;
    }

    public int getType() {
        return type;
    }
}
