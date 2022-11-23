package com.github.plugatarev.utils;

import com.github.plugatarev.SnakesProto.GameState;
import com.github.plugatarev.gamehandler.Coord;
import lombok.experimental.UtilityClass;

import java.util.List;

@UtilityClass
public final class PointUtils {
    public static List<Coord> getPointList(List<GameState.Coord> cordList) {
        return cordList.stream().map(cord -> new Coord(cord.getX(), cord.getY())).toList();
    }

    private static int countCoordinate(int coordinate, int coordinateLimit) {
        if (coordinate >= coordinateLimit) {
            return coordinate % coordinateLimit;
        }
        else if (coordinate < 0) {
            return coordinateLimit + coordinate;
        }
        return coordinate;
    }

    public static Coord getPointAbove(Coord point, int fieldHeight) {
        return new Coord(point.getX(), countCoordinate(point.getY() - 1, fieldHeight));
    }

    public static Coord getPointBelow(Coord point, int fieldHeight) {
        return new Coord(point.getX(), countCoordinate(point.getY() + 1, fieldHeight));
    }

    public static Coord getPointToRight(Coord point, int fieldWidth) {
        return new Coord(countCoordinate(point.getX() + 1, fieldWidth), point.getY());
    }

    public static Coord getPointToLeft(Coord point, int fieldWidth) {
        return new Coord(
                countCoordinate(point.getX() - 1, fieldWidth),
                point.getY()
        );
    }

    public static List<Coord> getStraightConnectedPoints(Coord point, int fieldWidth, int fieldHeight) {
        return List.of(
                PointUtils.getPointAbove(point, fieldHeight),
                PointUtils.getPointBelow(point, fieldHeight),
                PointUtils.getPointToLeft(point, fieldWidth),
                PointUtils.getPointToRight(point, fieldWidth)
        );
    }

    public static boolean arePointsStraightConnected(Coord p1, Coord p2, int fieldWidth, int fieldHeight) {
        return getStraightConnectedPoints(p1, fieldWidth, fieldHeight).contains(p2);
    }
}
