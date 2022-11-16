package com.github.plugatarev.utils;

import com.github.plugatarev.SnakesProto.Direction;
import lombok.experimental.UtilityClass;

@UtilityClass
public final class DirectionUtils {
    public static Direction reverse(Direction direction) {
        if (direction == null) return null;
        return switch (direction) {
            case DOWN -> Direction.UP;
            case UP -> Direction.DOWN;
            case RIGHT -> Direction.LEFT;
            case LEFT -> Direction.RIGHT;
        };
    }
}
