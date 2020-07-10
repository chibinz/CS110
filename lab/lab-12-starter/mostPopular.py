import sys
import re

from pyspark import SparkContext,SparkConf

def splitDocument(document):
    """Returns a list of all words in the document"""
    return re.findall(r"\w+", document[1])

def toPairs(word):
    """ Creates `(key, value)` pairs where the word is the key and 1 is the value """
    return (word, 1)

def sumCounts(a, b):
    """ Add up the values for each word, resulting in a count of occurrences """
    return a + b


def non_article(word):
    if(word[0] in ["a", "an", "the", "THE", "The", "A", "An", "AN"]):
        return False
    else:
        return True

def map2(arg):
    return ("article", (arg[1], arg[0]))

def compare(a, b):
    if(a[0] > b[0]):
        return a
    else:
        return b
""" TODO: Add functions here to determine the most popular word
    Note that Map/flatMap style functions take in one argument while Reduce functions take in two
"""

def mostPopular(file_name, output="spark-wc-out-mostPopular"):
    sc = SparkContext("local[8]", "WordCount", conf=SparkConf().set("spark.hadoop.validateOutputSpecs", "false"))
    """ Reads in a sequence file FILE_NAME to be manipulated """
    file = sc.sequenceFile(file_name)

    counts = file.flatMap(splitDocument) \
                 .map(toPairs) \
                 .reduceByKey(sumCounts) \
                 .sortByKey() \
                 .filter(non_article) \
                 .map(map2) \
                 .reduceByKey(compare)
                 # TODO: add appropriate extra transformations here

    """ Takes the dataset stored in counts and writes everything out to OUTPUT """
    #print(counts)
    counts.coalesce(1).saveAsTextFile(output)

    # print >> open("spark-wc-out-mostPopular/part-00000", "w"), counts

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        mostPopular(argv[1])
    else:
        mostPopular(argv[1], argv[2])
