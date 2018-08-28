// libraries
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

// creating server object
BridgeServer server;

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
  }
  
}

void digitalCommand(BridgeClient client) {
  int pin, value;

  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  } else {
    value = digitalRead(pin);
  }

  // printing in a json friendly format
  client.print(F("{\"pin\":"));
  client.print(pin);
  client.print(F(",\"value\":"));
  client.print(value);
  client.print(F("}"));

  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(BridgeClient client) {
  int pin, value;

  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    
    // setting analog value
    analogWrite(pin, value);

    // printing in a json friendly format
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.print(F(",\"value\":"));
    client.print(value);
    client.print(F("}"));
    
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  } else {
    // reading analog value
    value = analogRead(pin);

    // printing in a json friendly format
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.print(F(",\"value\":"));
    client.print(value);
    client.print(F("}"));
    
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
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
