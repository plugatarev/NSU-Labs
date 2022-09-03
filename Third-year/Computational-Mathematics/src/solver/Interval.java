package solver;

import java.util.Optional;

public record Interval(Optional<Integer> left, Optional<Integer> right) {

    public Optional<Integer> getLeft() {
        return left;
    }

    public Optional<Integer> getRight() {
        return right;
    }

    public boolean isPoint() {
        return left.isPresent() && right.isPresent() && left.get().equals(right.get());
    }
}
