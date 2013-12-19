from collections import defaultdict, Counter
import sys

def bidirected(struct, pos):
    numA, signA = struct[pos * 2]
    numB, signB = struct[pos * 2 + 1]
    return numA * numB > 0 and signA * signB > 0


def simplifiable(struct, n):
    for i in xrange(n):
        numA, signA = struct[i * 2]
        numB, signB = struct[i * 2 + 1]
        if numA * numB < 0 and signA * signB < 0:
            return True
    
    for i in xrange(n):
        for j in xrange(n):
            if i == j: continue
            if not bidirected(struct, i) or not bidirected(struct, j):
                continue
            if struct[i * 2][0] * struct[j * 2][0] < 0 and struct[i * 2][1] * struct[j * 2][1]:
                return True
    return False

def separatable(struct, n):
    for i in xrange(1, 2 ** (n - 1)):
        count = Counter()
        for j in xrange(n):
            if (2 ** j) & i:
                count[abs(struct[2 * j][0])] += 1
                count[abs(struct[2 * j + 1][0])] += 1
        ok = True
        for key, value in count.iteritems():
            if value != 2:
                ok = False
        if ok:
            return True
    return False

def intersect(a, b):
     return list(set(a) & set(b))

def sign(x):
    if x < 0:
        return -1
    return 1

def check(struct, n):
    #print "CHECK", struct
    if simplifiable(struct, n):
        return

    #print "NOT SIMPLIFIABLE"

    if separatable(struct, n):
        return
    
    #print "NOT SEPARATABLE"

    edges = defaultdict(lambda: [])
    edges_sign = {}
    for i, elem in enumerate(struct):
        edges[abs(elem[0])].append(i / 2)
        edges_sign[abs(elem[0])] = sign(elem[0] * elem[1])
    for i in edges:
        for j in edges:
            if i == j: continue
            if not intersect(edges[i], edges[j]) and edges_sign[i] == edges_sign[j]:
                return

    print n
    for elem in struct:
        print elem[0], elem[1]
    print

    
def gen(edge, visited, depth, n):
    if depth == n:
        check(visited, n)
        return

    if simplifiable(visited, n):
        return

    while edge < 2 * n and visited[edge] != (0, 0):
        edge += 1

    for sign in [-1, 1]:
        for direction in [-1, 1]:
            if depth == 0 and (sign != 1 or direction != 1):
                continue
            visited[edge] = ((depth + 1) * direction, sign)
            for i in xrange(edge + 1, 2 * n):
                if visited[i] == (0, 0):
                    visited[i] = (-(depth + 1) * direction, -sign)
                    gen(edge + 1, visited, depth + 1, n)
                    visited[i] = (0, 0)
            visited[edge] = (0, 0)


for n in xrange(4, 5):
    gen(0, [(0, 0) for _ in xrange(2 * n)], 0, n)

