#!/bin/bash

for bandwidth in {256..4096..256}
do
    for num_cores in {2..20..2}
    do
	./wrapper-mem.sh $num_cores $bandwidth
	for threads in {2..7}
	do
	    num_threads=$((2**$threads))
	    ./wrapper-cpu.sh  $num_cores 1
	    for vector in $(eval echo {$threads..8})
	    do
		vector_size=$((2**$vector))
		echo "Run :  $num_cores $num_threads $vector_size $bandwidth"
		echo "Bandwidth : $bandwidth"
		~/multi2sim-4.0/src/m2s  --x86-config multicore-config --mem-config multicore-mem-config.txt --x86-report cpu-report \
		    --mem-report mem-report --x86-sim detailed vecprod $num_threads $vector_size
	    done  
	done
    done
done

