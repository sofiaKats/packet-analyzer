#! /bin/bash

tld=$1

echo "given tld is: $tld "

#grep --include=\*.out -rnw '.' -e $tld

# find all .out files in current directory
outfiles=`ls ./*.out`   
for eachfile in $outfiles
do
    #echo $eachfile
    # for each file, read the lines and store columns in variables
    while IFS=" " read -r location num_of_appearances; do
        #echo "Text read from $eachfile is: $location $num_of_appearances"
        if [[ "$location" == *"$tld"* ]]; # if tld matches as substring in url
        then
            echo "tld found @ file $eachfile line: $location"
        fi
    done < $eachfile
done