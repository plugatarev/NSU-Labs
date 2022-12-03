import java.util.function.Function;

public class RungeKutta {
    public static double[] calculate(Function<Double, Double> f, double y0, double start, double end, double h) {
        int nStep = (int)((end - start) / h);
        double[] y = new double[nStep + 1];
        y[0] = y0;
        for (int j = 1; j <= nStep; j++) {
            double y1j = y[j - 1] + h * f.apply(y[j - 1]);
            double y2j = 0.25 * (3*y[j - 1] + y1j + h * f.apply(y1j));
            y[j] = (1.0/3) * (y[j - 1] + 2*y2j + 2*h*f.apply(y2j));
        }
        return y;
    }
}
