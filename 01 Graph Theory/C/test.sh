if make
then

start=$(date +%s)

arg="$2"

if [ "$arg" != "old" ]
then
	arg="new"
fi

dir=results/$arg

mkdir -p $dir 2>&1

echo $1 $arg

./threadproc $1 $arg > $dir/result$1.csv &

pid=$!

while kill -0 $pid > /dev/null 2>&1
do
	ps aux | grep $! | grep -v grep | awk 'BEGIN { sum=0 } {sum=sum+$6; } END {printf("Taille RAM utilisée: %s Mo\n",sum / 1024)}' 
	sleep 1
done

end=$(date +%s)

echo $((end-start))

fi
