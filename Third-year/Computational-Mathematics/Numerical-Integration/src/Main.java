import java.util.Scanner;
import java.util.function.Function;

public class Main {
    private static final int NUMBER_PARTITION_SEGMENTS = 5;
    public static void main(String[] args) {
        Function<Double, Double> f = (x) -> x * x;
        Scanner input = new Scanner(System.in);
        int[] n = new int[NUMBER_PARTITION_SEGMENTS];
        System.out.print("Введите число отрезков разбияния в количестве " + NUMBER_PARTITION_SEGMENTS + ": ");
        for (int i = 0; i < NUMBER_PARTITION_SEGMENTS; i++) n[i] = input.nextInt();

        System.out.println("Введите пределы интегрирования: ");
        double a = input.nextDouble();
        double b = input.nextDouble();

        for (int i = 0; i < NUMBER_PARTITION_SEGMENTS; i++) {
            System.out.println("n = " + n[i] + "Left rectangle " + IntegralSolver.calculateIntegralLeftRectangleMethod(f, a, b, n[i]));
            System.out.println("n = " + n[i] + "Middle rectangle " + IntegralSolver.calculateIntegralMiddleRectangleMethod(f, a, b, n[i]));
            System.out.println("--------------------------------------------------");
        }


    }
}
