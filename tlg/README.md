## Creating TLG Graph for CTC Models using OpenFST

### What is TLG?
T stands for token. T graph maps CTC acoustic tokens to characters. 
L stands for lexicon, mapping characters to word. 
G stands for grammar, assigning score for a sequence of words. 
For more details, refer to this [paper]()

#### Token Graph
In CTC, there is a blank symbol `<blk>` which complicates token (T) graph a bit.
Without the blank symbol, T graph will reduce to the following

![alt text](ifst.png "Identity T Graph")

Essentially, this is just a simple acceptor with all possible tokens.
With blank symbol, the T graph should look like below

![alt text](ofst.png "CTC T Graph with Backoff <phi>")

Here, `<phi>` represents a back off label; 
we take the `<phi>` arc when there is no matching label.
For example, we transition from state 1 to 0 for any input label but `a`.
The caveat is, at state 0 we again look for transition for the same input label.
That is because `<phi>` does not _consume_ the label.
See [Matchers](http://www.openfst.org/twiki/bin/view/FST/FstAdvancedUsage#Matchers) document in OpenFst for more details.

By convention, `<eps>` represents no-input and no-output.
Say we start from state 0 and get input token sequence `<blk> a a b <blk> b`.
From the T graph above, we will get output token sequence `<eps> a <eps> b <eps> b`.
Removing `<eps>` output tokens, we get `a b b` as expected.

### Expanding Backoff Arcs
What if we want to represent the T graph without any backoff arcs?
Below is the expanded T graph that we want.

![alt text](tokens_exp.png "CTC T Graph without Backoff transitions")

Ah, it's a mess, even with just three of 26 alphabet characters.
State 0 is the start state as usual. 
State 1 represents `<space>` character.
State 2, 3, and 4 each represents `a`, `b`, and `c` character, respectively.
As you can see, there is no `<phi>` label.
Every back off transition is explicitly represented by a dedicated transition arcs.

Why would we ever want to represent the T graph in this convoluted form?
For one, OpenFST's shell binary `fstcompose` does not seem to support special matchers,
so if we want to run composition using the shell binary, we should keep it expanded.

Below represents input token sequence `<blk> a a b <blk> b`.

![alt text](input.png "Input token sequence")

What would happen if we simply `fstcompose` this input with our non-expanded T graph with backoff arcs?
Answer: we get a blank graph, i.e., we can't successfully compose the two
because `<phi>` label is taken as a regular symbol and hence no valid transition at `b` after `<blk> a a` transitions.

What if we compose the expanded T graph with the same input? Viola!

![alt text](output.png "Output token sequence by composing input to expanded T")

Projecting the output labels and ignoring `<eps>`, we obtain output token sequence `a b b` as expected.
So, the question remains: how do we expand the T graph?
For that, take a look at [phi_compose](../phi_compose) directory.

### Lexicon and Grammar
Suppose simple grammar G below.

![alt text](3gram.png "Simple grammar graph")

Here, `<eps>` represents backoff transition.

We will compose G with two versions of lexicon L graphs. 
In the first case, the L graph and composed LG graph are shown below.

![alt text](lex2.png "Simple grammar graph")
![alt text](LG2.png "LG graph")

In the second case, the L graph and the composed LG graph are shown below.

![alt text](lex.png "Simple grammar graph")
![alt text](LG.png "LG graph")

In the above case, the `<epsilon>` transitions remain intact
because OpenFST's generic composition treats `<epsilon>` as a special symbol.

### TLG Construction
Now we are ready to construct TLG graph for CTC.
First, we create T graphs.
```bash
echo "a b e l n p r" | python create_t.py
```
This will create three files: `tokens.syms`, `tokens.txt`, and `tokens_self.txt`.
Let's compile `tokens.txt` and `tokens_self.txt` into FSTs.
`tokens.txt` is CTC's mapping from acoustic output token T to lexical character.
`tokens_self.txt` is simple self-mapping to expand acoustic output token T.
One thing to make sure here is that `tokens_self.txt` should not have `<phi>` label.
```bhas
fstcompile --isymbols=tokens.syms --osymbols=tokens.syms --keep_isymbols --keep_osymbols tokens.txt | fstarcsort --sort_type=ilabel - tokens.fst
fstcompile --isymbols=tokens.syms --osymbols=tokens.syms --keep_isymbols --keep_osymbols tokens_self.txt tokens_self.fst
```
![alt text](tokens.png "CTC T graph")

![alt text](tokens_self.png "CTC T self graph")

We now compose the two using phi-matcher.
```bash
./phi_compose tokens_self.fst tokens.fst T.fst 
```

![alt text](T.png "CTC T expanded graph")

Next we create lexicon L graph.
```bash
echo "apple pear banana" | python create_lex.py
```
This will create `lexicon.txt` and `lexicon.syms` files.
Let's compile L graph first.
```bash
fstcompile --isymbols=tokens.syms --osymbols=lexicon.syms --keep_isymbols --keep_osymbols lexicon.txt L.fst
```

![alt text](L.png "CTC T expanded graph")

One thing to note here is that we have to make sure repetitive space characters is non-emitting.
This is because `<space> <space> p e a r <space> <space> <blk> <space>` should map to `pear` regardless of extra spaces.

Finally, we create grammar G graph using KenLM.
```bash
lmplz --order 3 --discount_fallback  < corpus.txt > 3gram.arpa
ngramread --ARPA --symbols=lexicon.syms 3gram.arpa 3gram.fst 
```

![alt text](3gram.png "CTC G graph")

Note that we have back off label as `<epsilon>` instead of `<phi>`.
We relabel the input symbol index from `<epsilon>`'s 0 to `<phi>`'s 1.
These indices are in `lexicon.syms`.
```bash
fstrelabel --relabel_ipairs=relabel.txt 3gram.fst G.fst
```

We now phi-compose L and G to create LG graph.
```bash
./phi_compose L.fst G.fst LG.fst
```

![alt text](LG.png "CTC LG graph fully expanded")

Lastly, we compose T with LG to obtain TLG graph.
```bash
fstcompose T.fst LG.fst | fstminimize - TLG.fst
```

![alt text](TLG.png "CTC TLG graph fully expanded")


