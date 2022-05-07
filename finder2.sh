#! /bin/bash

#tld=$1
amount_of_arguments=$#

echo "total amount of arguments is: $amount_of_arguments"
#echo "given tld is: $tld "

#grep --include=\*.out -rnw '.' -e $tld
#i=1
#while [ $i -le $amount_of_arguments ]
#do 
#    tld=$i
#    echo "TLD is $tld"
#    ((i++))
#done

for TLD in "$@"
do
    echo "TLD is $TLD"
    counter=0
    # find all .out files in current directory
    outfiles=`ls ./*.out`
    for eachfile in $outfiles
    do
        #echo $eachfile
        # for each file, read the lines and store columns in variables
        while IFS=" " read -r location num_of_appearances; do
            #echo "Text read from $eachfile is: $location $num_of_appearances"
            if [[ "$location" == *"$TLD"* ]]; # if tld matches as substring in url
            then
                echo "tld found @ file $eachfile line: $location"
                let counter=counter+1 # yet another appearance in file
            fi
        done < $eachfile
    done
    echo "TLD $TLD found $counter total times."
done

