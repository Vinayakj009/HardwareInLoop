#!/bin/bash

echo "Processing file $1";
echo "Extracting sensor logs";
cat $1 | grep "Sensor Log For Sensor">temp.txt;
echo "creating new log folder";
filename=$(echo $1 | cut -d '.' -f 1)
mkdir -p $2/$filename/detailed
echo "parsing log"
ride_number=-1
sum=0
IFS=''
while read line
do
	IFS=', ' read -r -a array <<< $line;
	array1=(${array[@]:5:${#array[@]}});
	sensor=${array[4]};
	if [ "$sensor" == "1" ]; then
		ride_number=$(($ride_number +1));
		sum=0
	fi 
	echo "Senor Log For Sensor $sensor">>"$2/$filename/ride$ride_number.txt";
	sum=$(($sum + ${#array[@]}));
	data[$sensor]=${array1[@]};
	printf "%s\n"  ${array1[@]}>>"$2/$filename/ride$ride_number.txt";

	if [ "$sensor" == "4" ]; then
		echo "$ride_number,$sum"
		printf "%s\n"  ${data[@]}>>"$2/$filename/detailed/ride$ride_number.txt";				
	fi
done < temp.txt

