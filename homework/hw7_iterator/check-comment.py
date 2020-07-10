if __name__ == '__main__':
    with open('vector-impl.hpp', 'r') as f:
        lines = f.readlines()
        i = 0
        lines_len = len(lines)
        LoC = 0
        LoComment = 0
        while i < lines_len:
            line = lines[i].lstrip().rstrip()
            if line.find("goto") != -1:
                print("Do not use goto in your code!")
                exit(1)
            elif line.find("#include") != -1:
                print("Do not use include in your code!")
                exit(1)
            elif line == "" or line == "{" or line == "}":
                pass
            elif line.find("//") != -1:
                LoComment += 1
                if line[:2] != "//":
                    LoC += 1
            elif line.find("/*") != -1:
                j = i
                LoComment += 1
                if line[:2] != "/*":
                    LoC += 1
                while line.find("*/") == -1 and i < lines_len:
                    i += 1
                    LoComment += 1
                    line = lines[i].lstrip().rstrip()
                if i > j and line[-2:] != "*/":
                    LoC += 1
            else:
                LoC += 1
            i += 1
        print("Lines of code =", LoC)
        print("Lines of comment =", LoComment)
        if LoComment * 4 < LoC:
            print("You only have {0:.2f}% comments!".format(LoComment / LoC * 100))
            exit(1)
