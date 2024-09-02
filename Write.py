#!/usr/bin/env python

import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522
import json

reader = SimpleMFRC522()

data_file = "card_data.json"

try:
	with open(data_file,"r") as f:
		card_data = json.load(f)
except (FileNotFoundError, json.JSONDecodeError):
	card_data={}

try:
	text = input('Regist Card:')
	print("Now place your tag to write")
	id,text = reader.read()
	card_data[str(id)] = text
	
	with open(data_file,"w")as f:
		json.dump(card_data,f)
		
	print(f"Card data saved :ID={id}, Text ={text}")
finally:
	GPIO.cleanup()

