#! /bin/sh

build=1

for f in input/*
do
    name=$(echo $f | sed -E "s/input\/(.+)\.in/\1/g")
    rm -f input.S
    cp $f input.S

    java -jar venus-jvm-latest.jar heap.S > output/$name.out
    if test $? -ne 0
    then
        echo "Submission rejected by venus, exiting..."
        build=0
        break
    fi
done

cd output
for f in *.ref
do
    name=$(echo $f | sed -E "s/(.+)\.ref/\1/g")
    if test $build -eq 0
    then
        echo "$name: 0"
    else
        if diff -Z $name.out $name.ref > /dev/null 2> /dev/null
        then
            echo "$name: 1"
        else
            echo "$name: 0"
        fi
    fi
done
