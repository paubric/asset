#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

BridgeServer server;

void setup() {
  pinMode(13, OUTPUT);
  
  Bridge.begin();
  
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  BridgeClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }
   delay(50);
}

void process(BridgeClient client) {
  String command = client.readStringUntil('/');
  
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
    analogWrite(pin, value);
    
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.print(F(",\"value\":"));
    client.print(value);
    client.print(F("}"));
    
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  } else {
    value = analogRead(pin);

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
  
  if (client.read() != '/') {
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.println(F(",\"value\":\"error\"}"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.print(F(",\"value\":\"input\"}"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    client.print(F("{\"pin\":"));
    client.print(pin);
    client.print(F(",\"value\":\"output\"}"));
    return;
  }

    client.print(F("{\"pin\":"));
    client.print(pin);
    client.println(F(",\"value\":\"invalid\"}"));
}
