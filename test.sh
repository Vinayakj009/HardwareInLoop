rm $1;
for i in {1..4}; do
   echo "Senor Log For Sensor $i" >> $1;
   Inc=$(( $j * $2 ));
   for j in {0..$3}; do
     out=$(( $j * $4 ));
     out=$(( $out + $Inc ));
     echo $out >> $1;
    done;
done;
