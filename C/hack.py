MOD = 1_000_000_007


def solve(n, m):
    if n > m: return -1

    two = 1
    while two < n: two <<= 1

    result = 0
    if two != n: result = two - n

    while two < m:
        two <<= 1
        result += 1

    return result % MOD


if __name__ == '__main__':
    t = int(input())
    for _ in range(t):
        s = input()
        t = input()

        n, m = int(s, 2), int(t, 2)
        print(solve(n, m))
