#include <ns3/core-module.h>
#include <ns3/flow-monitor-helper.h>
#include <ns3/flow-monitor-module.h>
#include <ns3/friis-spectrum-propagation-loss.h>
#include <ns3/internet-module.h>
#include <ns3/mobility-helper.h>
#include <ns3/network-module.h>
#include <ns3/on-off-helper.h>
#include <ns3/udp-client-server-helper.h>
#include <ns3/wifi-module.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Assignment2");

int main(int argc, char *argv[]) {
    CommandLine cmd;
    double distance = 100;
    // uint32_t packetSize = 1000;
    cmd.AddValue("distance", "distance (m)", distance);
    cmd.Parse(argc, argv);

    // Creating node container and nodes
    NodeContainer nodes;
    nodes.Create(2);
    WifiHelper wifi;
    wifi.SetStandard(WIFI_PHY_STANDARD_80211b);
    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
    // set it to zero; otherwise, gain will be added
    // wifiPhy.Set("RxGain", DoubleValue(-10));

    // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
    wifiPhy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel",
                                   "Frequency", DoubleValue(5e9));

    wifiPhy.SetChannel(wifiChannel.Create());
    // Add an upper mac and disable rate control
    WifiMacHelper wifiMac;
    wifi.SetStandard(WIFI_PHY_STANDARD_80211b);
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode", StringValue("DsssRate1Mbps"),
                                 "ControlMode", StringValue("DsssRate1Mbps"));
    // Set it to adhoc mode
    wifiMac.SetType("ns3::AdhocWifiMac");
    NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);

    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));
    positionAlloc->Add(Vector(distance, 0.0, 0.0));
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);
    InternetStackHelper internet;
    internet.Install(nodes);
    Ipv4AddressHelper ipv4;
    NS_LOG_INFO("Assign IP Addresses.");
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i = ipv4.Assign(devices);
    Ipv4Address serverAddress = Ipv4Address(i.GetAddress(1));
    // Ipv4Address clientAddress = Ipv4Address(i.GetAddress(0));
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Ptr<Node> serverNode = nodes.Get(1);
    Ptr<Node> clientNode = nodes.Get(0);
    uint16_t serverPort = 3000;
    // Ptr<Socket> serverSocket = Socket::CreateSocket(serverNode, UdpSocketFactory::GetTypeId());
    // InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), serverPort);
    // serverSocket->Bind(local);
    // serverSocket->Listen();

    UdpServerHelper server(serverPort);
    ApplicationContainer serverApps;
    serverApps = server.Install(serverNode);
    serverApps.Start(Seconds(0.0));
    serverApps.Stop(Seconds(30.0));
    OnOffHelper onoff("ns3::UdpSocketFactory", Address(InetSocketAddress(serverAddress, serverPort)));
    onoff.SetConstantRate(DataRate("1Mbps"), 1024);
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=2]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    ApplicationContainer clientApps = onoff.Install(clientNode);
    clientApps.Start(Seconds(0.0));
    clientApps.Stop(Seconds(30.0));

    Ptr<FlowMonitor> flowMon;
    FlowMonitorHelper flowHelp;
    flowMon = flowHelp.InstallAll();
    flowMon = flowHelp.GetMonitor();

    Simulator::Stop(Seconds(30.0));
    Simulator::Run();
    flowMon->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> flowClassifier = DynamicCast<Ipv4FlowClassifier>(flowHelp.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMon->GetFlowStats();
    std::cout << "Distance: " << distance << "\n";
    for (std::map<FlowId, FlowMonitor::FlowStats>::iterator i = stats.begin(); i != stats.end(); ++i) {
        FlowMonitor::FlowStats flowStats = i->second;
        Ipv4FlowClassifier::FiveTuple t = flowClassifier->FindFlow(i->first);

        std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";

        std::cout << "Flow ID: " << i->first << std::endl;
        std::cout << "Packets sent: " << flowStats.txPackets << std::endl;
        std::cout << "Packets received: " << flowStats.rxPackets << std::endl;
    }
}