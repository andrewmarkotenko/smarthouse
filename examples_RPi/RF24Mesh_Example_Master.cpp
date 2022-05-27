 /** RF24Mesh_Example_Master.ino by TMRh20
  * 
  * Note: This sketch only functions on -Arduino Due-
  *
  * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
  * will receive all data from sensor nodes.
  *
  * The nodes can change physical or logical position in the network, and reconnect through different
  * routing nodes as required. The master node manages the address assignments for the individual nodes
  * in a manner similar to DHCP.
  *
  */
  
#include <time.h>
#include "RF24Mesh/RF24Mesh.h"  
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

int socket_open(int *socket_desc);
int socket_msg(int *socket_desc, const char *msg);

RF24 radio(RPI_V2_GPIO_P1_15,RPI_V2_GPIO_P1_24);
RF24Network network(radio);
RF24Mesh mesh(radio,network);
int socket_desc;

void _log(const char *msg){
    printf(msg);
    socket_msg(&socket_desc, msg);
}

int main(int argc, char** argv) {

	struct timespec gettime_now;
    uint32_t displayTimer = 0;
    uint32_t ctr = 0;
    struct payload_t {
      unsigned long ms;
      unsigned long counter;
    };
    time_t rawtime;
    struct tm * timeinfo;
    int prev_node_id;
    bool is_point=false;
    int res;
    char msg[1024];

    // Set the nodeID to 0 for the master node
    mesh.setNodeID(0);

    // Connect to the mesh
    _log("Start Mesh Coordinator\n");
    mesh.begin();
    radio.printDetails();
    socket_open(&socket_desc); 

    while(1){
      
        // Call network.update as usual to keep the network updated
        mesh.update();

        // In addition, keep the 'DHCP service' running on the master node so addresses will
        // be assigned to the sensor nodes
        mesh.DHCP();
  
        // Check for incoming data from the sensors
        while(network.available()){
            //    _log("rcv\n");
            RF24NetworkHeader header;
            network.peek(header);
    
            uint32_t dat=0;
            int node_id;
            switch(header.type){
            // Display the incoming millis() values from the sensor nodes
                case 'M': network.read(header,&dat,sizeof(dat)); 
                    node_id = mesh.getNodeID(header.from_node);
                    if (node_id==prev_node_id) {
                         _log(".");
                         fflush(stdout);
                        is_point=true;
                    } else {
                        if(is_point){
                            _log("\n");
                            is_point=false;
                        }
                        sprintf(msg, "Recv NodeID[%d] payload -> %u\n", node_id, dat);
                        _log(msg);
                    }
                    prev_node_id = node_id;
                    break;
                default:  network.read(header,0,0); 
                    sprintf(msg, "Recv NodeID[%d] bad type -> %d\n", mesh.getNodeID(header.from_node), header.type); 
                    _log(msg);
                    break;
            }
        }

        delay(2);
        clock_gettime(CLOCK_REALTIME, &gettime_now);

        // Send each node a message every five seconds
        // Send a different message to node 1, containing another counter instead of millis()
        if (millis() - displayTimer > 5000) {
          ctr++;
          for (int i = 0; i < mesh.addrListTop; i++) {
            payload_t payload = {millis(), ctr};
            if (mesh.addrList[i].nodeID == 3) {  //Searching for node one from address lista
                time(&rawtime);
                timeinfo = localtime ( &rawtime );
                payload = {0b0000000000000000, ctr};
                if (timeinfo->tm_hour == 6) payload = {0b0000000000001000, ctr};
                if (timeinfo->tm_hour == 7) payload = {0b0000000000001000, ctr};
                RF24NetworkHeader header(mesh.addrList[i].address); //Constructing a header
                res = network.write(header, &payload, sizeof(payload));
                if(res==1){
                        if(is_point){
                            _log("\n");
                            is_point=false;
                        }
                    sprintf(msg, "Send NodeID[%d] res->%d, payload->%lu\n", mesh.addrList[i].nodeID, res, payload.ms);
                    _log(msg);
                }
            }

          }
          displayTimer = millis();
        }

    } // end main while() 
    
    return 0;
}   // end main()
