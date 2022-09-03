package solver;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Solver {
    private static final int DELTA = 1;

    private static int findRoot(Interval interval) {
        if (interval.isPoint()) return interval.left().get();

    }

    private static List<Interval> localizeRoots() {

    }

    private static List<Integer> solve(int a, int b, int c, double eps) {
        List<Integer> roots = new ArrayList<>();
        List<Interval> intervals = localizeRoots();

        for (Interval interval : intervals) {
            roots.add(findRoot(interval));
        }
        return roots;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        List<Integer> roots = solve(in.nextInt(), in.nextInt(), in.nextInt(), in.nextDouble());
        System.out.printf("Уравнение имеет %d корней: ", roots.size());
        for (int root : roots) {
            System.out.println(root + " ");
        }
    }
}
