import java.util.function.Function;

public class IntegralSolver {
    public static double calculateIntegralLeftRectangleMethod(Function<Double, Double> f, double a, double b,  int n) {
        double h = (b - a) / n;
        double sum = 0;
        for (double i = 0; i < n; i++) {
            sum += f.apply(a + i * h) * h;
        }
        return sum;
    }

    public static double calculateIntegralMiddleRectangleMethod(Function<Double, Double> f, double a, double b, int n) {
        double h = (b - a) / n;
        double sum = 0;
        for (double i = 1; i <= 2 * n; i+=2) {
            sum += f.apply(a + i * h / 2) * h;
        }
        return sum;
    }

    public static double getAccuracyScoreForLeftRectangleMethod(Function<Double, Double> f, double a, double b,  int n, double exactValue) {
        double In  = calculateIntegralLeftRectangleMethod(f, a, b, n / 2);
        double I2n = calculateIntegralLeftRectangleMethod(f, a, b, n);

        return log2(1 + Math.abs((I2n - In) / (exactValue - I2n)));
    }

    public static double getAccuracyScoreForMiddleRectangleMethod(Function<Double, Double> f, double a, double b,  int n, double exactValue) {
        double In  = calculateIntegralMiddleRectangleMethod(f, a, b, n / 2);
        double I2n = calculateIntegralMiddleRectangleMethod(f, a, b, n);

        return log2(1 + Math.abs((I2n - In) / (exactValue - I2n)));
    }

    private static double log2(double x) {
        return Math.log(x) / Math.log(2);
    }
}
