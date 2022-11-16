package com.github.plugatarev.gamehandler.game;

import lombok.Getter;
import com.github.plugatarev.gamehandler.Point2D;

import javax.validation.constraints.NotNull;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.Random;

public final class GameField {
    private final List<Cell> field;
    private final List<Cell> emptyCells;
    @Getter private final int width;
    @Getter private final int height;

    private final Random random = new Random();

    public GameField(int width, int height) {
        //TODO: validateFieldSizes(width, height);
        field = new ArrayList<>(width * height);
        emptyCells = new ArrayList<>(width * height);
        for (int row = 0; row < height; row++){
            for (int col = 0; col < width; col++){
                Cell cell = new Cell(col, row);
                field.add(cell);
                emptyCells.add(cell);
            }
        }
        this.width = width;
        this.height = height;
    }
//TODO:
//    private void validateFieldSizes(int width, int height){
//        if (width < ConfigValidator.MIN_FIELD_WIDTH || width > ConfigValidator.MAX_FIELD_WIDTH){
//            throw new IllegalArgumentException("Width not from valid interval: ["
//                    + ConfigValidator.MIN_FIELD_WIDTH + ", " + ConfigValidator.MAX_FIELD_WIDTH + "]");
//        }
//        if (height < ConfigValidator.MIN_FIELD_HEIGHT || height > ConfigValidator.MAX_FIELD_HEIGHT){
//            throw new IllegalArgumentException("Height not from valid interval: ["
//                    + ConfigValidator.MIN_FIELD_HEIGHT + ", " + ConfigValidator.MAX_FIELD_HEIGHT + "]");
//        }
//    }

    public Cell get(int row, int col){
        return new Cell(getCell(row, col));
    }

    private Cell getCell(int row, int col){
        int y = (row < 0) ? height + row : row % height;
        int x = (col < 0) ? width + col : col % width;
        return field.get(y * width + x);
    }

    public void set(int row, int col, CellType type) {
        Cell cell = getCell(row, col);
        if (type.equals(CellType.EMPTY)) {
            if (!cell.getType().equals(CellType.EMPTY)) {
                emptyCells.add(cell);
            }
        }
        else {
            emptyCells.remove(cell);
        }
        cell.setType(type);
    }

    public void set(@NotNull Point2D point, @NotNull CellType type) {
        //TODO: mb delete?
        Objects.requireNonNull(point, "Point2D cant be null");
        set(point.getY(), point.getX(), type);
    }

    public int getEmptyCellsNumber() {
        return emptyCells.size();
    }

    Optional<Cell> findCenterOfSquareWithOutSnake(int squareSize) {
        return field.stream().filter(cell -> isSquareWithoutSnake(cell, squareSize)).findFirst();
    }

    private boolean isSquareWithoutSnake(Cell squareCenter, int squareSize) {
        final int centerOffset = squareSize / 2;
        for (int yCenterOffset = -centerOffset; yCenterOffset <= centerOffset; yCenterOffset++) {
            for (int xCenterOffset = -centerOffset; xCenterOffset <= centerOffset; xCenterOffset++) {
                Cell cell = getCell(squareCenter.getY() + yCenterOffset, squareCenter.getX() + xCenterOffset);
                if (cell.getType().equals(CellType.SNAKE)) return false;
            }
        }
        return true;
    }

    public Optional<Cell> findRandomEmptyCell() {
        return emptyCells.isEmpty() ?
                Optional.empty() : Optional.of(emptyCells.get(random.nextInt(emptyCells.size() - 1)));
    }
}
