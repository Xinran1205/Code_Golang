package com.example.demo;

import com.google.common.hash.BloomFilter;
import com.google.common.hash.Funnels;

/**
 * @author Xinran
 * @version 1.0
 * @description TODO
 * @date 2023/7/21 22:34
 */
public class BloomFilterTest {
    /** 预计插入的数据 */
    private static Integer expectedInsertions = 10000000;
    /** 误判率 */
    private static Double fpp = 0.01;
    /** 布隆过滤器 */
    private static BloomFilter<Integer> bloomFilter = BloomFilter.create(Funnels.integerFunnel(), expectedInsertions, fpp);

    public static void main(String[] args) {
        // 插入 1千万数据
        for (int i = 0; i < expectedInsertions; i++) {
            bloomFilter.put(i);
        }

        // 用1千万数据测试误判率，这一千万是没有插入进去的，所以如果if判断存在，那么就是误判了
        int count = 0;
        for (int i = expectedInsertions; i < expectedInsertions *2; i++) {
            if (bloomFilter.mightContain(i)) {
                count++;
            }
        }
        System.out.println("一共误判了：" + count);

    }
}
