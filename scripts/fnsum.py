# Summarise an objdump -dr listing: per function, the calls made (by reloc name)
# and the distinct immediate constants >= 0x100 used. Usage: fnsum.py <dis> [func-regex]
import re, sys, collections
dis = open(sys.argv[1]).read().splitlines()
want = re.compile(sys.argv[2]) if len(sys.argv) > 2 else None
cur, calls, consts, prev = None, collections.OrderedDict(), {}, ""
fn_re = re.compile(r'^[0-9a-f]+ <([^>]+)>:$')
rel_re = re.compile(r'R_386_(?:PC32|32|PLT32)\s+(\S+)')
imm_re = re.compile(r'\$0x([0-9a-f]+)')
for line in dis:
    m = fn_re.match(line)
    if m:
        cur = m.group(1); calls.setdefault(cur, []); consts.setdefault(cur, set()); continue
    if cur is None: continue
    r = rel_re.search(line)
    if r and 'call' in prev: calls[cur].append(r.group(1))
    for v in imm_re.findall(line):
        if int(v, 16) >= 0x100: consts[cur].add(int(v, 16))
    prev = line
    globals()['prev'] = line
for f in calls:
    if want and not want.search(f): continue
    cs = collections.Counter(calls[f])
    print(f'== {f}')
    if cs: print('   calls :', ', '.join(f'{k}x{v}' if v > 1 else k for k, v in cs.items()))
    if consts[f]: print('   consts:', ' '.join(hex(c) for c in sorted(consts[f])))
