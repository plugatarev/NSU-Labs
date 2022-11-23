package com.github.plugatarev.gamehandler.game;

import lombok.Getter;
import lombok.Setter;
import com.github.plugatarev.gamehandler.Coord;

import java.util.Objects;

public final class Cell {
    @Getter private final Coord point;
    @Getter @Setter private CellType type;

    public Cell(int x, int y, CellType type) {
        this.point = new Coord(x, y);
        this.type = type;
    }

    public Cell(Coord point, CellType type) {
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
