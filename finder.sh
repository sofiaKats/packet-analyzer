#! /bin/bash

# code inspired by cgi.di.uoa.gr/~mema/courses/k24/lectures/topic2-bashShellProgramming.pdf

for TLD
do
    echo "Checking all .out files for TLD : $TLD"
    counter=0
    # find all .out files in current directory
    FILE_LIST=`ls ./*.out`
    for eachfile in ${FILE_LIST}
    do
        text=( $(cat "$eachfile") )
        for ((i=0; i <= ${#text[@]} - 1; i=i+2)) #parse text in lines of file
        do  # assign each word to the corresponding value
            location=${text[$i]}; num_of_appearances=${text[$(($i+1))]};
            if [[ "$location" == *"$TLD"* ]];  # if tld matches as substring in url
            then
                echo "tld found @ file $eachfile line: $location"
                let counter=$(($counter+$num_of_appearances)) # yet another appearance in file
            fi
        done
    done
    echo "TLD $TLD found $counter total times."
done