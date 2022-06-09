#ThePackage
import random

#To derive the lat coord, it needs to be decoded with 4 possible operations (in any order)

starting = -0.841600711036728

def calculate():
    # fill the list again
    operations = ['add', 'sub', 'mult', 'div']
    # grab our starting number
    num = starting
    # shuffle the order of operations
    random.shuffle(operations)
    print('* Attempting order {}'.format(', '.join(operations)))
    while operations:
        action = operations.pop(0)
        # Add the sixth fibonacci value, zero inclusive => 5
        if action == 'add':
            num = num + 5
        # Subtract value equivalent to Pi (2 decomails) => 3.14
        elif action == 'sub':
            num = num - 3.14
        # Multiply by the number multiple of the first 5 digits (do not simplify)
        elif action == 'mult':
            num = num * (1*2*3*4*5)
        # Divided by the second prime number => 3
        elif action == 'div':
            num = num / 3
    return "{:.14f}".format(num) # discord chats reveal the lat has 14 decimal places


# the hint said our answer will start with 4, so loop the operations until we get 4 or 40
while True:
    answer = calculate()
    print('** Answer: {}'.format(float(answer)))

    # coordinates are -180, to 180 inclusive. Also we know the first digit is a 4 (not -4)
    if -180 <= float(answer) <= 180 and answer[0] == '4':
        break;

