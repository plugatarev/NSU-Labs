package com.github.plugatarev.gamehandler;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

import java.io.Serializable;
import java.util.Objects;

@RequiredArgsConstructor
public final class Coord implements Serializable {
    @Getter private final int x;
    @Getter private final int y;

    @Override
    public boolean equals(Object object){
        if (this == object) {
            return true;
        }
        if (!(object instanceof Coord other)){
            return false;
        }
        return (x == other.x) && (y == other.y);
    }

    @Override
    public int hashCode() {
        return Objects.hash(x, y);
    }

    @Override
    public String toString() {
        return String.format("Point2D{%d, %d}", x, y);
    }
}
