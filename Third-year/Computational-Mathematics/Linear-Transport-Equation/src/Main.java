public class Main {

    public static void main(String[] args) {
        double beginSegment = 0;
        double endSegment = 10;
        double h = 0.1;
        double[] r = new double[]{0.25, 0.5, 1, 1.25};
        int[] T = new int[]{1, 5};

        OrderUpwind.calculate(beginSegment, endSegment, h, r, T);
        ImplicitSchema.calculate(beginSegment, endSegment, h, r, T);
    }
}
