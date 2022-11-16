import java.util.Scanner;
import java.util.function.Function;

public class Main {
    private static final int NUMBER_PARTITION_SEGMENTS = 5;

    public static void main(String[] args) {
        Function<Double, Double> f = (x) -> 1 / (1 + x * x);
        Scanner input = new Scanner(System.in);
        int[] n = new int[NUMBER_PARTITION_SEGMENTS];
        System.out.print("Введите число отрезков разбияния в количестве " + NUMBER_PARTITION_SEGMENTS + ": ");
        for (int i = 0; i < NUMBER_PARTITION_SEGMENTS; i++) n[i] = input.nextInt();

        System.out.print("Введите пределы интегрирования: ");
        double a = input.nextDouble();
        double b = input.nextDouble();

        for (int i = 0; i < NUMBER_PARTITION_SEGMENTS; i++) {
            System.out.println("n = " + n[i] + " Left rectangle " + IntegralSolver.calculateIntegralLeftRectangleMethod(f, a, b, n[i]));
            System.out.println("n = " + n[i] + " Middle rectangle " + IntegralSolver.calculateIntegralMiddleRectangleMethod(f, a, b, n[i]));
            System.out.println("--------------------------------------------------------------------------------");
        }

        for (int i = 0; i < NUMBER_PARTITION_SEGMENTS; i++) {
            Function<Double, Double> fun = (x) -> 3*Math.pow(x, 3) + 117;
            System.out.println("n = " + 2*n[i] +
                    " | f(x)=3*x^3+117 порядок точности для метода левых прямоугольников равен " +
                    IntegralSolver.getAccuracyScoreForLeftRectangleMethod(fun, 0, 2, 2*n[i], 246));

            System.out.println("n = " + 2*n[i] +
                    " | f(x)=3*x^3+117 порядок точности для метода средних прямоугольников равен " +
                    IntegralSolver.getAccuracyScoreForMiddleRectangleMethod(fun, 0, 2, 2*n[i], 246));

            fun = Math::cos;
            System.out.println("n = " + 2*n[i] +
                    " | f(x)=cos(x) порядок точности для метода левых прямоугольников равен " +
                    IntegralSolver.getAccuracyScoreForLeftRectangleMethod(fun, -(Math.PI / 2), (Math.PI / 2), 2*n[i], 2));

            System.out.println("n = " + 2*n[i] +
                    " | f(x)=cos(x) порядок точности для метода средних прямоугольников равен " +
                    IntegralSolver.getAccuracyScoreForMiddleRectangleMethod(fun, -(Math.PI / 2), (Math.PI / 2), 2*n[i], 2));

            System.out.println("--------------------------------------------------------------------------------");
        }
    }
}
