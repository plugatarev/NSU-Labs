public record Interval(Double leftBorder, Double rightBorder) {
    public boolean isPoint() {
        return leftBorder != null && leftBorder.equals(rightBorder);
    }
}