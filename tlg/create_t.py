import argparse
from collections import OrderedDict


def Transition(istate:int, ostate:int, ilabel:str, olabel:str) -> str:
    return ('%d\t%d\t%s\t%s\n' % (istate, ostate, ilabel, olabel))

def ExitState(state:int) -> str:
    return ('%d\n' % state)


def main():
    parser = argparse.ArgumentParser('Create token FST')
    parser.add_argument('--tokens', type=str, default='/dev/stdin',
                        help='''path to token file; if not provided, read from stdin
                        the first token must be the blank symbol, usually <blk>
                        the second token must be the space symbol, usually <sp>
                        the rest will be regular tokens, such as a, b, c, ..., z
                        each of the tokens must not contain a space character
                        each token must be separated by a whitespace
                        ''')
    parser.add_argument('--output_prefix', type=str, default='tokens',
                        help='output prefix to write its txt and syms files')
    args = parser.parse_args()

    tokens = open(args.tokens).read().split()
    if len(tokens) <= 2:
        raise Exception("must provided at least one non-blank non-space token")

    token_map = OrderedDict()
    eps = '<eps>'
    phi = '<phi>' # backoff
    token_map[eps] = eps
    token_map[phi] = phi
    token_map['<blk>'] = tokens[0]
    token_map['<space>'] = tokens[1]
    for i in range(2, len(tokens)):
        token_map[tokens[i]] = tokens[i]

    regular_tokens = tokens[2:]
    blk = tokens[0]
    space = tokens[1]

    with open(args.output_prefix + '.syms', 'w') as f:
        for idx,(k,v) in enumerate(token_map.items()):
            f.write('%s\t%d\n' % (v,idx))

    with open(args.output_prefix + '.txt', 'w') as f:
        # state 0 is the initial state but also an exist state
        start = 0
        f.write(ExitState(start))
        f.write(Transition(start, start, blk, eps))
        current = 1
        # any regular token will transition to its own nb state
        for token in regular_tokens:
            f.write(Transition(start, current, token, token))
            # each repetitive token will output nothing
            f.write(Transition(current, current, token, eps))
            # any other token will be need to backoff to start
            f.write(Transition(current, start, phi, eps))
            f.write(ExitState(current))
            current += 1
        # space
        f.write(Transition(start, current, space, space))
        # any repetitive space shall not output
        f.write(Transition(current, current, space, eps))
        # any blank shall not matter for space
        f.write(Transition(current, current, blk, eps))
        # any other will back off to start
        f.write(Transition(current, start, phi, eps))
        f.write(ExitState(current))

    with open(args.output_prefix + '_expand.txt', 'w') as f:
        for idx,(k,v) in enumerate(token_map.items()):
            if idx <= 1: continue
            f.write(Transition(0, 0, v, v))
        f.write('0\n')


if __name__ == '__main__':
    main()