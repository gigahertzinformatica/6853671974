{
  if (button_boot.pressed) { //to connect wifi via Android esp touch app 
    Serial.println("Firmware update Starting..");
    firmwareUpdate();
    button_boot.pressed = false;
  }
  repeatedCall();
}
