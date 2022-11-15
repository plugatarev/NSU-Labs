import java.util.function.Function;

public class IntegralSolver {
    public static double calculateIntegralLeftRectangleMethod(Function<Double, Double> f, double a, double b,  int n) {
        double h = (b - a) / n;
        double sum = 0;
        for (double i = a; i < b; i += h) {
            sum += f.apply(i) * h;
        }
        return sum;
    }

    public static double calculateIntegralMiddleRectangleMethod(Function<Double, Double> f, double a, double b, int n) {
        double h = (b - a) / n;
        double sum = 0;
        for (double i = a; i < b; i += h) {
            sum += f.apply(i + h/2) * h;
        }
        return sum;
    }
}
