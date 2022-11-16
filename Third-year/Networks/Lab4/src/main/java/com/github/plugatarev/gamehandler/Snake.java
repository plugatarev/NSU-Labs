package com.github.plugatarev.gamehandler;

import com.github.plugatarev.SnakesProto;
import com.github.plugatarev.SnakesProto.Direction;
import lombok.Getter;
import lombok.Setter;
import com.github.plugatarev.utils.DirectionUtils;
import com.github.plugatarev.utils.PointUtils;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public final class Snake implements Serializable {
    private static final int EMPTY = -1;

    private final int fieldWidth;
    private final int fieldHeight;

    @Getter private Point2D head;
    @Getter private Point2D tail;

    @Getter @Setter private int playerID = EMPTY;
    @Getter @Setter private SnakesProto.GameState.Snake.SnakeState state = SnakesProto.GameState.Snake.SnakeState.ALIVE;
    @Getter private Direction direction;

    @Getter private final List<Point2D> points;

    public Snake(Point2D head, Point2D tail, int fieldWidth, int fieldHeight) {
        this.fieldWidth = fieldWidth;
        this.fieldHeight = fieldHeight;
        this.head = head;
        this.tail = tail;
        validateInitHeadAndTail(head, tail);
        this.points = new ArrayList<>();
        this.points.add(head);
        this.points.add(tail);

        this.direction = calculateCurrentDirection(head, tail);
    }

    public Snake(int playerID, List<Point2D> points, SnakesProto.GameState.Snake.SnakeState state, Direction direction, int fieldWidth, int fieldHeight) {
        this.fieldWidth = fieldWidth;
        this.fieldHeight = fieldHeight;
        this.playerID = playerID;
        this.points = new ArrayList<>(points);
        this.state = state;
        this.direction = direction;
        this.head = this.points.get(0);
        this.tail = this.points.get(this.points.size() - 1);
    }

    private void validateInitHeadAndTail(Point2D head, Point2D tail) {
        if (!PointUtils.arePointsStraightConnected(head, tail, fieldWidth, fieldHeight)) {
            throw new IllegalArgumentException("Head and tail are not connected");
        }
    }

    private Direction calculateCurrentDirection(Point2D head, Point2D tail) {
        validateInitHeadAndTail(head, tail);
        if (PointUtils.getPointToRight(head, fieldWidth).equals(tail)) {
            return Direction.LEFT;
        }
        else if (PointUtils.getPointToLeft(head, fieldWidth).equals(tail)) {
            return Direction.RIGHT;
        }
        else if (PointUtils.getPointBelow(head, fieldHeight).equals(tail)) {
            return Direction.UP;
        }
        else if (PointUtils.getPointAbove(head, fieldHeight).equals(tail)) {
            return Direction.DOWN;
        }
        throw new IllegalStateException("Cant calculate current direction");
    }

    public void makeMove(Direction direction) {
        if (direction == null) {
            makeMove();
            return;
        }

        if (DirectionUtils.reverse(direction).equals(this.direction)) {
            direction = this.direction;
        }

        this.direction = direction;
        head = getNewHead(direction);
        points.add(0, head);
    }

    private Point2D getNewHead(Direction direction) {
        return switch (direction) {
            case DOWN -> PointUtils.getPointBelow(head, fieldHeight);
            case UP -> PointUtils.getPointAbove(head, fieldHeight);
            case LEFT -> PointUtils.getPointToLeft(head, fieldWidth);
            case RIGHT -> PointUtils.getPointToRight(head, fieldWidth);
        };
    }

    public void makeMove() {
        makeMove(direction);
    }

    public void removeTail() {
        points.remove(tail);
        if (points.size() <= 1) {
            throw new IllegalStateException("Snake cant have less than 2 points");
        }
        tail = points.get(points.size() - 1);
    }

    public boolean isSnakeBody(Point2D point) {
        for (int i = 1; i < points.size() - 1; i++) {
            if (point.equals(points.get(i))) {
                return true;
            }
        }
        return false;
    }

    public boolean isSnake(Point2D point) {
        return point.equals(head) || point.equals(tail) || isSnakeBody(point);
    }

    public boolean isSnakeHead(Point2D point) {
        return point.equals(head);
    }

    @Override
    public boolean equals(Object object) {
        if (this == object) {
            return true;
        }
        if (!(object instanceof Snake other)) {
            return false;
        }
        return points.equals(other.points);
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.points);
    }
}
