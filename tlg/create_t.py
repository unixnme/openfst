import argparse
from collections import OrderedDict


def Transition(istate:int, ostate:int, ilabel:str, olabel:str) -> str:
    return ('%d %d %s %s\n' % (istate, ostate, ilabel, olabel))

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
    parser.add_argument('--output', type=str, default='tokens.txt',
                        help='output fst in text format')
    args = parser.parse_args()

    tokens = open(args.tokens).read().split()
    if len(tokens) <= 2:
        raise Exception("must provided at least one non-blank non-space token")

    token_map = OrderedDict()
    token_map['<blk>'] = tokens[0]
    token_map['<space>'] = tokens[1]
    for i in range(2, len(tokens)):
        token_map[tokens[i]] = tokens[i]

    for idx,(k,v) in enumerate(token_map.items()):
        print('%d: %s --> %s' % (idx, k, v))

    regular_tokens = tokens[2:]
    blk = tokens[0]
    space = tokens[1]
    eps = '<eps>'
    rho = '<rho>' # backoff
    with open(args.output, 'w') as f:
        # state 0 is the initial state but also an exist state
        start = 0
        f.write(ExitState(start))
        current = 1
        # any regular token will transition to its own no blank state
        for token in regular_tokens:
            nb_state = current
            b_state = current + 1
            f.write(Transition(start, nb_state, token, token))
            # blank will transition into its blank state
            f.write(Transition(nb_state, b_state, blk, eps))
            # repetitive token will land on itself for nb_state
            f.write(Transition(nb_state, nb_state, token, eps))
            # blank token will land on itself for b_state
            f.write(Transition(b_state, b_state, blk, blk))
            # all other token will be directed to start
            f.write(Transition(nb_state, start, rho, eps))
            f.write(Transition(b_state, start, rho, eps))
            current += 2
        # space
        f.write(Transition(start, current, space))



if __name__ == '__main__':
    main()