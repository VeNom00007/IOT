/*Design a networks to perform end-to-end data transfer of IPv6 Packets over Bluetooth
Low Energy Networks? Create a network topology of 10 Nodes and test to check the
connection between different two UDP clients and to verify the correctness of the
received packets.*/

//207201 ABHISHEK VERMA CSE-B

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleIpv6Example");

int main (int argc, char *argv[]) {
  CommandLine cmd;
  cmd.Parse (argc, argv);

  NodeContainer nodes;
  nodes.Create (10);

  std::cout << "Created 10 nodes" << std::endl;

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  InternetStackHelper internetv6;
  internetv6.SetIpv4StackInstall (false);
  internetv6.Install (nodes);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  for (uint32_t i = 0; i < nodes.GetN () - 1; ++i)
    {
      NetDeviceContainer link = pointToPoint.Install (nodes.Get (i), nodes.Get (i + 1));
      devices.Add (link);
    }

  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer interfaces = ipv6.Assign (devices);

  uint16_t port = 9;
  UdpEchoServerHelper server (port);
  ApplicationContainer apps = server.Install (nodes.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  UdpEchoClientHelper client (interfaces.GetAddress (0,1), port);
  client.SetAttribute ("MaxPackets", UintegerValue (1));
  client.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  client.SetAttribute ("PacketSize", UintegerValue (1024));

  apps = client.Install (nodes.Get (9));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

  
   // Enable pcap and ascii tracing
   AsciiTraceHelper ascii;
   pointToPoint.EnableAsciiAll(ascii.CreateFileStream("scratch/ble-UDP-Client.tr"));
   pointToPoint.EnablePcapAll("scratch/ble-UDP", true);

   // animation using NetAnim
   AnimationInterface anim("scratch/207201_q3.xml");
   int j = 0;
   for(float i=10.0;i<100;i+=20.0){
     anim.SetConstantPosition(nodes.Get(j),i,20.0);
     anim.SetConstantPosition(nodes.Get(9-j),i,60.0);
     j++;
   }

   Simulator::Run ();
   Simulator::Destroy ();

   return 0;
}
