package com.github.plugatarev.utils;

import com.github.plugatarev.SnakesProto.GameState;
import com.github.plugatarev.gamehandler.Point2D;
import lombok.experimental.UtilityClass;

import java.util.List;

@UtilityClass
public final class PointUtils {
    public static List<Point2D> getPointList(List<GameState.Coord> cordList) {
        return cordList.stream().map(cord -> new Point2D(cord.getX(), cord.getY())).toList();
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

    public static Point2D getPointAbove(Point2D point, int fieldHeight) {
        return new Point2D(point.getX(), countCoordinate(point.getY() - 1, fieldHeight));
    }

    public static Point2D getPointBelow(Point2D point, int fieldHeight) {
        return new Point2D(point.getX(), countCoordinate(point.getY() + 1, fieldHeight));
    }

    public static Point2D getPointToRight(Point2D point, int fieldWidth) {
        return new Point2D(countCoordinate(point.getX() + 1, fieldWidth), point.getY());
    }

    public static Point2D getPointToLeft(Point2D point, int fieldWidth) {
        return new Point2D(
                countCoordinate(point.getX() - 1, fieldWidth),
                point.getY()
        );
    }

    public static List<Point2D> getStraightConnectedPoints(Point2D point, int fieldWidth, int fieldHeight) {
        return List.of(
                PointUtils.getPointAbove(point, fieldHeight),
                PointUtils.getPointBelow(point, fieldHeight),
                PointUtils.getPointToLeft(point, fieldWidth),
                PointUtils.getPointToRight(point, fieldWidth)
        );
    }

    public static boolean arePointsStraightConnected(Point2D p1, Point2D p2, int fieldWidth, int fieldHeight) {
        return getStraightConnectedPoints(p1, fieldWidth, fieldHeight).contains(p2);
    }
}
