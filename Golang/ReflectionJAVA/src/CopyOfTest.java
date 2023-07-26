/**
 * @author Xinran
 * @version 1.0
 * @description TODO
 * @date 2023/6/17 16:40
 */
import sun.awt.geom.AreaOp;

import java.lang.reflect.*;
import java.util.*;

public class CopyOfTest {
    public static void main(String[] args) {
        int []a = {1,2,3};
        a = (int[]) goodCopyOf(a,10);
        System.out.println(Arrays.toString(a));

        String[] b = {"Tom", "Dick", "Harry"};
        b = (String[]) goodCopyOf(b,10);
        System.out.println(Arrays.toString(b));

        System.out.println("下面这个方法是错的");
        //因为一个从开始就是Object[]的数组是不能转换成其他类型的数组的
        //b = (String[]) badCopyOf(b,10);
    }
    /**
     * This method attempts to grow an array by allocating a new array and copying all elements.
     * @param a the array to grow
     * @param newLength the new length
     * @return a larger array that contains all elements of a. However, the returned array has
     * type Object[], not the same type as a
     */
    public static Object[] badCopyOf(Object[] a, int newLength) // not useful
    {
        Object[] newArray = new Object[newLength];
        System.arraycopy(a,0,newArray,0,Math.min(a.length,newLength));
        return newArray;
    }
    /**
     * This method grows an array by allocating a new array of the same type and
     * copying all elements.
     * @param a the array to grow. This can be an object array or a primitive
     * type array
     * @return a larger array that contains all elements of a.
     */
    //这个是正确的方法，这是一个通用的扩容任意类型数组的函数
    public static Object goodCopyOf(Object a, int newLength)
    {
        Class cl = a.getClass();
        if(!cl.isArray()) return null;
        Class componentType = cl.getComponentType();
        int length = Array.getLength(a);
        Object newArray = Array.newInstance(componentType,newLength);
        System.arraycopy(a,0,newArray,0,Math.min(length,newLength));
        return newArray;
    }
}
