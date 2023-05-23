#include <ns3/applications-module.h>
#include <ns3/core-module.h>
#include <ns3/flow-monitor-helper.h>
#include <ns3/flow-monitor-module.h>
#include <ns3/internet-module.h>
#include <ns3/network-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/udp-client-server-helper.h>

#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Assignment1");

// @Author: Kanishk Chathley kc294 2010110327
int main(int argc, char *argv[]) {
    uint64_t LATENCY = 650000;
    CommandLine cmd;
    cmd.AddValue("latency", "P2P link Latency in milliseconds", LATENCY);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::MS);
    LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
    LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
    // // Hardware Setup: // //
    // setting up nodes
    NodeContainer nodes;
    nodes.Create(2);
    // Setting up attributes for physical layer
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", TimeValue(MilliSeconds(LATENCY)));
    NetDeviceContainer netDevices1 = p2p.Install(nodes);
    // // Software Setup: // //
    // IP setup
    InternetStackHelper internetStack;
    internetStack.Install(nodes);
    Ipv4AddressHelper ip4Address;
    ip4Address.SetBase("10.1.1.0", "255.255.255.0");
    // Assigning ip address to devices
    Ipv4InterfaceContainer ipv4Interfaces1 = ip4Address.Assign(netDevices1);
    Address serverAddress = Address(ipv4Interfaces1.GetAddress(1));
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    // Setting up server on node 2
    uint16_t serverPort = 3000;
    UdpServerHelper server(serverPort);
    ApplicationContainer serverApps;
    serverApps = server.Install(nodes.Get(1));

    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(12.0));
    // Setting up client
    uint16_t clientPort = 3000;
    UdpClientHelper client(serverAddress, clientPort);

    // client.SetAttribute("MaxPackets", UintegerValue(10));
    client.SetAttribute("PacketSize", UintegerValue(512));
    client.SetAttribute("Interval", TimeValue(MilliSeconds(1)));

    ApplicationContainer clientApps;
    clientApps = client.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(12.0));
    AsciiTraceHelper ascii;
    p2p.EnableAscii(ascii.CreateFileStream("/home/kanishk/cn-lab/ns3/assignment-outputs/Assignment1_c_" + std::to_string(LATENCY) + "ms.tr"), netDevices1);
    p2p.EnablePcap("/home/kanishk/cn-lab/ns3/assignment-outputs/Assignment1_c_" + std::to_string(LATENCY) + "ms.tr", netDevices1, false);
    Ptr<FlowMonitor> flowMon;
    FlowMonitorHelper flowHelp;
    flowMon = flowHelp.InstallAll();
    flowMon = flowHelp.GetMonitor();

    Simulator::Stop(Seconds(12.0));
    Simulator::Run();
    flowMon->CheckForLostPackets();
    // Simulator::Destroy();
    Ptr<Ipv4FlowClassifier> flowClassifier = DynamicCast<Ipv4FlowClassifier>(flowHelp.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats();
    std::ofstream fout;
    flowMon->SerializeToXmlFile("/home/kanishk/cn-lab/ns3/assignment-outputs/Assignment1_c_" + std::to_string(LATENCY) + "ms.xml", true, true);
    std::cout << flowMon->SerializeToXmlString(4, true, true);
    fout.open("/home/kanishk/cn-lab/ns3/assignment-outputs/Assignment1_c_" + std::to_string(LATENCY) + "ms.txt");
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = flowStats.begin(); i != flowStats.end(); i++) {
        Ipv4FlowClassifier::FiveTuple t = flowClassifier->FindFlow(i->first);
        // if ((t.sourceAddress == "10.1.1.1" && t.destinationAddress == "10.1.1.2")) {
        std::cout << "Latency: " << LATENCY << "\n";
        std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
        std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
        std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
        std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 << " Mbps\n";
        fout << "Latency: " << LATENCY << "\n";
        fout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
        fout << "  Tx Bytes:   " << i->second.txBytes << "\n";
        fout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
        fout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 << " Mbps\n";
        // }
    }

    return 0;
}