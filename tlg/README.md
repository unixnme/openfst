## Creating TLG graph for CTC models using OpenFST

### What is TLG?
T stands for token. T graph maps CTC acoustic tokens to characters. 
L stands for lexicon, mapping characters to word. 
G stands for grammar, assigning score for a sequence of words. 
For more details, refer to this [paper]()

#### Token Graph
In CTC, there is a blank symbol` <blk>` which complicates token (T) graph a bit.
Without the blank symbol, T graph will reduce to the following

<object data="ifst.pdf" type="application/pdf" width="700px" height="700px">
    <embed src="ifst.pdf">
        <p>This browser does not support PDFs. Please download the PDF to view it: <a href="ifst.pdf">Download PDF</a>.</p>
    </embed>
</object> 

![alt text](ifst.png "Idea T Graph")

Basically, this is just a simple acceptor with

<object data="http://yoursite.com/the.pdf" type="application/pdf" width="700px" height="700px">
    <embed src="http://yoursite.com/the.pdf">
        <p>This browser does not support PDFs. Please download the PDF to view it: <a href="http://yoursite.com/the.pdf">Download PDF</a>.</p>
    </embed>
</object> 