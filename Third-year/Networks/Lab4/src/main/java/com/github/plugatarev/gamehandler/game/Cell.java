package com.github.plugatarev.gamehandler.game;

import lombok.Getter;
import lombok.Setter;
import com.github.plugatarev.gamehandler.Point2D;

import java.util.Objects;

public final class Cell {
    @Getter private final Point2D point;
    @Getter @Setter private CellType type;

    public Cell(int x, int y, CellType type) {
        this.point = new Point2D(x, y);
        this.type = type;
    }

    public Cell(Point2D point, CellType type) {
        this.point = point;
        this.type = type;
    }

    public Cell(int x, int y) {
        this(x, y, CellType.EMPTY);
    }

    public Cell(Cell cell) {
        this(cell.point, cell.getType());
    }

    public int getX() {
        return point.getX();
    }

    public int getY() {
        return point.getY();
    }

    @Override
    public boolean equals(Object object) {
        if (this == object) {
            return true;
        }
        if (!(object instanceof Cell other)) {
            return false;
        }
        return point.equals(other.point) && (type == other.type);
    }

    @Override
    public int hashCode() {
        return Objects.hash(point, type);
    }
}
