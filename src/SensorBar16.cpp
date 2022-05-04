#include "SensorBar16.h"
#include "Arduino.h"

/**
-------------------------------------------------------------------------------------------------
-------------------------------------------CONSTRUCTOR-------------------------------------------
-------------------------------------------------------------------------------------------------
*/
TapatioSensorBar::TapatioSensorBar(int s0, int s1, int s2, int s3, int out){
  this->s0=s0;
  this->s1=s1;
  this->s2=s2;
  this->s3=s3;
  this->out=out;
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);  
}
/**
-------------------------------------------------------------------------------------------------
-------------------------------------------PRIVATES----------------------------------------------
-------------------------------------------------------------------------------------------------
*/
int TapatioSensorBar::read(int x){
  digitalWrite(s0,bitRead(x,0));
  digitalWrite(s1,bitRead(x,1));
  digitalWrite(s2,bitRead(x,2));
  digitalWrite(s3,bitRead(x,3));
  return analogRead(out);
}

/**
-------------------------------------------------------------------------------------------------
-------------------------------------------PUBLICS-----------------------------------------------
-------------------------------------------------------------------------------------------------
*/
int TapatioSensorBar::singleSensorRead(int sensor){
  if(sensor>=0&&sensor<numSensors){
    return read(sensor);
  } else{
    return 0;    
  } 
  
}

int TapatioSensorBar::getPosition(int range){
  int p=getPosition();
  return map(p,100,1600,range*-1,range);
}

int TapatioSensorBar::getPosition(){
  int suma=0;
  int contador=0;
  for(int x=0;x<numSensors;x++){
    data[x]=read(x);
    if(data[x]>umbral[x]){
     suma+=(x+1)*100;
     contador++;
   }
 }
 lastPosition=contador>0?suma/contador:lastPosition;
 return lastPosition;
}

int* TapatioSensorBar::analogValues(){
  for(int x=0;x<numSensors;x++){
    data[x]=read(x);
  }
  return data;
}

int* TapatioSensorBar::digitalValues(){
  for(int x=0;x<numSensors;x++){
    if(read(x)>umbral[x]){
     data[x]=(x+1)*100;
   }else{
    data[x]=0;
  }
}
return data;
}

int* TapatioSensorBar::getUmbral(){
  return umbral;
}

int* TapatioSensorBar::getValues(){
  return data;
}

void TapatioSensorBar::calibrate(){
  int min[]={5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000};
  int max[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for(int y=0;y<800;y++){
    for(int x=0;x<numSensors;x++){
      int v=read(x);
      if(min[x]>v){
        min[x]=v;
      }
      if(max[x]<v){
        max[x]=v;
      }
    }
    delay(8);
  }
  for(int x=0;x<numSensors;x++){
    umbral[x]=((min[x]+max[x])/2);
  } 
}
