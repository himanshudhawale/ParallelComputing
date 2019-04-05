#!/bin/sh
KEY_FILE=test_cases.txt

INTENSITIES="1 10 100"
NS="1000 10000 1000000"
FUNCS="1 2 3 4"


echo -n > ${KEY_FILE}

for f in $FUNCS;
do
    for intensity in $INTENSITIES;
    do
        for n in $NS;
        do
#	    echo -n $f\ $n\ $intensity\ >> ${KEY_FILE}
#      ./main ${f} 0 10 ${n} ${intensity} >> ${KEY_FILE}
        echo $f\ $n\ $intensity\ $(./main $f 0 10 ${n} ${intensity} 2> /dev/null)
        done >> ${KEY_FILE}
    done 
done
