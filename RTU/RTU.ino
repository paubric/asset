/*
 * RTU v0.2 - Developed by Sturza Mihai
 * Awaits connections from localhost at {ardunio-name}.local/arduino/{command}
 * Prints out information as JSON
 * Avail commands:
 *  - analog/{pin}/(optional - analogWrite){value}
 *  - digital/{pin}/{value}
 *  - mode/{pin}/{input/output}
 *  - all/0
*/

// libraries
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

// creating server object
BridgeServer server;

void printJSON(BridgeClient client, int pin, int value, int stopval) {
  client.print(F("{\"pin\":"));
  client.print(pin);
  client.print(F(",\"value\":"));
  client.print(value);
  if(stopval==-1) {
    client.print(F("}"));
  } else {
    if(pin<stopval) client.print(F("},"));
    else client.print(F("}"));
  }
}

void setup() {
  // starting Bridge
  Bridge.begin();

  // listening on localhost
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // accepting connections
  BridgeClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }
}

void process(BridgeClient client) {
  // getting url input
  String command = client.readStringUntil('/');

  // validating input
  if(command == "analog") {
    analogCommand(client);
  } else if(command == "digital") {
    digitalCommand(client);
  } else if(command == "mode") {
    modeCommand(client);
  } else if(command=="all") {
    allCommand(client);
  }
  
}

void allCommand(BridgeClient client) {
  int value,command;

  command = client.parseInt();
  
  if(client.read()!='/' && command == 0) {
    // printing all digital pins
    client.print(F("{\"digital\":["));
    for(int i = 2;i<=13;++i) {
        value = digitalRead(i);
        printJSON(client,i,value,13);
    }
    client.print(F("],"));
    // printing all analog pins
    client.print(F("\"analog\":["));
    for(int i = 0;i<=5;++i) {
        value = analogRead(i);
        printJSON(client,i,value,5);
      }
    client.print(F("]}"));
    return;
  }
}

void digitalCommand(BridgeClient client) {
  int pin, value;

  pin = client.parseInt();

  // printing all digital pins if input is -1
  client.print(F("{\"digital\":["));
  if(pin==-1) {
    for(int i = 2;i<=13;++i) {
      value = digitalRead(i);
      printJSON(client,i,value,13);
    }
    client.print(F("]}"));
    return;
  }

  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
    // printing in a json friendly format
    printJSON(client,pin,value,-1);
    return;
  } else {
    value = digitalRead(pin);
    // printing in a json friendly format
    printJSON(client,pin,value,-1);
    return;
  }
}

void analogCommand(BridgeClient client) {
  int pin, value;

  pin = client.parseInt();

  // printing all analog pins if input is -1
  client.print(F("{\"analog\":["));
  if(pin==-1) {
    for(int i = 0;i<=5;++i) {
      value = analogRead(i);
      printJSON(client,i,value,5);
    }
    client.print(F("]}"));
    return;
  }
  
  if (client.read() == '/') {
    value = client.parseInt();
    
    // setting analog value
    analogWrite(pin, value);

    // printing in a json friendly format
    printJSON(client,pin,value,-1);
    return;
  } else {
    // reading analog value
    value = analogRead(pin);

    // printing in a json friendly format
    printJSON(client,pin,value,-1);
    return;
  }
}

void modeCommand(BridgeClient client) {
  int pin;

  pin = client.parseInt();

  // encountered an error
  if (client.read() != '/') {
    // printing in a json friendly format
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.println(F(",\"value\":\"error\"}"));
    return;
  }

  String mode = client.readStringUntil('\r');

  // setting to input mode
  if (mode == "input") {
    pinMode(pin, INPUT);
    // printing in a json friendly format
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.print(F(",\"value\":\"input\"}"));
    return;
  }

  // setting to output mode
  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // printing in a json friendly format
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.print(F(",\"value\":\"output\"}"));
    return;
  }
  // encountered invalid mode
    // printing in a json friendly format
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.println(F(",\"value\":\"invalid\"}"));
}
