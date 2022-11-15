import java.util.function.Function;

public class Main {
    public static void main(String[] args) {
        Function<Double, Double> firstF =  (x) -> (3*Math.pow(x, 3) + 117);
        double firstA = 0;
        double firstB = 2;

        Function<Double, Double> secondF = Math::cos;
        double secondA = -(Math.PI / 2);
        double secondB = (Math.PI / 2);
    }
}
