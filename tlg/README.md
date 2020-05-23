## Creating TLG graph for CTC models using OpenFST

### What is TLG?
T stands for token. T graph maps CTC acoustic tokens to characters. 
L stands for lexicon, mapping characters to word. 
G stands for grammar, assigning score for a sequence of words. 
For more details, refer to this [paper]()

#### Token Graph
In CTC, there is a blank symbol` <blk>` which complicates token (T) graph a bit.
Without the blank symbol, T graph will reduce to the following

![alt text](ifst.png "Idea T Graph")

Essentially, this is just a simple acceptor with all possible tokens.
With blank symbol, the T graph should look like below

![alt text](ofst.png "CTC T Graph with Backoff <phi>")

Here, `<phi>` represents a back off label; 
we take the `<phi>` arc when there is no matching label.
For example, we transition from state 1 to 0 for any input label but `a`.
The caveat is, at state 0 we again look for transition for the same input label.
That is because `<phi>` label does not _consume_ the label.
See [Matchers](http://www.openfst.org/twiki/bin/view/FST/FstAdvancedUsage#Matchers) document in OpenFst.

By convention, `<eps>` represents no-input and no-output.
Say we start from state 0 and get input token sequence `<blk> a a b <blk> b`.
From the T graph above, we will get output token sequence `<eps> a <eps> <eps> b <eps> <eps> b`.
Note that we take back off transition twice, yielding output token sequence length of 8 compared to input token sequence of 6.
Removing `<eps>` output tokens, we get `a b b` as expected.

