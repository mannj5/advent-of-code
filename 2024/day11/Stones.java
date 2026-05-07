
import java.util.*;

public class Day11 {

    public static void main(String[] args) {
        // int[] stones = {125, 17};
        List<Long> stones = List.of(9759L, 0L, 256219L, 60L, 1175776L, 113L, 6L, 92833L);
        Stones s = new Stones();

        System.out.println(s.countList(stones));
    }
}

public class Stones {

    private Map<Pair<Integer, Long>, Long> memo = new HashMap<>();
    
    public long countList(List<Long> stones) {
        long total = 0;
        for (long i : stones) {
            total += count(75, i);
        }
        return total;
    }

    public long count(int steps, long n) {
        if (steps == 0) {
            return 1;
        }

        Pair<Integer, Long> key = new Pair<>(steps, n);
        if (memo.containsKey(key)) {
            return memo.get(key);
        }

        long result;
        if (n == 0) {
            result = count(steps - 1, 1);
        } else {
            int num = Long.toString(n).length();
            if (num % 2 == 0) {
                int mid = num / 2;
                long left = n / (long) Math.pow(10, mid);
                long right = n % (long) Math.pow(10, mid);
                result = count(steps - 1, left) + count(steps - 1, right);
            } else {
                result = count(steps - 1, n * 2024);
            }
        }

        memo.put(key, result);
        return result;
    }
}

public class Pair<K, V> {
    private final K key;
    private final V value;

    public Pair(K key, V value) {
        this.key = key;
        this.value = value;
    }

    public K getKey() { return key; }
    public V getValue() { return value; }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Pair<?, ?> p)) return false;
        return Objects.equals(key, p.key) && Objects.equals(value, p.value);
    }

    @Override
    public int hashCode() {
        return Objects.hash(key, value);
    }
}