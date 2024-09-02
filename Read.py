#!/usr/bin/env python

import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522
import json
import time

TARGET_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(TARGET_PIN,GPIO.OUT)

data_file = "card_data.json"

try:
	with open(data_file,"r") as f:
		card_data = json.load(f)
except (FileNotFoundError, json.JSONDecodeError):
	card_data = {}
	
reader = SimpleMFRC522()

try:
	id,text = reader.read()
	print(f"Read Tag ID : {id}")
	
	if str(id) in card_data:
		registered_text = card_data[str(id)]
		GPIO.output(TARGET_PIN, GPIO.HIGH)
		print(f"Welcome!!{registered_text}!!")
	else:
		GPIO.output(TARGET_PIN,GPIO.LOW)
		print("This card is not registered.")
		
	time.sleep(2)
	GPIO.output(TARGET_PIN, GPIO.LOW)
finally:
	GPIO.cleanup()
