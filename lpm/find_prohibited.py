"""
Given subwords vocabulary, find all sequences that are not allowed
"""

import io
import sys
from typing import List

import numpy as np
from tqdm import tqdm

memo = {}
max_vocab_length = 0


class Node:
    def __init__(self):
        self.children = {}

class PrefixTree:
    def __init__(self, vocab:set):
        self.root = self.build(vocab)

    def build(self, vocab:set) -> Node:
        chars = set(s[0] for s in vocab)
        for c in chars:


def startswith(prefix:str, vocab:set) -> bool:
    for subword in vocab:
        if subword.startswith(prefix): return True
    return False


def prohibited_seq(vocab:set, prefix='') -> List[List[str]]:
    result = []
    if prefix in memo: return memo[prefix]
    for word in tqdm(vocab, disable=len(prefix) > 0):
        target = prefix + word
        if len(target) > max_vocab_length: continue
        if target in vocab and len(prefix) > 0:
            result.append([word])
        if startswith(target, vocab):
            for seq in prohibited_seq(vocab, target):
                result.append([word] + seq)

    memo[prefix] = result
    return result


if __name__ == '__main__':
    not_valid = {'<s>', '</s>', '<eps>', '<epsilon>', '<unk>'}
    vocab = set()
    with io.TextIOWrapper(sys.stdin.buffer, encoding='utf-8') as stdin:
        for line in stdin:
            word = line.strip()
            if '<' in word or word in not_valid:
                continue
            vocab.add(word)

    max_vocab_length = np.max([len(s) for s in vocab])
    seq = prohibited_seq(vocab)
    for s in np.unique(seq):
        assert ''.join(s) in vocab
        if len(s) == 1: continue
        print(' '.join(s))