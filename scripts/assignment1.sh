mkdir /home/kanishk/cn-lab/ns3/assignment-outputs
cd ~/ns3-install/bake/source/ns-3.30
./waf --run "scratch/assignment-1 --latency=10"
./waf --run "scratch/assignment-1 --latency=100"
./waf --run "scratch/assignment-1 --latency=1000"
./waf --run "scratch/assignment-1 --latency=5000"
mkdir /home/kanishk/cn-lab/ns3/assignment-outputs/final
./waf --run "scratch/assignment-1-multipleApps --latency=10"
./waf --run "scratch/assignment-1-multipleApps --latency=100"
./waf --run "scratch/assignment-1-multipleApps --latency=1000"
./waf --run "scratch/assignment-1-multipleApps --latency=5000"
