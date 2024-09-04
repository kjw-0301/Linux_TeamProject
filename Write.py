#!/usr/bin/env python

import RPi.GPIO as GPIO
GPIO.setwarnings(False)
from mfrc522 import SimpleMFRC522
import json

reader = SimpleMFRC522()

data_file = "card_data.json"

try:
    with open(data_file, "r") as f:
        card_data = json.load(f)
except (FileNotFoundError, json.JSONDecodeError):
    card_data = {}

try:
    input_text = input('Regist Card:').strip()  
    print("Now place your tag to write")
    id, text = reader.read()
    
    
    if isinstance(text, str):
        clean_text = text.replace('\x00', '').strip() + "\n"
    else:
        clean_text = str(text).replace('\x00', '').strip() + "\n"  
    
    card_data[str(id)] = input_text
    
    with open(data_file, "w") as f:
        json.dump(card_data, f, ensure_ascii=False, indent=4)  
    
    print(f"Card data saved : ID = {id}, Name = {input_text}")  
finally:
    GPIO.cleanup()
