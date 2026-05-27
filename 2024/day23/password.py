from collections import defaultdict
import sys

best = set()

def bronk(graph, R, P, X):
    global best

    if not P and not X:
        if len(R) > len(best):
            best = R.copy()
        return

    # simple pivot heuristic
    u = max(P | X, key=lambda v: len(graph[v]), default=None)

    for v in P - graph[u]:
        bronk(
            graph,
            R | {v},
            P & graph[v],
            X & graph[v]
        )

        P.remove(v)
        X.add(v)

def parse(filename):
    graph = defaultdict(set)
    with open(filename) as f:
        for line in f:
            a, b = line.strip().split('-')
            graph[a].add(b)
            graph[b].add(a)

    f.close()
    return graph


def main():
    if len(sys.argv) < 2:
        print("no file given")
        return

    graph = parse(sys.argv[1])
    nodes = set(graph.keys())
    bronk(graph, set(), nodes, set())


    print(",".join(sorted(best)))


main()