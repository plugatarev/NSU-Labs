import java.util.Arrays;

public class ImplicitSchema {
    private static double[] initAlpha(double r, int numH) {
        double[] alpha = new double[numH + 1];
        alpha[0] = 0;
        for (int j = 1; j <= numH; j++) {
            alpha[j] = - ((r/2) / (1 - (r/2)*alpha[j - 1]));
        }
        return alpha;
    }

    private static double getBeta(double r, double prevBeta, double ksi, double prevAlpha) {
        return ((ksi + (r/2) * prevBeta) / (1 - (r/2) * prevAlpha));
    }

    private static double[] initBeta(double[][] u, int numH, double h, double tau, int i, double r, double[] alpha) {
        double[] beta = new double[numH + 1];
        beta[0] = getU(-tau * (i + 1));
        for (int j = 1; j <= numH; j++) {
            beta[j] = getBeta(r, beta[j - 1], u[i][j], alpha[j - 1]);
        }
        return beta;
    }

    public static void calculate(double start, double end, double h, double[] r, int[] T) {
        int maxT = Arrays.stream(T).max().getAsInt();
        int numH = (int)((end - start) / h);
        double[][] u = new double[(int)(maxT / (r[0] * h)) + 1][numH + 1];

        System.out.println("T = 0");
        for (int i = 0; i <= numH; i++) {
            u[0][i] = getU(i * h);
            System.out.println(i * h + " " + u[0][i]);
        }

        double[][] res = new double[2][numH + 1];
        double[][] exactSolve = new double[2][numH + 1];
        for (double koef : r) {
            double tau = koef * h;
            double[] alpha = initAlpha(koef, numH);
            int numT = (int)(maxT / tau);
            for (int i = 0; i < numT; i++) {
                double[] beta = initBeta(u, numH, h, tau, i, koef, alpha);
                for (int j = numH; j >= 0; j--) {
                    u[i + 1][j] = j == numH ? alpha[j] * getU((j + 1) * h - (i + 1) * tau) + beta[j]
                                            : alpha[j] * u[i + 1][j + 1] + beta[j];
                    if (Math.abs(i * tau - 1.0) < 0.000001) {
                        exactSolve[0][j] = getU(j * h - 1);
                        res[0][j] = u[i][j];
                    }
                    if (Math.abs((i + 1) * tau - 5.0) < 0.000001) {
                        exactSolve[1][j] = getU(j * h - 5);
                        res[1][j] = u[i + 1][j];
                    }
                }
            }
            print(koef, h, numH, res, exactSolve);
        }
    }

    private static void print(double koef, double h, int numH, double[][] res, double[][] exactSolve) {
        System.out.println("--------------------------------------------------------");
        System.out.println("r = " + koef);
        System.out.println("T = 1");
        System.out.println("Exact solve");
        for (int j = 0; j <= numH; j++) {
            System.out.println(j * h + " " + exactSolve[0][j]);
        }
        System.out.println("--------------------------------------------------------");
        System.out.println("My solve");
        for (int j = 0; j <= numH; j++) {
            System.out.println(j * h + " " + res[0][j]);
        }
        System.out.println("--------------------------------------------------------");
        System.out.println("T = 5");
        System.out.println("Exact solve");
        for (int j = 0; j <= numH; j++) {
            System.out.println(j * h + " " + exactSolve[1][j]);
        }
        System.out.println("--------------------------------------------------------");
        System.out.println("My solve");
        for (int j = 0; j <= numH; j++) {
            System.out.println(j * h + " " + res[1][j]);
        }
    }

    private static double getU(double x) {
        return x <= 3 ? 5 : 1;
    }
}
