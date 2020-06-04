package ch4;

import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.*;

public class ForkJoinSort {
    public static void main(String[] args) {
        ForkJoinPool pool = new ForkJoinPool();
        Scanner sc = new Scanner(System.in);
        int length = sc.nextInt();
        int[] array = new int[length];
        int[] aux = new int[length];
        // Quick sort
        long start3 = System.nanoTime();
        QuickSortTask task2 = new QuickSortTask(0, length - 1, array);
        pool.invoke(task2);
        long elapsedTime3 = System.nanoTime() - start3;
        System.out.println("Multi threaded quicksort time: " + elapsedTime3 / 1000000 + " ms");
        if (isSorted(array))
            System.out.println("success");
        else
            System.out.println("failure");
        // Arrays.sort
        for (int i = 0; i < length; i++) {
            array[i] = (int) (Math.random() * 1000);
        }
        long start = System.nanoTime();
        Arrays.sort(array);
        long elapsedTime = System.nanoTime() - start;
        System.out.println("Arrays.sort time: " + elapsedTime / 1000000 + " ms");
        // Merge sort
        for (int i = 0; i < length; i++) {
            array[i] = (int) (Math.random() * 1000);
        }
        long start2 = System.nanoTime();
        MergeSortTask task = new MergeSortTask(0, length - 1, array, aux);
        pool.invoke(task);
        long elapsedTime2 = System.nanoTime() - start2;
        System.out.println("Multi threaded mergesort time: " + elapsedTime2 / 1000000 + " ms");
        if (isSorted(array))
            System.out.println("success");
        else
            System.out.println("failure");
        for (int i = 0; i < length; i++) {
            array[i] = (int) (Math.random() * 1000);
        }

    }

    public static boolean isSorted(int[] array) {
        for (int i = 0; i < array.length - 1; i++) {
            if (array[i + 1] < array[i])
                return false;
        }
        return true;
    }

    public static class MergeSortTask extends RecursiveAction {
        static final int THRESSHOLD = 100;
        private int begin;
        private int end;
        private int[] array;
        private int[] aux;

        public MergeSortTask(int begin, int end, int[] array, int[] aux) {
            this.begin = begin;
            this.end = end;
            this.array = array;
            this.aux = aux;
        }

        @Override
        protected void compute() {
            if (end - begin < THRESSHOLD) {
                for (int i = begin + 1; i <= end; i++) {
                    for (int j = i; j > begin && array[j - 1] > array[j]; j--) {
                        int temp = array[j];
                        array[j] = array[j - 1];
                        array[j - 1] = temp;
                    }
                }
            } else {
                int mid = (begin + end) / 2;
                MergeSortTask leftTask = new MergeSortTask(begin, mid, array, aux);
                MergeSortTask rightTask = new MergeSortTask(mid + 1, end, array, aux);
                leftTask.fork();
                rightTask.fork();
                leftTask.join();
                rightTask.join();
                int i = begin;
                int j = mid + 1;
                for (int k = begin; k <= end; k++) {
                    aux[k] = array[k];
                }
                for (int k = begin; k <= end; k++) {
                    if (i > mid)
                        array[k] = aux[j++];
                    else if (j > end)
                        array[k] = aux[i++];
                    else if (aux[i] > aux[j])
                        array[k] = aux[j++];
                    else
                        array[k] = aux[i++];
                }
            }
        }

    }

    public static class QuickSortTask extends RecursiveAction {
        static final int THRESSHOLD = 100;
        private int begin;
        private int end;
        private int[] array;

        public QuickSortTask(int begin, int end, int[] array) {
            this.begin = begin;
            this.end = end;
            this.array = array;
        }

        @Override
        protected void compute() {
            if (end - begin < THRESSHOLD) {
                for (int i = begin + 1; i <= end; i++) {
                    for (int j = i; j > begin && array[j - 1] > array[j]; j--) {
                        int temp = array[j];
                        array[j] = array[j - 1];
                        array[j - 1] = temp;
                    }
                }
            } else {
                int pointer = array[begin];
                int i = begin;
                int j = end + 1;
                while (true) {
                    while (pointer > array[++i])
                        if (i == end)
                            break;
                    while (pointer < array[--j])
                        ;
                    if (i >= j)
                        break;
                    int temp = array[j];
                    array[j] = array[i];
                    array[i] = temp;
                }
                array[begin] = array[j];
                array[j] = pointer;

                QuickSortTask leftTask = new QuickSortTask(begin, j, array);
                QuickSortTask rightTask = new QuickSortTask(j + 1, end, array);
                leftTask.fork();
                rightTask.fork();
                leftTask.join();
                rightTask.join();
            }
        }

    }
}