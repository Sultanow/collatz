if make
then

start=$(date +%s)


if [ ! -d results ]
then
	mkdir results
fi

./threadproc $1 > ./results/result.csv &

pid=$!

while kill -0 $pid > /dev/null 2>&1
do
	ps aux | grep $! | grep -v grep | awk 'BEGIN { sum=0 } {sum=sum+$6; } END {printf("Taille RAM utilisée: %s Mo\n",sum / 1024)}' 
	sleep 1
done

end=$(date +%s)

echo $((end-start))

fi
