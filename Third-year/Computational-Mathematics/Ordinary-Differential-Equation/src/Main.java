import java.util.function.Function;

public class Main {
    private static void printResult(double[] res, double h) {
        for (int j = 0; j <= 50; j++) {
            System.out.println(j * h + " " + res[j]);
        }
    }

    public static void main(String[] args) {
        double beginSegment = 0;
        double endSegment = 5;
        double h = 0.1;
        double y0 = 1;
        Function<Double, Double> f = y -> Math.sin(Math.exp(y));

        double[] rkY = RungeKutta.calculate(f, y0, beginSegment, endSegment, h);
        double[] eulerY = Euler.calculate(f, y0, beginSegment, endSegment, h);
        double[] eulerRecY = EulerRecalculation.calculate(f, y0, beginSegment, endSegment, h);

        System.out.println("----------Runge-Kutta----------");
        printResult(rkY, h);

        System.out.println("----------Euler----------");
        printResult(eulerY, h);

        System.out.println("----------Euler-Recalculation----------");
        printResult(eulerRecY, h);

        System.out.println("----------------------Accuracy-Estimate-------------------------------");
        double[] rkY1 = RungeKutta.calculate(f, y0, beginSegment, endSegment, h);
        double[] eulerY1 = Euler.calculate(f, y0, beginSegment, endSegment, h);
        double[] eulerRecY1 = EulerRecalculation.calculate(f, y0, beginSegment, endSegment, h);

        double[] rkY2 = RungeKutta.calculate(f, y0, beginSegment, endSegment, h * 0.5);
        double[] eulerY2 = Euler.calculate(f, y0, beginSegment, endSegment, h * 0.5);
        double[] eulerRecY2 = EulerRecalculation.calculate(f, y0, beginSegment, endSegment, h * 0.5);

        double[] rkY3 = RungeKutta.calculate(f, y0, beginSegment, endSegment, h * 0.25);
        double[] eulerY3 = Euler.calculate(f, y0, beginSegment, endSegment, h * 0.25);
        double[] eulerRecY3 = EulerRecalculation.calculate(f, y0, beginSegment, endSegment, h * 0.25);
        System.out.println("----------Runge-Kutta----------");
        for (int j = 0; j <= 50; j++) {
            double delta1 = Math.abs(rkY1[j] - rkY2[j]);
            double delta2 = Math.abs(rkY2[j] - rkY3[j]);
            System.out.println(j * h + " " + (delta1 / (1 - (delta2 / delta1))));
        }

        System.out.println("----------Euler----------");
        for (int j = 0; j <= 50; j++) {
            double delta1 = Math.abs(eulerY1[j] - eulerY2[j]);
            double delta2 = Math.abs(eulerY2[j] - eulerY3[j]);
            System.out.println(j * h + " " + (delta1 / (1 - (delta2 / delta1))));
        }

        System.out.println("----------Euler-Recalculation----------");
        for (int j = 0; j <= 50; j++) {
            double delta1 = Math.abs(eulerRecY1[j] - eulerRecY2[j]);
            double delta2 = Math.abs(eulerRecY2[j] - eulerRecY3[j]);
            System.out.println(j * h + " " + (delta1 / (1 - (delta2 / delta1))));
        }
    }
}
