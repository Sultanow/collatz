
if make
then

start=$(date +%s)
echo $start

if [ ! -d results ]
then
	mkdir results
fi

./threadproc $1 > ./results/result.csv

end=$(date +%s)
echo $end
echo $((end-start))

fi
