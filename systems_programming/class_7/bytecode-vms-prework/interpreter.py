import dis  # wait so some things survived the fire?
import marshal
import sys
from pprint import pprint

assert sys.version_info[:2] == (2, 7)


def parse_pyc(f):
    """
    Given a Python 2.7 .pyc file, read the key information and unmarshall and
    return the code object.
    """
    magic_number = f.read(4)
    assert magic_number.encode('hex') == '03f30d0a'
    f.read(4)  # next 4 bytes is the timestamp
    return marshal.load(f)

OP = {
    'NOP': 9,
    'BINARY_SUBSCR_LIST_INT': 20, # todo: implement
    'BINARY_DIVIDE': 21,
    'BINARY_MODULO': 22,
    'BINARY_ADD': 23,
    'CALL_PY_WITH_DEFAULTS': 24, # todo: implement
    'PRINT_ITEM': 71,
    'PRINT_NEWLINE': 72,
    'RETURN_VALUE': 83,
    'STORE_NAME': 90,
    'LOAD_CONST': 100,
    'LOAD_NAME': 101,
    'COMPARE_OP': 107, 
    'POP_JUMP_IF_FALSE': 114, # todo: implement
    'LOAD_GLOBAL': 116, # todo: implement
    'LOAD_FAST': 124,
    'CALL_FUNCTION': 131,
    'MAKE_FUNCTION': 132,
}

HAVE_ARGUMENT = 90  # opcodes from here on have an argument

class DataStack:
    variables = []
    def __init__(self):
        self.values = []
    def pop(self):
        return self.values.pop()
    def push(self, value):
        self.values.append(value)
    def top(self):
        return self.values[-1]
    def set_top(self, value):
        self.values[-1] = value
    

class FrameStack:
    def __init__(self):
        self.values = []
        self.push()
    def pop(self):
        return self.values.pop()
    def push(self):
        stack = DataStack()
        self.values.append(stack)
    def top(self):
        return self.values[-1]


class VM:
    def __init__(self):
        self.frame_stack = FrameStack()

    def interpret(self, code):
        """
        Given a code object, interpret (evaluate) the code.
        """
        bytecode = code.co_code
        pc = 0
        names = code.co_names
        consts = code.co_consts
        locals = dict()
            
        while True:
            try:
                opcode = ord(bytecode[pc])
                pc += 1
            except StopIteration:
                break
            if opcode >= HAVE_ARGUMENT:
                oparg = ord(bytecode[pc]) + (ord(bytecode[pc+1]) << 8)
                pc += 2

            if opcode == OP['NOP']:
                continue
            elif opcode == OP['BINARY_DIVIDE']:
                x = self.frame_stack.top().pop() 
                y = self.frame_stack.top().top()
                z = y / x
                self.frame_stack.top().set_top(z)
            elif opcode == OP['BINARY_MODULO']:
                x = self.frame_stack.top().pop()
                y = self.frame_stack.top().top()                
                z = y % x
                self.frame_stack.top().set_top(z)
            elif opcode == OP['BINARY_ADD']:
                x = self.frame_stack.top().pop()
                y = self.frame_stack.top().top()
                z = x + y
                self.frame_stack.top().set_top(z)
            elif opcode == OP['CALL_PY_WITH_DEFAULTS']:
                continue
            elif opcode == OP['PRINT_ITEM']:
                sys.stdout.write(str(self.frame_stack.top().pop()))
                sys.stdout.flush()
            elif opcode == OP['PRINT_NEWLINE']:
                print
            elif opcode == OP['RETURN_VALUE']:
                return self.frame_stack.top().pop()

            # Codes after this point have arguments
            
            elif opcode == OP['STORE_NAME']:
                name = names[oparg]
                x = self.frame_stack.top().pop()
                locals[name] = x
                self.frame_stack.top().push(name)
            elif opcode == OP['LOAD_CONST']:
                self.frame_stack.top().push(consts[oparg])
            elif opcode == OP['LOAD_NAME']:
                name = names[oparg]
                x = locals[name]
                self.frame_stack.top().push(x)
            elif opcode == OP['COMPARE_OP']:
                x = self.frame_stack.top().pop()
                y = self.frame_stack.top().pop()
                if (x == y):
                    self.frame_stack.top().push(1)
                else:
                    self.frame_stack.top().push(0)
            elif opcode == OP['POP_JUMP_IF_FALSE']:
                x = self.frame_stack.top().pop()
                if (x == 0):
                    pc = 16
            elif opcode == OP['LOAD_FAST']:
                self.frame_stack.top().push(self.frame_stack.top().variables.pop())
            elif opcode == OP['CALL_FUNCTION']:
                new_vars = []
                for x in range(oparg):
                    new_vars.append(self.frame_stack.top().pop())
                func_code = self.frame_stack.top().pop()
                self.frame_stack.push()
                self.frame_stack.top().variables = new_vars
                value = self.interpret(func_code)
                self.frame_stack.pop()
                self.frame_stack.top().push(value)
            elif opcode == OP['MAKE_FUNCTION']:
                continue
            else:
                print('Unknown opcode {}'.format(opcode))

    def print_code_info(self, code):
        print(code.co_global)
        code = code.co_consts[0]
        bytecode = iter(code.co_code)
        while True:
            try:
                opcode = ord(bytecode.next())
            except StopIteration:
                break
            opname = opcode
            for key, value in OP.items():
                if opcode == value:
                    opname = key
                    break
            if opcode >= HAVE_ARGUMENT:
                oparg = ord(bytecode.next()) + (ord(bytecode.next()) << 8)
                print('%s %s'%(opname, oparg))
            else:
                print(opname)
        if (code.co_consts is not None):
            print('consts: %s'%str(code.co_consts))
        if (code.co_names is not None):
            print('names: %s'%str(code.co_names))
        if (code.co_name is not None):
            print('name: %s'%str(code.co_name))
        if (code.co_name is not None):
            print('name: %s'%str(code))

if __name__ == '__main__':
    """
    Unmarshall the code object from the .pyc file reference as a command
    line argument, and intrepret it.

    Usage: python interpreter.py 1.pyc
    """
    if (len(sys.argv) < 2):
        print("unknown")
    else:
        f = open(sys.argv[1], 'rb')
        code = parse_pyc(f)
        # dis.dis(code)  # helpful for debugging! but kinda cheating

        # PRINTING INFO
        if (len(sys.argv) > 2 and sys.argv[2] == '-p'):
            vm = VM()
            vm.print_code_info(code)


        #EXECUTING CODE
        else:
            print('Interpreting {}...\n'.format(sys.argv[1]))
            vm = VM()
            ret = vm.interpret(code)
            print('\nFinished interpreting, and returned {}'.format(ret))
