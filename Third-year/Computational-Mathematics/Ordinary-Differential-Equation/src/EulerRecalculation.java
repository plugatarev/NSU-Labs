import java.util.function.Function;

public class EulerRecalculation {
    public static double[] calculate(Function<Double, Double> f, double y0, double start, double end, double h) {
        int nStep = (int)((end - start) / h);
        double[] y = new double[nStep + 1];
        y[0] = y0;
        for (int j = 1; j <= nStep; j++) {
            double midY = y[j - 1] + h/2 * f.apply(y[j - 1]);
            y[j] = y[j - 1] + h * f.apply(midY);
        }
        return y;
    }
}
