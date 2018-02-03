#!/bin/sh
rm $1;
i=0;
while [ $i -lt 4 ]; do
	i=$(( $i + 1 ));
	Inc=$(( $i * $2 ));
	echo "Senor Log For Sensor $i">>$1;
	j=0;
	while [ $j -lt $3 ]; do
		j=$(( $j +1 ));
		out=$(( $j * $4 ));
		out=$(( $out + $Inc ));
		echo $out >> $1;
	done;
done;
