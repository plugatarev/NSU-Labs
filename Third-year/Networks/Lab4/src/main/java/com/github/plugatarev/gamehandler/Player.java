package com.github.plugatarev.gamehandler;

import com.github.plugatarev.SnakesProto.NodeRole;
import com.github.plugatarev.datatransfer.NetNode;
import lombok.Getter;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

import java.io.Serializable;
import java.util.Objects;

@RequiredArgsConstructor
public final class Player implements Serializable {
    @Getter private final String name;
    @Getter private final int id;
    @Getter private final NetNode netNode;
    @Getter @Setter private NodeRole role = NodeRole.NORMAL;
    @Getter @Setter private int score = 0;

    public void incrementScore() {
        this.score++;
    }

    @Override
    public String toString() {
        return this.name;
    }

    @Override
    public boolean equals(Object object) {
        if (this == object) {
            return true;
        }
        if (!(object instanceof Player other)) {
            return false;
        }
        return name.equals(other.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name);
    }
}