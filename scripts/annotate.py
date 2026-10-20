import re, sys
hdr, dis, out = sys.argv[1:4]
base = 0x10000
iat = {}
dll = None
for line in open(hdr):
    m = re.match(r'\s*DLL Name: (\S+)', line)
    if m: dll = m.group(1); continue
    m = re.match(r'\s*([0-9a-f]{8})\s+<none>\s+[0-9a-f]+\s+(\w+)', line)
    if m and dll: iat[int(m.group(1),16)+base] = m.group(2)
txt = open(dis).read()
def rep(m):
    a = int(m.group(1),16)
    return m.group(0) + (f'  ; <{iat[a]}>' if a in iat else '')
txt = re.sub(r'ds:0x([0-9a-f]+)\]?', rep, txt)
open(out,'w').write(txt)
counts = {}
for n in re.findall(r'; <(\w+)>', txt): counts[n]=counts.get(n,0)+1
for k,v in sorted(counts.items(), key=lambda x:-x[1]): print(f'{v:4d}  {k}')
