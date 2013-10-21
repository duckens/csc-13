package ru.compscicenter.java2013.electro;

/**
 * Значение физической величины.
 */
public final class ElectricQuantity {

    private final ElectricQuantityType type;
    private final double value;

    public ElectricQuantity(ElectricQuantityType type, double value) {
        this.type = type;
        this.value = value;
    }

    public ElectricQuantityType getType() {
        return type;
    }

    public double getValue() {
        return value;
    }

    @Override
    public String toString() {
        return type.getSymbol() + " = " + value;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ElectricQuantity) {
            ElectricQuantity that = (ElectricQuantity) obj;
            return type == that.type && value == that.value;
        }
        return false;
    }


    public static ElectricQuantity current(double value) {
        return new ElectricQuantity(ElectricQuantityType.CURRENT, value);
    }

    public static ElectricQuantity resistance(double value) {
        return new ElectricQuantity(ElectricQuantityType.RESISTANCE, value);
    }

    public static ElectricQuantity voltage(double value) {
        return new ElectricQuantity(ElectricQuantityType.VOLTAGE, value);
    }
}
