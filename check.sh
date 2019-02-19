#!/bin/bash

points=(5 5 5 5 5 10 15 20 20)

function check_task() {
	# @params: $TASK_NUMBER $OUTPUT $REFERENCE
	grep "^-$1" $2 > .tmp.out
	grep "^-$1" $3 > .tmp.ref
	diff -iwBu .tmp.out .tmp.ref &> /dev/null
	return $?
}

function check_test() {
	# @params: $INPUT $OUTPUT $REFERENCE
	INPUT=input/$1
	OUTPUT=output/$(basename $INPUT .in).out
	REFERENCE=ref/$(basename $INPUT .in).out
	echo "=========================="
	echo "Test file "$INPUT
	echo "=========================="
	./$EXE < $INPUT > $OUTPUT
	test_points=0
	for iter in $(seq 0 8); do
		check_task $iter $OUTPUT $REFERENCE
		if [ $? -eq 0 ]; then
			echo -e "\ttask $iter: ok"
			test_points=$(($test_points + ${points[$iter]}))
		else
			echo -e "\ttask $iter: failed"
		fi
	done
	return $test_points
}

EXE=ip
TESTING=checker
make clean
if [ $? -ne 0 ]; then
	exit 1
fi
make build
if [ $? -ne 0 ]; then
	exit 1
fi
if [ ! -f $EXE ]; then
	echo "The executable name must be $EXE."
	exit 1
fi

TOTAL=0
mkdir -p output
for i in $(ls input); do
	check_test $i
	TOTAL=$(($TOTAL+$?))
done
echo "=============="
echo "TOTAL: "$TOTAL"/900"
echo "=============="
