import java.util.Scanner;

public class CalculatePi {
    
    static class PiCalculator implements Runnable {
        private final double a;
        private final double b;
        private final int n;
        private final int start;
        private final int end;
        private double sum;

        public PiCalculator(double a, double b, int n, int start, int end) {
            this.a = a;
            this.b = b;
            this.n = n;
            this.start = start;
            this.end = end;
            this.sum = 0.0;
        }

        @Override
        public void run() {
            for (int i = start; i < end; i++) {
                double x = a + (b - a) * i / n;
                double value = 4.0 / (1.0 + x * x);
                sum += value;
            }
        }

        public double getSum() {
            return sum;
        }
    }

    public static double calculatePi(double a, double b, int n, int numThreads) throws InterruptedException {
        Thread[] threads = new Thread[numThreads];
        PiCalculator[] calculators = new PiCalculator[numThreads];
        int chunkSize = n / numThreads;
        
        for (int i = 0; i < numThreads; i++) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? n : start + chunkSize;
            calculators[i] = new PiCalculator(a, b, n, start, end);
            threads[i] = new Thread(calculators[i]);
            threads[i].start();
        }
        
        double sum = 0.0;
        for (int i = 0; i < numThreads; i++) {
            threads[i].join();
            sum += calculators[i].getSum();
        }
        
        sum *= (b - a) / n;
        return sum;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("[Вычисление числа PI по формуле средних прямоугольников]\n");
        System.out.print("Введите число интегралов для получения числа PI (например, 10000): ");
        int n = scanner.nextInt();
        
        int numThreads = Runtime.getRuntime().availableProcessors();
        double startTime = System.currentTimeMillis();
        
        try {
            double pi = calculatePi(0.0, 1.0, n, numThreads);
            double endTime = System.currentTimeMillis();
            
            System.out.printf("Полученное число PI: %.15f%n", pi);
            System.out.printf("Время выполнения: %.5f секунд%n", (endTime - startTime) / 1000.0);
        } catch (InterruptedException e) {
            System.err.println("Поток был прерван");
        }
    }
}
