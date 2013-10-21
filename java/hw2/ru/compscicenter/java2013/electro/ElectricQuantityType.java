package ru.compscicenter.java2013.electro;

/**
 * Тип физической величины.
 */
public enum ElectricQuantityType {
    /**
     * Ток
     */
    CURRENT('I', "А"),

    /**
     * Сопротивление
     */
    RESISTANCE('R', "Ом"),

    /**
     * Напряжение
     */
    VOLTAGE('U', "В");


    private final char symbol;
    private final String unit;

    private ElectricQuantityType(char symbol, String unit) {
        this.symbol = symbol;
        this.unit = unit;
    }

    /**
     * @return  символ данной физической величины
     */
    public char getSymbol() {
        return symbol;
    }

    /**
     * @return  единица измерения данной физической величины
     */
    public String getUnit() {
        return unit;
    }

    /**
     *
     * @param symbol  символ физической величины
     * @return  тип физической величины, соответствующий символу,
     *      или <code>null</code>, если такой физической величины нет
     */
    public static ElectricQuantityType forSymbol(char symbol) {
        for (ElectricQuantityType type : values()) {
            if (type.getSymbol() == symbol) {
                return type;
            }
        }
        return null;
    }
}
