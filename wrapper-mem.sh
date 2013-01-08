#!/bin/bash



if [ -e "multicore-mem-config.txt" ]
then
    rm multicore-mem-config.txt
fi

echo " [CacheGeometry geo-l1]
Sets = 256
Assoc = 2
BlockSize = 64
Latency = 2
Policy = LRU
Ports = 2

[CacheGeometry geo-l2]
Sets = 512
Assoc = 4
BlockSize = 64
Latency = 20
Policy = LRU
Ports = 4

[Module mod-l1-0]
Type = Cache
Geometry = geo-l1
LowNetwork = net-l1-l2 
LowModules = mod-l2

[Module mod-l1-1]
Type = Cache
Geometry = geo-l1
LowNetwork = net-l1-l2 
LowModules = mod-l2

[Module mod-l2]
Type = Cache
Geometry = geo-l2
HighNetwork = net-l1-l2 
LowNetwork = net-l2-mm
LowModules = mod-mm

[Module mod-mm]
Type = MainMemory
BlockSize = 256
Latency = 200
HighNetwork = net-l2-mm
" >>multicore-mem-config.txt

echo "[Network net-l2-mm]
DefaultInputBufferSize="$(($2*2))" 
DefaultOutputBufferSize="$(($2*2))"
DefaultBandwidth="$2"
">>multicore-mem-config.txt

echo "[Network net-l1-l2]
DefaultInputBufferSize = 1024 
DefaultOutputBufferSize = 1024
DefaultBandwidth="$2" 
" >> multicore-mem-config.txt

for i in `seq 1 $1`
do

cores=$(($i-1))
echo " 
[Entry core-"$cores"] 
Type = CPU
Core = "$cores"
Thread = 0
DataModule = mod-l1-0
InstModule = mod-l1-0
" >> multicore-mem-config.txt

done