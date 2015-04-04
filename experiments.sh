#slave <NUM_THREADS> <COMPRESS_LEVEL> <NUM_SEGMENTS> <NVALUES> <SEGMENT_TETA> <NUM_ROWS_R> <NUM_ROWS_S>


echo "Uniform, compress 4 level, 60 threads"
./slave.mic 60 4 60 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 200 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 600 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 2000 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 6000 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 20000 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 60000 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 3000 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 1500 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 1000 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 750 100 0.0 60000 6000000 >> out.txt
./slave.mic 60 4 500 100 0.0 60000 6000000 >> out.txt


echo "80-20, compress 4 level, 60 threads"
./slave.mic 60 4 60 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 200 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 600 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 2000 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 6000 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 20000 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 60000 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 3000 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 1500 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 1000 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 750 100 0.86 60000 6000000 >> out.txt
./slave.mic 60 4 500 100 0.86 60000 6000000 >> out.txt


echo "65-20, compress 4 level, 60 threads"
./slave.mic 60 4 60 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 200 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 600 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 2000 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 6000 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 20000 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 60000 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 3000 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 1500 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 1000 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 750 100 0.73 60000 6000000 >> out.txt
./slave.mic 60 4 500 100 0.73 60000 6000000 >> out.txt


echo "45-20, compress 4 level, 60 threads"
./slave.mic 60 4 60 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 200 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 600 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 2000 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 6000 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 20000 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 60000 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 3000 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 1500 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 1000 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 750 100 0.5 60000 6000000 >> out.txt
./slave.mic 60 4 500 100 0.5 60000 6000000 >> out.txt

