
for i in $(seq 1 5);
do
	nohup ./main > log_10_20/$1_$i.log &
done
