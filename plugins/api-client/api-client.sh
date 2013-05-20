#!/bin/bash
TOKEN="65897756"
USERNAME="test2"
TIMEOUT="5"
RESIZE="" #width x height
/bin/cat list.txt | while read IMAGE
do
echo $IMAGE
#/usr/bin/curl --connect-timeout $TIMEOUT --form userfile=@$IMAGE --form token=$TOKEN --form username=$USERNAME --form resize=$RESIZE http://api.us.visqua.com -o revo.$IMAGE
/usr/bin/curl --connect-timeout $TIMEOUT --form userfile=@$IMAGE --form token=$TOKEN --form username=$USERNAME --form resize=$RESIZE http://api.vn.visqua.com -o $1revo.$IMAGE
done
