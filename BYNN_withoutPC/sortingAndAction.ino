///////////////////////////////////////////////////////////////
//////////sortingAndAction////////////////
///////////////////////////////////////////
void sortingAndAction() {
  //Serial.println(IRState);
  IntIRState = IRState.toInt();
  switch (IntIRState) {
    case 1111 : //01111
      //Serial.println("need very big left turn");
      directionControl(verybigDegree);
      speedControl(FverylowSpeed, BbrustSpeed);
      break;
    case 11110 :
      //Serial.println("need very big right turn");
      directionControl(-1 * verybigDegree);
      speedControl(FverylowSpeed, BbrustSpeed);
      break;
    case 111 : //00111
      // Serial.println("need big left turn");
      directionControl(bigDegree);
      speedControl(FlowSpeed, BbrustSpeed);
      break;
    case 11100 :
      //Serial.println("need big right turn");
      directionControl(-1 * bigDegree);
      speedControl(FlowSpeed, BbrustSpeed);
      break;
    case 10111 :
      //Serial.println("need small left turn");
      directionControl(smallDegree);
      speedControl(FnormalSpeed, BbrustSpeed);
      break;
    case 11101 :
      //Serial.println("need small right turn");
      directionControl(-1 * smallDegree);
      speedControl(FnormalSpeed, BbrustSpeed);
      break;
    case 10011 :
      //Serial.println("need very small left turn");
      directionControl(verysmallDegree);
      speedControl(FfastSpeed, BbrustSpeed);
      break;
    case 11001 :
      //Serial.println("need very small right turn");
      directionControl(-1 * verysmallDegree);
      speedControl(FfastSpeed, BbrustSpeed);
      break;
    case 11011 :
      //Serial.println("straight forward! ");
      directionControl(0);
      speedControl(FbrustSpeed, BbrustSpeed);
      break;
    case 0:  //00000
      //Serial.println("all black, stop!.");
      directionControl(0);
      speedControl(10, 10);
      break;
    case 11111:
      //Serial.println("all white, maintain direction!");
      speedControl(FverylowSpeed, BbrustSpeed);
      break;
    default:
      //Serial.println("WTF, not in the sort! maintain direction.");
      speedControl(FverylowSpeed, BbrustSpeed);
  }
}


