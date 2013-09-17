
rm h5files/youbottest.h5 -v
touch h5files/youbottest.h5 
#./H5FDdsmReceiver_cwrite_cread 10.33.172.170 22000
./H5FDdsmReceiver_cwrite_cread 192.168.10.171 22000
