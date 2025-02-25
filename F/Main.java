package F;

import java.io.*;
import java.util.*;

/**
 * Made by egor https://github.com/chermehdi/egor.
 *
 * @author Azuz
 *
 */
public class Main {

  void solve(InputReader in, PrintWriter out) {
    int r = in.nextInt();
    int c = in.nextInt();

    int[][] m = new int[r][c];
    int[] grow = new int[r];
    int[] gcol = new int[c];
    for (int i = 0; i < r; ++i) {
      for (int j = 0; j < c; ++j) {
          m[i][j] = in.nextInt();
          grow[i] = gcd(m[i][j], grow[i]);
          gcol[j] = gcd(m[i][j], gcol[j]);
        }
      }

      int ans = Math.min(r, c);
      int cnt = 0;

      Trie trie = new Trie();
      for (int i = 0; i < r; ++i) {
        int[] tmp = new int[c];
        for (int j = 0; j < c; ++j) {
          if (grow[i] > 0)
            tmp[j] = m[i][j] / grow[i];
        }

        if (!trie.insert(tmp)) {
          ++cnt;
        }
      }

      ans = Math.min(cnt, ans);
      cnt = 0;
      trie = new Trie();

      for (int j = 0; j < c; ++j) {
        int[] tmp = new int[r];
        for (int i = 0; i < r; ++i) {
          if (gcol[j] > 0)
            tmp[i] = m[i][j] / gcol[j];
        }

        if (!trie.insert(tmp)) {
          ++cnt;
        }
      }

      out.println(Math.min(cnt, ans));
    }

    public static void main(String[] args) {
      InputReader in = new InputReader(System.in);
      try (PrintWriter out = new PrintWriter(System.out)) {
        new Main().solve(in, out);
      }
    }

    private static int gcd(int a, int b) {
      if (b == 0) return a;
      return gcd(b, a % b);
    }
  }



  class Trie {

    TrieNode root = new TrieNode();

    public boolean insert(int[] arr) {
      TrieNode cur = root;
      boolean exists = true;
      for (int i : arr) {
        TrieNode next = cur.get(i);

        if (next == null) {
          exists = false;
          next = cur.create(i);
        }
        cur = next;
      }

      return exists;
    }

  }

  class TrieNode {
    HashMap<Integer, TrieNode> next;

    public TrieNode() {
      next = new HashMap<>();
    }


    public TrieNode get(int x) {
      return next.get(x);
    }

    public TrieNode create(int x) {
      TrieNode node = new TrieNode();
      next.put(x, node);
      return node;
    }

    public void set(int x, TrieNode node) {
      next.put(x, node);
    }


  }

  class InputReader  {

    private InputStream stream;
    private static final int DEFAULT_BUFFER_SIZE = 1 << 16;
    private static final int EOF = -1;
    private byte[] buf = new byte[DEFAULT_BUFFER_SIZE];
    private int curChar;
    private int numChars;

    public InputReader(InputStream stream) {
      this.stream = stream;
    }

    public int[] readIntArray(int tokens) {
      int[] ret = new int[tokens];
      for (int i = 0; i < tokens; i++) {
        ret[i] = nextInt();
      }
      return ret;
    }

    public int read() {
      if (this.numChars == EOF) {
        throw new UnknownError();
      } else {
        if (this.curChar >= this.numChars) {
          this.curChar = 0;

          try {
            this.numChars = this.stream.read(this.buf);
          } catch (IOException ex) {
            throw new InputMismatchException();
          }

          if (this.numChars <= 0) {
            return EOF;
          }
        }

        return this.buf[this.curChar++];
      }
    }

    public int nextInt() {
      int c;
      for (c = this.read(); isSpaceChar(c); c = this.read()) {
      }

      byte sgn = 1;
      if (c == 45) {
        sgn = -1;
        c = this.read();
      }

      int res = 0;

      while (c >= 48 && c <= 57) {
        res *= 10;
        res += c - 48;
        c = this.read();
        if (isSpaceChar(c)) {
          return res * sgn;
        }
      }

      throw new InputMismatchException();
    }

    public long nextLong() {
      int c;
      for (c = this.read(); isSpaceChar(c); c = this.read()) {
      }

      byte sgn = 1;
      if (c == 45) {
        sgn = -1;
        c = this.read();
      }

      long res = 0;

      while (c >= 48 && c <= 57) {
        res *= 10L;
        res += c - 48;
        c = this.read();
        if (isSpaceChar(c)) {
          return res * sgn;
        }
      }
      throw new InputMismatchException();
    }

    public double nextDouble() {
      double ret = 0, div = 1;
      int c = read();
      while (c <= ' ') {
        c = read();
      }
      boolean neg = (c == '-');
      if (neg) {
        c = read();
      }

      do {
        ret = ret * 10 + c - '0';
      }
      while ((c = read()) >= '0' && c <= '9');

      if (c == '.') {
        while ((c = read()) >= '0' && c <= '9') {
          ret += (c - '0') / (div *= 10);
        }
      }

      if (neg) {
        return -ret;
      }
      return ret;
    }

    public String next() {
      int c;
      while (isSpaceChar(c = this.read())) {
      }

      StringBuilder result = new StringBuilder();
      result.appendCodePoint(c);

      while (!isSpaceChar(c = this.read())) {
        result.appendCodePoint(c);
      }

      return result.toString();
    }

    public String nextLine() {
      int c;
      StringBuilder result = new StringBuilder();
      boolean read = false;
      while ((c = this.read()) != '\n') {
        if (c == -1) {
          return null;
        }
        result.appendCodePoint(c);
        read = true;
      }
      if (!read) {
        return null;
      }
      return result.toString();
    }

    public static boolean isSpaceChar(int c) {
      return c == 32 || c == 10 || c == 13 || c == 9 || c == EOF;
    }

    public int[] nextIntArray(int n) {
      int[] arr = new int[n];
      for (int i = 0; i < n; i++) {
        arr[i] = nextInt();
      }
      return arr;
    }

    public long[] nextLongArray(int n) {
      long[] arr = new long[n];
      for (int i = 0; i < n; i++) {
        arr[i] = nextLong();
      }
      return arr;
    }

    public int[][] nextIntMatrix(int n, int m) {
      int[][] arr = new int[n][m];
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          arr[i][j] = nextInt();
        }
      }
      return arr;
    }

    public long[][] nextLongMatrix(int n, int m) {
      long[][] arr = new long[n][m];
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          arr[i][j] = nextLong();
        }
      }
      return arr;
    }

    public char[] nextCharArray() {
      return next().toCharArray();
    }

    public double[] nextDoubleArray(int n) {
      double[] ret = new double[n];
      for (int i = 0; i < n; i++) {
        ret[i] = nextDouble();
      }
      return ret;
    }

    public int[]
    nextIntArrayOneBased(int n) {
      int[] ret = new int[n + 1];
      for (int i = 1; i <= n; i++) {
        ret[i] = nextInt();
      }
      return ret;
    }

    public char[][] nextCharMatrix(int n, int m) {
      char[][] res = new char[n][m];
      for (int i = 0; i < n; ++i) {
        res[i] = nextCharArray();
      }
      return res;
    }
  }
