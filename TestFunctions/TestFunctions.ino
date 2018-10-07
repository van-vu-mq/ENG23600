void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
String removeMarker(String data) {
  String t = "";
  int i=0;
  while(i<data.length()){
    if(data.charAt(i) !="<" && data.charAt(i) !=">" && data.charAt(i) !="%"){
      i++;
    }
  }
}
