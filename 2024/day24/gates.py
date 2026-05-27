import sys

# build a set that maps wire names to values: wires
# build a map from wire outputs to inputs: gates
# build a mpa from wire inputs to instances of usage in gates: uses
def parse(filename: str):
    fptr = open(filename, "r")

    wire_data, gate_data = fptr.read().strip().split('\n\n')
    fptr.close()

    wires = {}
    for line in wire_data.split('\n'):
        name, val = line.split(':')
        wires[name] = int(val)
    
    uses = {}
    gates = {}
    for line in gate_data.split('\n'):
        # a op b -> out
        a, op, b, _, out = line.split(' ')
        gates[out] = (a, op, b)
        
        for w in (a, b):
            uses.setdefault(w, []).append((a, op, b, out))

    return wires, gates, uses

def part_one(wires, gates):
    zs = {}
    while True:

        changed = False
        for out in gates:

            if out in wires or out in zs:
                continue

            a, op, b = gates[out]
            if a not in wires or b not in wires:
                continue
            
            regA = wires[a]
            regB = wires[b]
            match op:
                case "AND":
                    result = regA & regB
                case "OR":
                    result = regA | regB
                case "XOR":
                    result = regA ^ regB

            if out[0] == 'z':
                zs[out] = result
            else:
                wires[out] = result

            changed = True

        if not changed:
            break
            
    ans = 0
    for ix, name in enumerate(sorted(zs)):
        ans += zs[name] * (1 << ix)
    print(ans)


# General stage of a full adder looks like
# sum = (a XOR b) XOR cin
# cout = (a AND b) OR ((a XOR b) AND cin)
# So we have the following rules:
#   1. every sum must be an XOR (except z45 - that is just the cout from z44)
#   2. every internal XOR must be of a and b
#   3. AND gates should feed into OR gates () go straight into an OR
#   4. XOR gates should NOT feed OR gates
def part_two(gates, uses):

    bad = set()
    for out in gates:
        a, op, b = gates[out]

        # Rule 1:
        # Every z wire except final carry should come from XOR
        if out.startswith("z") and out != "z45":
            if op != "XOR":
                bad.add(out)

        # Rule 2:
        # Internal XORs must connect x/y inputs or z outputs
        if op == "XOR":
            names = {a[0], b[0], out[0]}

            if names == {"x", "y", "z"}:
                pass
            elif "x" not in names and "y" not in names and out[0] != "z":
                bad.add(out)

        # Rule 3:
        # AND gates should feed OR gates (except bit 0 carry)
        if op == "AND" and a != "x00" and b != "x00":
            found_or = False

            # check that the output wire feeds into an OR gate
            for g in uses.get(out, []):
                if g[1] == "OR":
                    found_or = True

            if not found_or:
                bad.add(out)

        # Rule 4:
        # XOR gates should not feed OR gates
        if op == "XOR":
            for g in uses.get(out, []):
                if g[1] == "OR":
                    bad.add(out)

    print(",".join(sorted(bad)))
            

def main():
    if len(sys.argv) < 2:
        print("no file given")
        return
    
    wires, gates, uses = parse(sys.argv[1])

    part_one(wires, gates)
    part_two(gates, uses)

main()