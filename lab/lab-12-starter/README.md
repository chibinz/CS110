# Lab 12

This repository contains the starter file for Lab 12.

## Environment Setup

This part only contains guide for a few Linux distributions. Feel free to start a merge request if you have tested on other ones. Also, this is only a brief guide and may not include all the details. If you encounter any problems, first search online and then ask on piazza.

### Arch Linux

1. Install [apache-spark](https://aur.archlinux.org/packages/apache-spark/), [hadoop](https://aur.archlinux.org/packages/hadoop/) and [anaconda2](https://aur.archlinux.org/packages/anaconda2/) from AUR.

2. Install [jdk8-openjdk](https://www.archlinux.org/packages/extra/x86_64/jdk8-openjdk/) and set default Java environment to JDK8.

3. Edit `/etc/hadoop/hadoop-env.sh` to tell Hadoop what `JAVA_HOME` is (should be at line 54).
```
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk/jre
```

### Ubuntu 18.04

1. Install [openjdk-8-jdk](https://packages.ubuntu.com/bionic/openjdk-8-jdk).

2. Download binary files for [Spark](https://www.apache.org/dyn/closer.lua/spark/spark-2.4.5/spark-2.4.5-bin-hadoop2.7.tgz) and [Hadoop](https://www.apache.org/dyn/closer.cgi/hadoop/common/hadoop-3.2.1/hadoop-3.2.1.tar.gz).

3. (Optional) Verify the downloads using the checksum.

4. Extract the two tarballs to one location.

5. Add the `bin/` path in Spark and Hadoop to `PATH`.

6. Modify `etc/hadoop/hadoop-env.sh` in Hadoop to tell Hadoop what `JAVA_HOME` is (should be at line 54).
```
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
```

7. Install [Anaconda2](https://repo.anaconda.com/archive/Anaconda2-2019.10-Linux-x86_64.sh).

## Lab Setup

You will be working with Spark (in Python!), so you may need to brush up a bit on your Python! To be able to run Spark, you must create a virtual environment using the correct version of Python. This can be done as such:
```bash
$ conda create --name lab12env python=2.7
```

Respond to the prompt to install packages with “y” (with no quotes). You can (and should) ignore any warnings about conda being out of date. These will take about 30 seconds to install. Finally, run the following command to activate the virtual environment:

```bash
$ conda activate lab12env
```

This will put you in a virtual environment needed for this lab. **Please remember that if you exit the virtual environment and want to return to work on the lab, you must re-run** `conda activate lab12env` **first for Spark to work.**

## Background Information

In lecture we’ve exposed you to cluster computing (in particular, the MapReduce framework), how it is set up and executed, but now it’s time to get some hands-on experience running programs with a cluster computing framework!

In this lab, we will be introducing you to a cluster computing framework called Spark. Spark was developed at Berkeley before being donated to the Apache Software Foundation in 2013. We will be writing Python code to run in Spark to give us some practice in writing Map and Reduce routines.

### Avoid Global Variables

When using Spark, avoid using global variables! This defeats the purpose of having multiple tasks running in parallel and creates a bottleneck when multiple tasks try to access the same global variable. As a result, most algorithms will be implemented without the use of global variables.

### Documentation, and Additional Resources

- A quickstart programming guide for Spark (click the Python tab to see the Python code) is [available here](https://spark.apache.org/docs/latest/rdd-programming-guide.html)!

- The version of Spark we will be using will be 2.4.5 and the link to the API documentation is [available here](https://spark.apache.org/docs/latest/api/python/index.html) (Note that the docs likely say a different version, but the API should be compatible).

## Exercises

Note: Different exercises may be solvable or needed to be solved by reconsidering how `map()`, `flatMap()` and `reduce()` are implemented and called and in which order, so keep this in mind when calling whichever you must use.

The following exercises use sample input files, which can be found in `seqFiles/`.

1. `billOfRights.txt.seq` – the first 10 Amendments of the US constitution split into separate documents (a very small input).

2. `complete-works-mark-twain.txt.seq` – The Complete Works of Mark Twain (a medium-sized input).

Notice the `.seq` extension, which signifies a sequence file that is readable by Spark. These are NOT human-readable. Spark supports other input formats, but you will not need to worry about that for this lab.

The human-readable text file version of each is included in `textFiles/` so you can open those to get a sense of the contents of each file.

Although an exercise may not explicitly ask you to use it, we recommend testing your code on the `billOfRights` data set first in order to verify correct behavior and help you debug.

### Exercise 0: Generating an Input File for Spark

In this lab, we’ll be working heavily with textual data. We have some pre-generated datasets as indicated above, but it’s always more fun to use a dataset that you find interesting. This section of the lab will walk you through generating your own dataset using works from Project Gutenberg (a database of public-domain literary works).

**Step 1:** Head over to [Project Gutenberg](https://www.gutenberg.org/), pick a work of your choosing, and download the “Plain Text UTF–8” version into your lab directory.

**Step 2:** Open up the file you downloaded in your favorite text editor and insert `—END.OF.DOCUMENT—` by itself on a new line wherever you want Spark to split the input file into separate `(key, value)` pairs. The importer we’re using will assign an arbitrary key (like `doc_xyz`) and the value will be the contents of our input file between two `—END.OF.DOCUMENT—` markers. You’ll want to break the work into reasonably-sized chunks, but don’t spend too much time on this part (chapters/sections within a single work or individual works in a body of works are good splitting points).

**Step 3:** Now, we’re going to run our Importer to generate a `.seq` file that we can pass into the Spark programs we’ll write. The importer is actually a MapReduce program, written using Hadoop! You can take a look at `Importer.java` if you want, but the implementation details aren’t important for this part of the lab. You can generate your input file like so:
```bash
$ make generate-input myinput=YOUR_FILE_FROM_STEP_2.txt
```

Your generated `.seq` file can now be found in the `seqFiles/` directory in your lab directory. When you complete other exercise in this lab, run them on your downloaded file as well and investigate the results.

### Exercise 1: Running Word Count

For this exercise you will use the already-completed `wordCount.py`. Open the file and take a look. Make sure you understand what the file is attempting to do.

You can run it on the `billOfRights` text with the following command:
```bash
$ spark-submit wordCount.py seqFiles/billOfRights.txt.seq
```

Where `spark-submit` takes a python file describing a series of map and reduce steps, distributes that files between different worker processors (often across many physical computers, but just across local processors for this lab), and provides the `.seq` file as an input to that python file.

In this case, the command will run `wordCount.py` over `billOfRights.txt.seq`. Your output should be visible in `spark-wc-out-wordCount/part-00000`.

Next, try your the code on the larger input file `complete-works-mark-twain.txt.seq`.
```bash
$ spark-submit wordCount.py seqFiles/complete-works-mark-twain.txt.seq
```

Your output for this command will be located in the same `spark-wc-out-wordCount/part-00000` file (overwriting the previous results). Search through the file for a word like 'the' to get a better understanding of the output.

Exercise 2-4 will be updated later.
