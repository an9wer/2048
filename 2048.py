import random

def initialize():
    '''Initializes a block with two numbers. 
    The two numbers can be 2 and 2 or 2 and 4 at a random place.
    '''
    initialization = [''] * 16
    a = random.sample(range(16), 2)
    b = random.choice([('2', '2'), ('2', '4')])
    initialization[a[0]] = b[0]
    initialization[a[1]] = b[1]
    return initialization

def combine(extraction, i):
    '''Combines one number with the next same number.

    :param extraction: a list need to be combined
    :param i: the first item in the extraction need to be combined.
    '''
    extraction.pop(i+1)
    extraction[i] = str(int(extraction[i]) * 2)
    return extraction + [''] * (4 - len(extraction))

def magic(column):
    '''Changes the column. it's the basic logic in 2048.

    :param column: the column or row need to be changed.
    '''
    extraction = [] 
    for i in range(4):
        if column[i] != '':
            extraction.append(column[i])
    conversion = extraction + [''] * (4 - len(extraction))
    if (len(extraction) == 4 
            and extraction[0] == extraction[1] 
            and extraction[2] == extraction[3]):
        extraction[2] = str(int(extraction[2]) * 2)
        extraction[0] = str(int(extraction[0]) * 2)
        extraction.pop(3)
        extraction.pop(1)
        conversion = extraction + [''] * 2
    else:
        for i in range(len(extraction)-1):
            if extraction[i] == extraction[i+1]:
                conversion = combine(extraction, i)
                break
    return conversion

def move(block, direction):
    '''Moves the block.

    :param block: a list with all items in the game
    :param direction: the direction we choice to move.
    '''
    if direction == 'h':
        for i in range(0, 16, 4):
            block[i:i+4] = magic(block[i:i+4])
    if direction == 'l':
        for i in range(0, 16, 4):
            tmp = block[i:i+4]
            tmp.reverse()
            tmp = magic(tmp)
            tmp.reverse()
            block[i:i+4] = tmp
    if direction == 'k':
        for i in range(4):
            block[i:16:4] = magic(block[i:16:4])
    if direction == 'j':
        for i in range(4):
            '''
            block[i:16:4].reverse()
            block[i:16:4] = magic(block[i:16:4])
            该种方法时错误的，没有 reverse() 不能对 list 对象做局部反转
            '''
            tmp = block[i:16:4]
            tmp.reverse()
            tmp = magic(tmp)
            tmp.reverse()
            block[i:16:4] = tmp
    return block

def new(block):
    '''Add a new '2' in the block.

    :param block: a list with all items in the game.
    '''
    blank = [i for i in range(16) if block[i] == '']
    if blank:
        a = random.choice(blank)
        block[a] = '2'
    return block

def show(block):
    '''Show the block.

    :param block: a list with all items in the game.
    '''
    print(' ——————————————— ')
    for i in range(0, 16, 4):
        print('|%6s |%6s |%6s |%6s |' % tuple(i for i in block[i:i+4]))
        print(' ——————————————— ')


if __name__ == '__main__':
    block = initialize()
    show(block)

    key = input('please enter the key(h/j/k/l) to move: ')
    '''
    block = [
        '2', '16', '4', '2',
        '8', '32', '2', '8',
        '4', '16', '8', '4',
        '16', '8', '16', '2'
    ]
    '''
    while key != 'q':
        if block.copy() != move(block, key):
            '''Only when the block is changed after the block moves,
            we can add a new '2' in the block.
            '''
            block = move(block, key)
            block = new(block)
        show(block)
        if ('' not in block 
                # 这里必须使用 copy()
                and move(block.copy(), 'k') == move(block.copy(), 'h')):
            break
        key = input('please enter the key(h/j/k/l) to move: ')
    print()
    print('THE END')

