if make
then

start=$(date +%s)


if [ ! -d results ]
then
	mkdir results
fi

./threadproc $1 > ./results/result.csv &
cat /proc/$! 

end=$(date +%s)

echo $((end-start))

fi
