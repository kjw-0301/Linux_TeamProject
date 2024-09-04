#!/usr/bin/env python

from mfrc522 import SimpleMFRC522
import json
import time
from datetime import datetime

reader = SimpleMFRC522()

output_file = "tag.txt"
entry_file = "entry_records.txt"
out_file = "out_records.txt"
data_file = "card_data.json"

tag_status = {}

try:
    with open(output_file, "w") as f:
        f.write("")  

    with open(data_file, "r") as f:
        card_data = json.load(f)
except (FileNotFoundError, json.JSONDecodeError):
    card_data = {}

try:
    print("Now place your tag to read")
    
    while True:        
        id, text = reader.read()
        tag_id = str(id)
        
        if tag_id in card_data:
            tag_name = card_data[tag_id]
            print(f"Tag Read: ID = {tag_id}, Name = {tag_name}")

            if tag_id not in tag_status:
                tag_status[tag_id] = True  
                
            if tag_status[tag_id]:
                # Entry 
                entry_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                entry_message = f"{tag_name.strip()} : Entry at {entry_time}"
                
                with open(output_file, "a") as f:
                    f.write(entry_message + "\n")
                with open(entry_file, "a") as f:
                    f.write(entry_message + "\n")
                
                print(entry_message)
                tag_status[tag_id] = False  
            else:
                # Out
                out_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                out_message = f"{tag_name.strip()} : Out at {out_time}"
                
                with open(output_file, "a") as f:
                    f.write(out_message + "\n")
                with open(out_file, "a") as f:
                    f.write(out_message + "\n")
                
                print(out_message)
                tag_status[tag_id] = True  

        else:
            print("This card is not registered.")
        
        time.sleep(2)

finally:
    pass  
