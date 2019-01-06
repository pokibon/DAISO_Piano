//=========================================================
//  Play Piano : Auto and Manual Play 
//                  for DAISO Msical Toy Piano
//  History    : V0.0  2019-01-05 New Create(K.Ohe)
//             : V1.0  2019-01-06 first release
//=========================================================
#define TIME_BASE 24000                 // tempo base value
int tone_io[8] =                        // I/O channel 
          {2, 3, 4, 5, 6, 7, 8, 9};
int key_control = 10;
int oct_base;                           // octave (only 4 or 5)
int len_base;                           // length (4, 8, 16, 32 ...)

//=========================================================
// morobitokozorite
//=========================================================
const char music1[] = "o5C4o4B8r16A16G4r8FE4DC4r8G8A4r8a8B4r8b8O5C2r4r8";
const char music2[] = "o5C8C8o4BAGGr16F16E8o5C8C8o4BAGGr16F16E8";
const char music3[] = "E8EEEE16F16G4r8F16E16D8DDD16E16F4r8";
const char music4[] = "E16DC8o5C4o4A8G8r32F16E8F8E4D4C4r4";

//=========================================================
//  Initialize
//=========================================================
static void setup_manual_mode() {
  int i;

  for (i = 0; i < 8; i++) {
    pinMode(tone_io[i], INPUT);        // set all I/O to OUTPUT
  }
  pinMode(key_control, OUTPUT);
  digitalWrite(key_control, 1);

}
static void setup_auto_mode() {
  int i;

  pinMode(key_control, INPUT_PULLUP);
  for (i = 0; i < 8; i++) {
    pinMode(tone_io[i], OUTPUT);        // set all I/O to OUTPUT
    digitalWrite(tone_io[i], 0);        // set to 0 (no tone)
  }
  pinMode(key_control, INPUT_PULLUP);
}

void setup() {
//  Serial.begin(9600);
  oct_base = 4;                         // base octave is 4 (or 5)
  len_base = 4;                         // base length is 1/4  
}

//=========================================================
//  programable delay function
//=========================================================
void t_delay(int t) { 
  int i;
  t = t / 50;
  for (i = 0; i < t; i++) {
    delay(5);
  }
}

//=========================================================
//  play tone
//=========================================================
void play_tone2(int t, int n) {
  digitalWrite(tone_io[t], 1);
  t_delay(TIME_BASE / n * 0.9);
  digitalWrite(tone_io[t], 0);
  t_delay(TIME_BASE / n * 0.1);         // make no tone for repeat tone
}

void play_char2(char t, int n) {
  if (t == 'O') {                       // set octave
    if (n == 5) {
      oct_base = 5;
    } else {
      oct_base = 4;
    }
    return;
  }
  
  t = toupper(t);
  if (n > 0 && n <=128) {               // 1/n * TIME_BASE
    len_base = n;
  } else {
    n = len_base;
  }
  if (t > 'C' && t <= 'G') {            // DEFGABCDEFG
    play_tone2(t - 'C', n); 
  } else if (t >= 'A' && t < 'C') {
    play_tone2(t - 'A'+ 5, n);
  } else if (t == 'C') {                // c
    if (oct_base == 5) {                // High octave
      play_tone2(t - 'C' + 7, n);
    } else {                            // base octave
      play_tone2(t - 'C', n);
    }
  } else if (t == 'R') {                // R 
    t_delay(TIME_BASE / n);
  } 
}

//=========================================================
//  play function : nearly  BASIC Language function
//=========================================================
void play2(char *t) {
  char buf[32];
  char pbuf[128];
  char *cp, *np, c, n;
  int cnt = 0;

//  Serial.println(t); 
  cp = t;                               // top of string

  while (*cp) {
    c = toupper(*cp);                   // get command code
//    sprintf(pbuf, " tone=%c", c);
//    Serial.println(pbuf);
//    Serial.println(t);  
    np = cp;                            // get length string
    cnt = 0;                            // length of length string
    if (c >= 'A' && c <= 'G' || c == 'O' || c == 'R') {
      np++;
      while (*np && *np >= '0' && *np <= '9') {
        n = *np;
 //       sprintf(pbuf, " %c %c", c, n);
 //       Serial.println(pbuf);        
        
        buf[cnt] = *np;
        np++;
        cnt++;
        cp++; 
      }
      buf[cnt]=0;
//      sprintf(pbuf, "%c %d len=%d", c, atoi(buf), cnt);
//     sprintf(pbuf, "%c %s len=%d %s", c, buf, cnt, t);
//      Serial.println(t);
      play_char2(c, atoi(buf));
    }
    cp++;
  }
}
//=========================================================
//  play music
//=========================================================
void play_music() {
  setup_auto_mode();                      // change port dir
  delay(1000);                           // wait 
  play2(music1);                        // morobito
  play2(music2);
  play2(music3);
  play2(music4);
} 

//=========================================================
//  main loop
//=========================================================
void loop() {
  int i, k;
  int mode;
  int key[8];

  setup_manual_mode();

  for (i = 0; i < 8; i++){
    k = digitalRead(tone_io[i]);
    key[i] = k;
//    Serial.print(k);
  }
//  Serial.println();
  if (key[7] == HIGH) {
    if (key[0] == HIGH) {
      play_music();
    }
  }
}
