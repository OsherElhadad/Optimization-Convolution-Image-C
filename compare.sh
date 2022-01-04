#!/bin/bash

DEFAULT_RUN_COUNT=10
TEST_SERVER="planet"

HOST=$(hostname)
timeout 0.25 ls > /dev/null 2>/dev/null
if [ $? == 0 ]
then
	SLEEP_PERIOD=0.25
else
	SLEEP_PERIOD=1
fi

NO_FILTER_PARAM="1"
FILTER_PARAM="2"

NO_FILTER_RESULTS=0
FILTER_RESULTS=0

TEMP_FILE=".compare_tmp"

ERROR=0

print_ret() {
	if [ $1 -eq 0 ]
	then
		echo "✅ Passed"
	else
		echo "❌ Failed"
		ERROR=1
	fi
}

run_program() {
	echo "Running with parameters: $1 $2"
	timeout $SLEEP_PERIOD stdbuf -oL ./showBMP "$1" $2 > $TEMP_FILE
	time_took=`cat $TEMP_FILE | grep "Total runtime:" | cut -d " " -f3`

	echo "Command took $time_took ms"
	if [ $2 == $NO_FILTER_PARAM ]
	then
		NO_FILTER_RESULTS=$(echo "$NO_FILTER_RESULTS + $time_took" | bc)
	elif [ $2 == $FILTER_PARAM ]
	then
		FILTER_RESULTS=$(echo "$FILTER_RESULTS + $time_took" | bc)
	fi

	if [ "$NO_FILTER_RESULTS" == "" -o "$FILTER_RESULTS" == "" ]
	then
		ERROR=1
	fi
}

RUN_COUNT=$1

if [ "$RUN_COUNT" == "" ]
then
	RUN_COUNT=$DEFAULT_RUN_COUNT
fi

echo "REBUILDING..."
make clean
make

echo "Running test $RUN_COUNT time(s)..."

for i in `seq 1 $RUN_COUNT`
do
	rm Blur.bmp Filtered_Blur.bmp Filtered_Sharpen.bmp Sharpen.bmp
	run_program gibson_500.bmp $NO_FILTER_PARAM

	echo "==== Checking Blur ===="
	cmp Blur.bmp Blur_correct.bmp
	print_ret $?
	printf "\n"

	echo "==== Checking Sharpen ===="
	cmp Sharpen.bmp Sharpen_correct.bmp
	print_ret $?
	printf "\n"

	run_program gibson_500.bmp $FILTER_PARAM

	echo "==== Checking Filtered + Blur ===="
	cmp Filtered_Blur.bmp Filtered_Blur_correct.bmp
	print_ret $?
	printf "\n"

	echo "==== Checking Filtered + Sharpen ===="
	cmp Filtered_Sharpen.bmp Filtered_Sharpen_correct.bmp
	print_ret $?
	printf "\n"

done

echo "======== RUNTIMES ======="
printf "With filter average:\t$(echo "scale=6; $FILTER_RESULTS / $RUN_COUNT" | bc) ms\n"
printf "No filter average:\t$(echo "scale=6; $NO_FILTER_RESULTS / $RUN_COUNT" | bc) ms\n"

if [ "$RUN_COUNT" == 20 -a "$HOST" == "$TEST_SERVER" -a "$ERROR" == 0 ]
then
	with_filter=$(echo "$FILTER_RESULTS / $RUN_COUNT" | bc)
	no_filter=$(echo "$NO_FILTER_RESULTS / $RUN_COUNT" | bc)
	echo "Uploading results to forms... ($with_filter,$no_filter)"
	curl 'https://docs.google.com/forms/u/0/d/e/1FAIpQLScIN4njh40n6osL4Ag4qJf5RsS88_q6NqEHYwWJeI-Wn1seJw/formResponse' \
	  -H 'authority: docs.google.com' \
	  -H 'pragma: no-cache' \
	  -H 'cache-control: no-cache' \
	  -H 'upgrade-insecure-requests: 1' \
	  -H 'origin: https://docs.google.com' \
	  -H 'content-type: application/x-www-form-urlencoded' \
	  -H 'user-agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 11_0_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36' \
	  -H 'accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9' \
	  -H 'sec-fetch-site: same-origin' \
	  -H 'sec-fetch-mode: navigate' \
	  -H 'sec-fetch-user: ?1' \
	  -H 'sec-fetch-dest: document' \
	  -H 'accept-language: en-US,en;q=0.9,he;q=0.8' \
	  --data "entry.214523754=$with_filter&entry.1230630933=$no_filter&entry.214523754_sentinel=&entry.1230630933_sentinel=&fvv=1&draftResponse=%5Bnull%2Cnull%2C%225197060369208800395%22%5D%0D%0A&pageHistory=0&fbzx=5197060369208800395" \
	  --compressed > /dev/null
else
	echo "Results submission to statistics forms is done only for COMPLETELY SUCCESSFUL run count of 20. If you want to submit, run the script as follows: $0 20"
fi

echo "View others results at: https://docs.google.com/spreadsheets/d/1Dq6T_zcLeEfznZ_hyaiy3hMaljXtM9XUwCmvM9xhsxg"
echo "Test script was written by Ron Even"
echo "Thank you for using it!"
rm -f $TEMP_FILE
