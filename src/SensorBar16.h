class SensorBar16{
private:
  int numSensors=16;
  int umbral[16];
  int data[16];
  int s0,s1,s2,s3;
  int out;
  int read(int x);
  int lastPosition;
public:
  TapatioSensorBar(int s0,int s1,int s2, int s3, int out);
  int getPosition();
  int getPosition(int range);
  void calibrate();
  int* analogValues();
  int* digitalValues();
  int singleSensorRead(int sensor);
  int* getUmbral();
  int* getValues();
};
