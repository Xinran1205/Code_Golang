/**
 * @author Xinran
 * @version 1.0
 * @description TODO
 * @date 2023/6/17 14:46
 */

//首先，反射是什么意思，java中的反射：能够分析类能力的程序称为反射
//反射的功能，这些比较常见，还有其他很多功能
//1.在运行时分析类的能力，看ReflectionTest.java这个类
//2.在运行时检查对象，例如，编写一个适用于所有类的toString方法 //这个没写，可以看书ObjectAnalyzer.java这个类
//3.实现通用的数组操作代码（泛型数组）看CopyOfTest.java这个类
//4.利用Method对象，这个对象很像C++中的函数指针，看MethodTableTest.java这个类
public class JavaReflection {

    public static void main(String[] args) {
        //Son.class是编译时确定的，表示Son类的静态信息；
        // 而e.getClass()是运行时确定的，表示e对象的实际类型。
        Father e = new Son();
        Class a = e.getClass();
        if (a == Son.class) {
            System.out.println("a == Son.class");
        }
        //检查类的结构，Field，Method，Constructor三个类可以用来描述类的字段，方法和构造器，具体可以看ReflectionTest.java这个类
        //这个返回1，说明是public的，这个是因为在Father类中，name和age都是默认的，而默认的是public的
        //不同修饰符返回不同的值，例如：如果返回的是3，则表示该成员既使用了public修饰符（值为1），
        //又使用了private修饰符（值为2），因为3的二进制表示为 11，其中每一位表示一种修饰符的使用情况。
        System.out.println(a.getModifiers());
    }

}
