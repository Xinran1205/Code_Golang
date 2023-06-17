/**
 * @author Xinran
 * @version 1.0
 * @description TODO
 * @date 2023/6/17 16:55
 */
import java.lang.reflect.*;

/**
 * This program shows how to invoke methods through reflection.
 * @Author Xinran
 */
//调用任意方法和构造器
public class MethodTableTest {
    public static void main(String[] args) throws ReflectiveOperationException{
        //get method pointers to the square and sqrt methods
        //获取方法
        Method square = MethodTableTest.class.getMethod("square",double.class);
        Method sqrt = Math.class.getMethod("sqrt",double.class);

        //print tables of x- and y-values
        printTable(1,10,10,square);
        printTable(1,10,10,sqrt);
    }
    /**
     * Returns the square of a number
     * @param x a number
     * @return x squared
     */
    public static double square(double x) {
        return x * x;
    }

    /**
     * Prints a table with x- and y-values for a method
     * @param from the lower bound for the x-values
     * @param to the upper bound for the x-values
     * @param n the number of rows in the table
     * @param f a method with a double parameter and double return value
     */
    public static void printTable(double from,double to,int n,Method f) throws ReflectiveOperationException {
        //print out the method as table header
        System.out.println(f);
        double dx = (to - from) / (n - 1);
        //用f来调用函数
        for (double x = from; x <= to; x += dx) {
            double y = (Double) f.invoke(null, x);
            System.out.printf("%10.4f | %10.4f%n", x, y);
        }
    }
}
