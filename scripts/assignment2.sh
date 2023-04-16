cd ~/ns3-install/bake/source/ns-3.30
echo "Simulation run at 3 distances: 1m, 100m, 379m and 380m"
echo "Simulation run at 3 distances: 1m, 100m, 379m and 380m" > ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
./waf --run "scratch/assignment-2 --distance=1" >> ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
echo "" >> ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
./waf --run "scratch/assignment-2 --distance=100" >> ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
echo "" >> ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
./waf --run "scratch/assignment-2 --distance=379" >> ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
echo "" >> ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
./waf --run "scratch/assignment-2 --distance=380" >> ~/cn-lab/ns3/assignment-outputs/assignment2-outputs.txt
