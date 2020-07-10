import xml.etree.ElementTree as ET
import argparse
import os
import re


def main(assembly_files, num_cycles):

    for assembly_file in assembly_files:

        trace_format = "%1%\t%2%\t%5%\t%6%\t%7%\t%8%\t%9%\t%10%\t%pc%\t%inst%\t%line%\n"

        if ".s" not in assembly_file:
            print("Invalid test name: " + assembly_file + ", skipping test")
            continue
        test_name = assembly_file[:-2] ## eliminates .s at end
        prefix = 'CPU-' + test_name
        ref_output = "./my_tests/circ_files/reference_output/" + prefix + ".out"
        hex_file = "./my_tests/input/" + test_name + ".hex"

        with open("trace_format", "w") as f:
        	f.write(trace_format)

        if not os.path.exists("my_tests/circ_files/reference_output"):
            os.makedirs("my_tests/circ_files/reference_output")
        if not os.path.exists("my_tests/input"):
            os.makedirs("my_tests/input")

        # Creates ref file
        if num_cycles == -1:
            os.system("java -jar venus-jvm-latest.jar -t -tf trace_format -ti -tw -ts -r " + assembly_file + " > " + ref_output)
        else:
            os.system("java -jar venus-jvm-latest.jar -t -tf trace_format -ti -tw -ts -r -tn " + str(num_cycles + 1) + " " + assembly_file + " > " + ref_output)
        os.system("rm -f trace_format")

        # Creates hex file
        os.system("java -jar venus-jvm-latest.jar -d " + assembly_file + " > " + hex_file)
        os.system("cp " + assembly_file + " my_tests/input/")

        ### CLEANS UP REFERENCE OUTPUT AND GETS NUM_CYCLES
        with open(ref_output, "r+") as f:
            out = f.read()
            out = re.sub("\n\n", "\n", out)
            if num_cycles == -1:
                num_cycles = len(out.split("\n")) - 2 # gotta account for extra new lines
            f.seek(0)
            f.write(out)
            f.truncate()

        ### FORMATS HEX FOR INPUTTING INTO CIRCUIT
        instructions = ""
        with open(hex_file, "r") as f:
            nums = f.read().split("\n")
            nums.pop()
            # pop gets rid of extra newlines that venus automatically adds
            for num in nums:
                num = num[2:] # removes 0x as beginning
                instructions += num + " "

            instructions = instructions[:-1] # gets rid of extra space at end
            instructions += "\n"

        ### PUTS TESTS INTO CIRCUIT
        tree = ET.parse('run.circ')
        root = tree.getroot()
        circuit = root.find('circuit')
        ROM = circuit.find("./comp/[@name='ROM']")

        pre = "addr/data: 14 32\n"
        result = pre + instructions

        ROM[2].text = result

        ### PUTS CYLCLE NUM INTO CIRCUIT
        constant = circuit.find("./comp/[@name='Constant']")
        num_cycles_formatted = hex(num_cycles)
        constant[1].attrib['val'] = num_cycles_formatted

        tree.write(prefix + '.circ')

        ### MOVES THINGS WHERE THEY SHOULD BE
        os.system("mv " + prefix + ".circ my_tests/circ_files/")
        print("Test " + test_name + " created!")

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Creates custom CPU tests.')

    parser.add_argument("tests", nargs = "+", help="Which tests you want to create")
    parser.add_argument("-n", type = int, default = -1, help = "How many cycles to simulate the CPU for (default is however long until your code terminates)")

    args = parser.parse_args()

    main(args.tests, args.n)
