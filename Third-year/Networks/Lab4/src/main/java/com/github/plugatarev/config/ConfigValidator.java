//TODO:
//package com.github.plugatarev.config;
//
//import com.github.plugatarev.SnakesProto;
//import lombok.experimental.UtilityClass;
//
//@UtilityClass
//public final class ConfigValidator {
//    public static final int MIN_FIELD_WIDTH = 10;
//    public static final int MAX_FIELD_WIDTH = 100;
//
//    public static final int MIN_FIELD_HEIGHT = 10;
//    public static final int MAX_FIELD_HEIGHT = 100;
//
//    private static final int MIN_FOOD_STATIC = 0;
//    private static final int MAX_FOOD_STATIC = 100;
//
//    private static final int MIN_STATE_DELAY_MS = 1;
//    private static final int MAX_STATE_DELAY_MS = 10000;
//
//    public static void validate(SnakesProto.GameConfig config) {
//        validateIntField(config.getWidth(), MIN_FIELD_WIDTH, MAX_FIELD_WIDTH, ConfigFieldNames.FIELD_WIDTH);
//        validateIntField(config.getHeight(), MIN_FIELD_HEIGHT, MAX_FIELD_HEIGHT, ConfigFieldNames.FIELD_HEIGHT);
//        validateIntField(config.getFoodStatic(), MIN_FOOD_STATIC, MAX_FOOD_STATIC, ConfigFieldNames.FOOD_STATIC);
//        validateIntField(config.getStateDelayMs(), MIN_STATE_DELAY_MS, MAX_STATE_DELAY_MS, ConfigFieldNames.STATE_DELAY_MS);
//    }
//
//    private static void validateIntField(int fieldValue, int minValue, int maxValue, String fieldName) {
//        if (fieldValue < minValue || fieldValue > maxValue) {
//            throw new IllegalStateException(String.format("Config field %s=%d is not in range [%d, %d]", fieldName, fieldValue, minValue, maxValue));
//        }
//    }
//}
