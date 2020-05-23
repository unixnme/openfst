## Composition with Phi-Matcher

Let's create a simple graph.
```bash
fstcompile --isymbols=ascii.syms --osymbols=ascii.syms --acceptor --keep_isymbols --keep_osymbols input.txt input.fst
```

![alt text](input.png "Input graph")

We want to compose above with another graph that has `<phi>` input label, i.e., backoff.
```bash
fstcompile --isymbols=ascii.syms --osymbols=ascii.syms --keep_isymbols --keep_osymbols output.txt | fstarcsort --sort_type=ilabel - output.fst 
```

![alt text](output.png "Output graph")

Simple `fstcompose` yields
```bash
fstcompose input.fst output.fst composed.fst
```

![alt text](composed.png "Generically composed graph")

On contrary, phi-matcher composition yields
```bash
./phi_compose input.fst output.fst 
```
![alt text](composed_phi.png "Composed graph with phi-matcher")