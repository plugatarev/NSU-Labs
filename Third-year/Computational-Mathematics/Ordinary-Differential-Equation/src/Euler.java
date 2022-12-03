import java.util.function.Function;

public class Euler {
    public static double[] calculate(Function<Double, Double> f, double y0, double start, double end, double h) {
        int nStep = (int)((end - start) / h);
        double[] y = new double[nStep + 1];
        y[0] = y0;
        for (int j = 1; j <= nStep; j++) {
            y[j] = y[j - 1] + h * f.apply(y[j - 1]);
        }
        return y;
    }
}
