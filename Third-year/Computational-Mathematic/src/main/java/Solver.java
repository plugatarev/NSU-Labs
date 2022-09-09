import java.util.*;

public record Solver(double a, double b, double c, double eps) {
    private static final int DELTA = 1;
    private static final double REAL_ZERO = 0.0;
    private static final Double INFINITY = null;

    public static void solveEquation(int a, int b, int c, double eps) {
        Solver solver = new Solver(a, b, c, eps);
        solver.solve();
    }

    private void solve() {
        List<Interval> intervals = localizeRoots();
        for (Interval interval : intervals) {
            double root = findRoot(interval);
            System.out.printf("Корень уравнения х=%f; f(%f)=%f\n", root, root, getValueFunction(root));
        }
    }

    private double findRoot(Interval interval) {
        if (interval.isPoint()) return interval.leftBorder();

        if (interval.rightBorder() == null || interval.leftBorder() == null) {
            return getRootFromLimitInterval(interval);
        }

        double leftBorder = interval.leftBorder();
        double rightBorder = interval.rightBorder();
        double middle = (leftBorder + rightBorder) / 2;

        if (getValueFunction(leftBorder) <= -eps && getValueFunction(rightBorder) >= eps) {
            while (Math.abs(getValueFunction(middle)) >= eps) {
                if (getValueFunction(middle) <= -eps) {
                    leftBorder = middle;
                } else {
                    rightBorder = middle;
                }
                middle = (leftBorder + rightBorder) / 2;
            }
        }
        else if (getValueFunction(leftBorder) >= eps && getValueFunction(rightBorder) <= -eps) {
            while (Math.abs(getValueFunction(middle)) >= eps) {
                double a = getValueFunction(middle);
                if (a <= -eps) {
                    rightBorder = middle;
                } else {
                    leftBorder = middle;
                }
                middle = (leftBorder + rightBorder) / 2;
            }
        }
        else throw new IllegalStateException("Incorrect case");
        return middle;
    }

    private double getRootFromLimitInterval(Interval interval) {
        double step;
        double leftBorder;
        double rightBorder;
        if (interval.leftBorder() == null) {
            rightBorder = interval.rightBorder();
            leftBorder = rightBorder - DELTA;
            step = -DELTA;
        }
        else {
            leftBorder = interval.leftBorder();
            rightBorder = leftBorder + DELTA;
            step = DELTA;
        }

        while ((getValueFunction(leftBorder) <= -eps && getValueFunction(rightBorder) <= -eps) ||
               (getValueFunction(leftBorder) >=  eps && getValueFunction(rightBorder) >= eps)) {
            leftBorder += step;
            rightBorder += step;
        }
        return findRoot(new Interval(leftBorder, rightBorder));
    }

    private List<Interval> localizeRoots() {
        List<Interval> intervals = new ArrayList<>();
        double derivativeDiscriminant = calculateDerivativeDiscriminant();
        if (derivativeDiscriminant <= -eps) {
            if (getValueFunction(REAL_ZERO) < -eps) {
                intervals.add(new Interval(REAL_ZERO, null));
            } else if (getValueFunction(REAL_ZERO) > eps) {
                intervals.add(new Interval(INFINITY, REAL_ZERO));
            } else intervals.add(new Interval(REAL_ZERO, REAL_ZERO));
        }
        else if (derivativeDiscriminant < eps) {
            double extreme = (-2 * a) / 6;
            if (getValueFunction(extreme) >= eps) {
                intervals.add(new Interval(INFINITY, extreme));
            }
            else if (getValueFunction(extreme) <= -eps) {
                intervals.add(new Interval(extreme, INFINITY));
            }
            else intervals.add(new Interval(extreme, extreme));
        }
        else {
            double alpha = (-2 * a - Math.sqrt(calculateDerivativeDiscriminant())) / 6;
            double beta = (-2 * a + Math.sqrt(calculateDerivativeDiscriminant())) / 6;
            if (alpha > beta) {
                double temp = beta;
                beta = alpha;
                alpha = temp;
            }

            double fAlpha = getValueFunction(alpha);
            double fBeta = getValueFunction(beta);

            if (fAlpha >= eps && fBeta <= -eps) {
                intervals.add(new Interval(beta, INFINITY));
                intervals.add(new Interval(INFINITY, alpha));
                intervals.add(new Interval(alpha, beta));
            }
            else if (Math.abs(fAlpha) < eps && fBeta <= -eps) {
                intervals.add(new Interval(alpha, alpha));
                intervals.add(new Interval(beta, INFINITY));
            }
            else if (fAlpha >= eps && Math.abs(fBeta) < eps) {
                intervals.add(new Interval(beta, beta));
                intervals.add(new Interval(INFINITY, alpha));
            }
            else if (fAlpha <= -eps && fBeta <= -eps) {
                intervals.add(new Interval(beta, INFINITY));
            }
            else if (fAlpha >= eps && fBeta >= eps) {
                intervals.add(new Interval(INFINITY, alpha));
            }
            else throw new IllegalStateException("Incorrect case");
        }
        return intervals;
    }

    private double calculateDerivativeDiscriminant() {
        return 4 * Math.pow(a, 2) - 12 * b;
    }

    private double getValueFunction(double x) {
        return Math.pow(x, 3) + a * Math.pow(x, 2) + b * x + c;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        solveEquation(in.nextInt(), in.nextInt(), in.nextInt(), in.nextDouble());
    }
}