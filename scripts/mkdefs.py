import sys

def write2doth(f, content):

    towrite = ["//Generated by mkdefs.py","#ifndef _DEFS_H_","#define _DEFS_H_"]
    counter = 0
    for x in content:
        if counter % 2 == 0:
            towrite.append("#define " + x)
        else:
            towrite[-1] = towrite[-1] + "\t(" + x + ")"
        counter = counter + 1
    towrite.append("#endif")


    for x in towrite:
        f.write(x)
        f.write("\n")

def write2dots(f, content):
    towrite = [";Generated by mkdefs.py"]
    counter = 0
    for x in content:
        if counter % 2 == 0:
            towrite.append(x)
        else:
            towrite[-1] = towrite[-1] + "\tequ\t" + x 
        counter = counter + 1


    for x in towrite:
        f.write(x)
        f.write("\n")

    pass



if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("too less cmd arg")
        exit(1)
    
    content = ""
    with open(sys.argv[1]) as f:
        content = f.read().split()
    fouts = sys.argv[2:]

    for fn in fouts:
        f = open(fn, "w+")
        if fn.endswith(".s"):
            write2dots(f, content)
        elif fn.endswith(".h"):
            write2doth(f, content)
        f.close()
    
    exit(0)


    
            