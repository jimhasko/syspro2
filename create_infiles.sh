DAYRANGE=30
MONTHRANGE=12
YEARRANGE=20
baseyear=2000
basemonth=1
baseday=1
namebase=3
namerange=9
agerange=120
patid=1
exitafter=3
charbase=97
charrange=25
diseasesnum=$(wc -l < "$1")
chars=abcdefghijklmnopqrstuvwxyz
e=ENTER
x=EXIT
two=2
one=1
minus1=-1
if [ $4 -lt 0 ]
then
echo files cant be negative

fi

if [ $5 -lt 0 ]
then
echo cases cant be negative

fi



if [ ! -d "$3" ]
then

    mkdir ./$3
    echo " initial directory created"
else
    echo "directory exists"
fi


while IFS= read -r country; do

    cd $3
    echo "Country to be added: $country"
        mkdir ./$country
        cd $country
 array_counter=0
  patient_array=('0')
 ###############for each country
        for i in `seq 1 $4`
        do


        day=$RANDOM
        let "day %= $DAYRANGE"

        month=$RANDOM
        let "month %= $MONTHRANGE"

        year=$RANDOM
        let "year %= $YEARRANGE "

        dayf=$(($day + $baseday))
        monthf=$(($month + $basemonth))
        yearf=$(($year + $baseyear))
          touch "$dayf"-"$monthf"-"$yearf"

############for eatch file


         for j in `seq 1 $5`
          do

            enter_exit=$RANDOM
            let "enter_exit %= $two"

            if [ "$array_counter" -eq "0"  -o "$enter_exit" -eq "0" ]
          #  if [ ${patient_array[0]} -eq "0" -o "$enter_exit" -eq "0" ];
            then
            #  disease=$RANDOM
            #  echo "got in test :$test i:$i  id:$patid array : ${patient_array[i-1]}"
          #  fi
            disease=$RANDOM
            let "disease %= $diseasesnum"
            disease=$(($disease + $one))
            name=$RANDOM
            let "name %= $namerange"
            name=$(($name + $namebase))
            surname=$RANDOM
            let "surname %= $namerange"
            surname=$(($surname + $namebase))
          #  char=$RANDOM
          age=$RANDOM
          let "age %= $agerange"






              namef="${chars:RANDOM%${#chars}:1}"
                for h in `seq 1 $name`
                do
                    namef="${namef}${chars:RANDOM%${#chars}:1}"
                  done

              surnamef="${chars:RANDOM%${#chars}:1}"
                for h in `seq 1 $surname`
                do
                  surnamef="${surnamef}${chars:RANDOM%${#chars}:1}"
                done


                cd ..
                cd ..
                virus=$(head -n $disease $1 | tail -1)
                cd $3
                cd $country
                patient=" ${namef} ${surnamef} ${virus} ${age}"
                patient_array=("$patid" $patient "${patient_array[@]}")
                patient="$patid $e $patient"
                patid=$(($patid+$one))
                array_counter=$(($array_counter+$one))
            #    echo "num patient:$array_counter  "
              echo "$patient" >> $dayf-$monthf-$yearf
            #  echo "  list elements:${patient_array[array_counter*5]} :: ${patient_array[(array_counter*5)+1]} :: ${patient_array[(array_counter*5)+2]}::${patient_array[(array_counter*5)+3]}::${patient_array[(array_counter*5)+4]}::$array_counter"
            #  echo "  hole list:${patient_array[@]} "
else
  rpat=$RANDOM
  let "rpat %= $array_counter"
  patient="${patient_array[rpat*5]} $x ${patient_array[(rpat*5)+1]} ${patient_array[(rpat*5)+2]} ${patient_array[(rpat*5)+3]} ${patient_array[(rpat*5)+4]}"
  echo "$patient" >> $dayf-$monthf-$yearf
  array_counter=$(($array_counter+$minus1))
 unset patient_array[rpat*5]
 unset patient_array[rpat*5+1]
 unset patient_array[rpat*5+2]
 unset patient_array[rpat*5+3]
 unset patient_array[rpat*5+4]
patient_array=( "${patient_array[@]}" )

fi

          done
          ############for eatch file
done
#echo "done with country "
cd ..
    cd ..
    done < $2
