import twitter4j.conf.*;
import twitter4j.*;
import twitter4j.auth.*;
import twitter4j.api.*;
import java.util.*;

import processing.serial.*;
Serial myPort;

Twitter twitter;
long i = 0;
float value;

void setup()
{
    size(800,600);
    
    // List all the available serial ports:
    printArray(Serial.list());
    // First port [0] in serial list is usually Arduino, but *check every time*:
    String portName = Serial.list()[0];
    myPort = new Serial(this, portName, 9600);
    // don't generate a serialEvent() until getting a newline character:
    myPort.bufferUntil('\n');

    ConfigurationBuilder cb = new ConfigurationBuilder();
    cb.setOAuthConsumerKey("aW0c4ZRktv0DRjlPBJ1WCWenE");
    cb.setOAuthConsumerSecret("LcxzcNqOeH1UYdWK3nmNMhoFEYnFN9ZLpnw1SqXT3yLuytbur8");
    cb.setOAuthAccessToken("710166322907373569-bvkvV865MG02oqtzyLlZRNLqorZCav7");
    cb.setOAuthAccessTokenSecret("d7UUybjwGJ9Xjg4QWJ9U3xHP2edmfOel48FXqyKZ3tAOh");

    TwitterFactory tf = new TwitterFactory(cb.build());

    twitter = tf.getInstance();
}

void draw()
{

}

void tweet()
{
    try
    {  
      if(value < 30){
        
        Status status = twitter.updateStatus("An object is approaching! Distance: " + value);
        System.out.println("Status updated to [" + status.getText() + "].");
      }
    }
    catch (TwitterException te)
    {
        System.out.println("Error: "+ te.getMessage());
    }
    i++;
}

void keyPressed()
{
    tweet();
}

void serialEvent(Serial myPort) { 
    try {
      // get the ASCII string:
      String inString = myPort.readStringUntil('\n');
      //println("raw: \t" + inString); // Used for debugging
  
      if (inString != null) {
        // trim off any whitespace:
        inString = trim(inString);
  
        // split the string on the delimiters and convert the resulting substrings into an float array:
        value = float(inString); // delimiter can be comma space or tab
        println(value); 
         
      }
    }
    catch(RuntimeException e) {
      // only if there is an error:
      e.printStackTrace();
    }
}