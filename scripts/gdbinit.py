import gdb
import os
from elftools.elf.elffile import ELFFile
from os import listdir
from os.path import isfile, join, isdir


def getSectionAddr(filename, section_name):
    with open(filename, 'rb') as f:
        elffile = ELFFile(f)
        dot_text = elffile.get_section_by_name(section_name)
        if not dot_text:
            print('WARNING: The section:\"{}\" dose not exists in file:\"{}\".'.format(section_name, filename))
            return 0
        
        return dot_text['sh_addr']

def myGDBAutoLoadSymFileWithOffset(filename, offset):
    addr = getSectionAddr(filename, '.text')
    gdb.execute('add-symbol-file ' + filename + ' ' + hex(addr + offset))

def myGDBAutoLoadSymFile(filename):
    myGDBAutoLoadSymFileWithOffset(filename, 0)



def main():
    # get project full path
    project_path = os.getenv('DOTKERNEL_PATH')
    # fixed setup
    gdb.execute('target remote localhost:1234')
    gdb.execute('set disassembly-flavor intel')


    # symbol file load
    symbol_file_dir = join(project_path, 'debug')
    for f in listdir(symbol_file_dir):
        full_path = join(symbol_file_dir, f)
        if(isfile(full_path)):
            myGDBAutoLoadSymFile(full_path)
    
    # source directory load
    for f in listdir(project_path):
        full_path = join(project_path, f)
        if isdir(full_path):
            temp_cmd = 'dir ' + full_path
            print(temp_cmd)
            gdb.execute(temp_cmd)


    gdb.execute('b _start')

if __name__ == "__main__":
    main()
