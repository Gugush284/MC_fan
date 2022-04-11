#include <IRremote.h> //подключение библиотеки
#include <OneWire.h>
const int IR_pin = 13; //пин подключения инфракрасного приемника
decode_results results; //переменная для хранения результата приема
IRrecv irreceiver(IR_pin); //создание объекта приемника
int anodPins[] = {A1, A2, A3, A4};
int segmentsPins[] = {5, 6, 7, 8, 9, 10, 11, 12};
int a = 2;
int b = 0;
int c = 3;
int d = 0;
OneWire ds(2);
#define OK 16726215
#define UP 16718055
#define DOWN 16730805
#define LEFT 16716015
#define RIGHT 16734885
void setup()
{
  for (int i = 0; i < 4; i++) {
    pinMode(anodPins[i], OUTPUT);
  }
  pinMode(3, OUTPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(segmentsPins[i], OUTPUT);
  }
  Serial.begin(9600); //инициализация Serial - порта
  irreceiver.enableIRIn(); //инициализация приемника
}

int up(int count)
{
  int k;
  if (count == 0)
  {
    if (b > 8) return b;
    b++;
    k = b;
  }
  if (count == 1) 
  {
    if (a > 8) return a;
    a++;
    k = a;
  }
  return k;
}

int up1(int count)
{
  int k;
  if (count == 0)
  {
    if (d > 8) return d;
    d++;
    k = d;
  }
  if (count == 1) 
  {
    if (c > 8) return c;
    c++;
    k = c;
  }
  return k;
}

int down(int count)
{
  int k;
  if (count == 0)
  {
    if (b < 1) return b;
    b--;
    k = b;
  }
  if (count == 1) 
  {
    if (a < 1) return a;
    a--;
    k = a;
  }
  return k;
}

int down1(int count)
{
  int k;
  if (count == 0)
  {
    if (d < 1) return d;
    d--;
    k = d;
  }
  if (count == 1) 
  {
    if (c < 1) return c;
    c--;
    k = c;
  }
  return k;
}

void prin(int k, int count)
{
  if (k == 0) zero(count);
  if (k == 1) one(count);
  if (k == 2) two(count);
  if (k == 3) three(count);
  if (k == 4) four(count);
  if (k == 5) five(count);
  if (k == 6) six(count);
  if (k == 7) seven(count);
  if (k == 8) eight(count);
  if (k == 9) nine(count);
}

void loop()
{
  int k1, k2, i;
  k1 = b;
  k2 = a;
  int k3 = d;
  int k4 = c;
  int counter = 0;
  if (irreceiver.decode(&results)) 
  {
    if (results.value == OK)
    {
      irreceiver.resume();
      while(true)
      {
       if (irreceiver.decode(&results))
       { 
         Serial.println("HERERE");
        if ( results.value == UP)
         k1 = up(counter);
         if ( results.value == DOWN)
          k1 = down(counter);
        if (results.value == OK)
          break;
        irreceiver.resume();
       }
       prin(k2, 1);
       prin(k1, 0);
       S(3);
       Serial.println("HERE");
      }
      irreceiver.resume();
       
        counter = 1;  
        
        while(true)
        {
        if (irreceiver.decode(&results))
        { 
          Serial.println("HERERE");
         if ( results.value == UP)
          k2 = up(counter);
          if ( results.value == DOWN)
           k2 = down(counter);
         if (results.value == OK)
           break;
         irreceiver.resume();
        }
        prin(k2, 1);
        prin(k1, 0);
        S(3);
       Serial.println("HERE");
      }
      irreceiver.resume();

      counter = 0;
      
      while(true)
      {
       if (irreceiver.decode(&results))
       { 
         Serial.println("HERERE");
        if ( results.value == UP)
         k3 = up1(counter);
         if ( results.value == DOWN)
          k3 = down1(counter);
        if (results.value == OK)
          break;
        irreceiver.resume();
       }
       prin(k4, 1);
       prin(k3, 0);
       S(2);
       Serial.println("HERE");
      }
      irreceiver.resume();
       
        counter = 1;  
        
        while(true)
        {
        if (irreceiver.decode(&results))
        { 
          Serial.println("HERERE");
         if ( results.value == UP)
          k4 = up1(counter);
          if ( results.value == DOWN)
           k4 = down1(counter);
         if (results.value == OK)
           break;
         irreceiver.resume();
        }
        prin(k4, 1);
        prin(k3, 0);
        S(2);
       Serial.println("HERE");
      }
      irreceiver.resume();
      
    }
    irreceiver.resume();
  }
    
  
  int num = 0;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  int value;
  if ( !ds.search(addr)) 
  {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  Serial.print("ROM =");
  if (OneWire::crc8(addr, 7) != addr[7]) 
  {
    Serial.println("CRC is not valid!");
    return;
  }
  switch (addr[0]) 
  {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      Serial.println(" Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println(" Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;

  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44); // начинаем преобразование, используя ds.write(0x44,1) с "паразитным" питанием
  delay(1000); // 750 может быть достаточно, а может быть и не хватит
// мы могли бы использовать тут ds.depower(), но reset позаботится об этом
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  for ( i = 0; i < 9; i++) 
  { // нам необходимо 9 байт
    data[i] = ds.read();
//Serial.print(data[i], HEX);
//Serial.print(" ");
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) 
  {
    raw = raw << 3; // разрешение 9 бит по умолчанию
    if (data[7] == 0x10) 
    {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else 
  {
    byte cfg = (data[4] & 0x60);
    // при маленьких значениях, малые биты не определены, давайте их обнулим
    if (cfg == 0x00) raw = raw & ~7; // разрешение 9 бит, 93.75 мс
      else if (cfg == 0x20) raw = raw & ~3; // разрешение 10 бит, 187.5 мс
      else if (cfg == 0x40) raw = raw & ~1; 

}

celsius = (float)raw / 16.0;
value = (int) celsius;
fahrenheit = celsius * 1.8 + 32.0;

Serial.print(" Temperature = ");
Serial.print(celsius);
Serial.print(" Celsius and , ");
Serial.print(value);
Serial.println(" Fahrenheit");
Serial.print(fahrenheit);

Serial.println(" Fahrenheit");
  int k;
int count = 0;
for (i = 0; i<= 5; i++)
{
  count = 0;
  value = (int) celsius;  
  while(value > 0)
  {
   k = value%10;
   if (k == 0) zero(count);
   if (k == 1) one(count);
   if (k == 2) two(count);
   if (k == 3) three(count);
   if (k == 4) four(count);
   if (k == 5) five(count);
   if (k == 6) six(count);
   if (k == 7) seven(count);
   if (k == 8) eight(count);
   if (k == 9) nine(count);
   count++;
   value = value / 10;
   Serial.print(k);
  }
}
for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  int resulte = 0;
  if (a == c && b == d) 
    d++;
  resulte = (int) (255 * ((int)celsius - 10*a - b) / (10*c + d - 10*a - b));
  if (resulte < 0)
    resulte = 0;
   if (resulte > 255)
    resulte = 255;
  Serial.print("Result power is ");
  Serial.println(resulte);
  delay(100);
}

void nine(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
 int seg[] = {1, 1, 1, 1, 0, 1, 1, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
 // digitalWrite(anodPins[0], HIGH);
}
void one(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
  // if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {0, 1, 1, 0, 0, 0, 0, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
//  digitalWrite(anodPins[0], HIGH);
}

void two(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
  //if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {1, 1, 0, 1, 1, 0, 1, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
  //digitalWrite(anodPins[count], HIGH);
}

void three(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
  // if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {1, 1, 1, 1, 0, 0, 1, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
 // digitalWrite(anodPins[0], HIGH);
}

void four(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
   if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {0, 1, 1, 0, 0, 1, 1, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
  digitalWrite(anodPins[0], HIGH);
}

void five(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
   if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {1, 0, 1, 1, 0, 1, 1, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
  digitalWrite(anodPins[0], HIGH);
}
void zero(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
 int seg[] = {1, 1, 1, 1, 1, 1, 0, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
//  digitalWrite(anodPins[0], HIGH);
}
void six(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
   if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {1, 0, 1, 1, 1, 1, 1, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
  digitalWrite(anodPins[0], HIGH);
}

void seven(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
   if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {1, 1, 1, 0, 0, 0, 0, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
  digitalWrite(anodPins[0], HIGH);
}
void S(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
   if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {0, 0, 0, 0, 0, 0, 0, 1};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
  digitalWrite(anodPins[0], HIGH);
}
void eight(int count)
{
  int i;
  for (i =0; i<= 3; i++)
  {
    digitalWrite(anodPins[i], HIGH);
  }
  digitalWrite(anodPins[3-count], LOW);
   if (count == 1) digitalWrite(anodPins[0], LOW);
 int seg[] = {1, 1, 1, 1, 1, 1, 1, 0};
  for (i =0; i<= 7; i++)
  {
     if (seg[i] == 1) digitalWrite(segmentsPins[i], HIGH);
     else digitalWrite(segmentsPins[i], LOW);
  }
  delay(10);
  digitalWrite(anodPins[0], HIGH);
}
