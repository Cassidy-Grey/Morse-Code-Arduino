#define TRACE
#define LEDB 11
#define LEDG 10
#define LEDY 9
#define LEDO 6
#define LEDR 5
#define POTENT A0

//Constant Variables
const int size = 47; //letters in the alphabet

//Global definitions
char plain[size] = "abcdefghijklmnopqrstuvwxyz1234567890.,'?!+-*/= ";
char * dis[size] = {"^","$$+","$+^","+$","$","$+$","++","$++","$$","+$$","+$^","$$^","$+","$^","^+","^$","+^","$^+","$^$","+","^^","++^","++$","+^$","$^^","+++","^$$+","^$$^","^$+$","^$++","^$+^","^$^$","^$^+","^$^^","^+$$","^$$$","^$$","^+$","^$+","^++","^+^","^^$","^^+","^^^","^$^","^+++^"," "};

char * char2dis(char lett){
  #ifdef TRACE
    Serial.print("char2dis active. letter = ");
    Serial.println(lett);
  #endif
  
  //definitions
  char unk[5] = "error";
  int place = -1;
  
  for (int i = 0; i < size; i++){
    if (lett == plain[i]){
      place=i ;
    }
  }
  if (place != -1){
    return dis[place];
  }
  else {
    return unk;
  }
}



String string2dis (String mess){
  #ifdef TRACE
    Serial.print("string2dis active. message = ");
    Serial.println(mess);
  #endif
  
  //definitions
  int len;
  len = mess.length();
  
  String translated = "";
  int i;

  //conversion
  for (i = 0; i < len; i++){
    char low;
    
    low = tolower(mess[i]);
    translated += char2dis(low);
    
    //Line control
    if (( mess[i+1] != " " ) && ( mess[i] != " ") && ((i+1) != len)){
      translated += ("|"); 
    }
  }
  
  return translated;
}



char dis2char(String m){
  #ifdef TRACE
    Serial.print("dis2char active. letter = ");
    Serial.println(m);
  #endif
  
  //definitions
  char unk = '?';
  int place = -1;
  
  for (int i = 0; i < size; i++){
  
    if (m.compareTo(dis[i]) == 0){
      place=i ;
    }
  }
  if (place != -1){
    return plain[place];
  }
  else {
    return unk;
  }
}



String dis2string(String m){
  #ifdef TRACE
    Serial.print("dis2string active. message = ");
    Serial.println(m);
  #endif
  
  //definitions
  int len;
  len = m.length();
  
  String translated = "";
  String pass = "";

  //conversion
  for (int i = 0; i < len; i++){

  switch (m[i]) {
    case ' ':
      if (pass.length()) {
        translated += dis2char(pass);
        pass = "";
      }
      translated += ' ';
      break;

    case '|':
      if (pass.length()) {
        translated += dis2char(pass);
        pass = "";
        break;
      }
    case '+':
    case '$':
    case '^':
      pass += m[i];
      if (pass.length() > 8) {
        return "Error";
      }
    }
  }
  if (pass.length()) {
    translated += dis2char(pass);
  }
  return translated;
}



void sendDigital(int pin, String mess) {
  #ifdef TRACE
    Serial.println("");
    Serial.print("sendDigital active. pin = ");
    Serial.println(pin);
    Serial.print("message = ");
    Serial.println(mess);
  #endif
  //constants
  const int maxlen = 500;
  
  //Initialise
  pinMode(pin, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(pin,LOW);


  // define flash length from potentiometer
  float l = analogRead(A0);
  long flash = 20;
  float percent = l / 10.23;
  float len;
  float add;

  len = maxlen;
  len -= 20;
  add = (len / 100) * percent;
  flash += add;

  #ifdef TRACE
    Serial.print("percent: ");
    Serial.println(percent);
    Serial.print("len: ");
    Serial.println(len);
    Serial.print("add: ");
    Serial.println(add);
  
    Serial.print("The flash length is: ");
    Serial.println(flash);
  #endif TRACE


  //send message
  for (int i = 0; i < mess.length(); i++) {
    switch (mess[i]) {
      case 36: //dollar
        digitalWrite(pin,HIGH);
        delay(flash);
        digitalWrite(pin,LOW);
      case 43: //plus
        digitalWrite(pin,HIGH);
        delay(2 * flash);
        digitalWrite(pin,LOW);
      case 94: //up
        digitalWrite(pin,HIGH);
        delay(4 * flash);
        digitalWrite(pin,LOW);
      case 124: //Line - additional delay
        delay(2 * flash);
      case 32: //Space - additional delay
        delay(4 * flash);
    }
    delay(flash); //base delay
  }
}


void translate() {
  String m = "";
  String x = "";
  
  Serial.println("Input your message:");
  while (!Serial.available()) { 
    //wait for a the user to enter a value 
    ; 
  }
  m = Serial.readString();
  
  switch (m[0]) {
    case '+':
    case '$':
    case '^':
      x = dis2string(m);
      Serial.println(x);
      #ifdef TRACE
        Serial.println("");
        Serial.print("Sending message: ");
        Serial.println(m);
      #endif
      sendDigital(LEDG, m);
      break;
      
    case 32 ... 33:
    case 39:
    case 42:
    case 44 ... 46:
    case 48 ... 57:
    case 61:
    case 63:
    case 65 ... 90:
    case 97 ... 122:
      x = string2dis(m);
      Serial.println(x);
      #ifdef TRACE
        Serial.println("");
        Serial.print("Sending translated message: ");
        Serial.println(x);
      #endif
      sendDigital(LEDB, x);
      break;

    default:
      Serial.println("Error, invalid message");
  }
  effect(x); //Begin any additional effects
}

void effect(String m) {
  pinMode(LEDB, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDO, OUTPUT);
  pinMode(LEDR, OUTPUT);
  int flash;
  String temp = "";
  int num = 0;
  int check = 0;

  #ifdef TRACE
    Serial.println("");
    Serial.println("Effect Begin");
  #endif
  
  switch (m[0]) {
    case 'T': //Flash all LEDs
      flash = 250;
      
      digitalWrite(LEDR,HIGH);
      delay(flash);
      digitalWrite(LEDR,LOW);
      
      digitalWrite(LEDO,HIGH);
      delay(flash);
      digitalWrite(LEDO,LOW);
            
      digitalWrite(LEDY,HIGH);
      delay(flash);
      digitalWrite(LEDY,LOW);
            
      digitalWrite(LEDB,HIGH);
      delay(flash);
      digitalWrite(LEDB,LOW);
      break;

    case 'P': //Set Brightness
      flash = 500;

      #ifdef TRACE
        Serial.println(int(m[1]));
        Serial.println(int(m[2]));
        Serial.println(int(m[3]));
      #endif
      //Validate numbers
      for (int x = 1; x < 4; x++){
        if (47 < int(m[x]) && int(m[x]) < 58){
          check += 1;
        }
      }
      
      if (check == 3) {
        //Extract number
          for (int i = 1; i < 4; i++) {
            temp += m[i];
            #ifdef TRACE
              Serial.print(i);
              Serial.print(" current number is ");
              Serial.println(m[i]);
            #endif
          }
          num = temp.toInt();
          
          //Flash LED
          if (num < 256) {
            #ifdef TRACE
              Serial.print("Brightness: ");
              Serial.print(num);
            #endif
          
            analogWrite(LEDB, num);
            delay(flash);
            analogWrite(LEDB, 0);
          }
        }
        break;
        
    case 'A': //Addition
      #ifdef TRACE
        Serial.println("A Begin");
      #endif
      flash = 125;
      int num2 = 0;

      #ifdef TRACE
        Serial.println(int(m[1]));
        Serial.println(int(m[2]));
        Serial.println(int(m[3]));
        Serial.println(int(m[4]));
        Serial.println(int(m[5]));
        Serial.println(int(m[6]));
      #endif
      //Validate numbers
      for (int x = 1; x < 7; x++){
        if (47 < int(m[x]) && int(m[x]) < 58){
          check += 1;
        }
      }
      
      #ifdef TRACE
        Serial.print("Check: ");
        Serial.println(check);
      #endif
      if (check == 6) {
        //Extract number1 
        for (int i = 1; i < 4; i++) {
          temp += m[i];
          #ifdef TRACE
            Serial.print(i);
            Serial.print(" current number is ");          
            Serial.println(m[i]);
          #endif
        }
        num = temp.toInt();

        //Extract number2
        temp = "";
        
        for (int i = 4; i < 7; i++) {
          temp += m[i];
          #ifdef TRACE
            Serial.print(i);
            Serial.print(" current number is ");
            Serial.println(m[i]);
          #endif
        }
        num += temp.toInt();

        #ifdef TRACE
          Serial.println("");
          Serial.println(num);
        #endif
        //Flash LEDs
        int d1 = (num / 100) % 10;
        int d2 = (num / 10) % 10;
        int d3 = num % 10;
        #ifdef TRACE
          Serial.println(d1);
          Serial.println(d2);
          Serial.println(d3);
        #endif

        if (num > 999){
          digitalWrite(LEDB,HIGH);
        }
        for (int i = 0; i < 9; i++){
          if ((d1 - i) > 0){
            digitalWrite(LEDY,HIGH);
          }
          if ((d2 - i) > 0){
            digitalWrite(LEDO,HIGH);
          }
          if ((d3 - i) > 0){
            digitalWrite(LEDR,HIGH);
          }
          delay(flash);
          digitalWrite(LEDY,LOW);
          digitalWrite(LEDO,LOW);
          digitalWrite(LEDR,LOW);
          delay(flash);
        }
        digitalWrite(LEDB,LOW);
        


        
      default:
        break;
    }
    Serial.println("");
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  translate();
  Serial.println("");
}
