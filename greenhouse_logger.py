import serial
import mysql-connector-python

dbConn = mysql-connector-python.connect("localhost", "root", "raspbian", "thesis")  # Removed "or die" for clarity
cursor = dbConn.cursor()

device = "/dev/ttyUSB0"

try:
    arduino = serial.Serial(device, 9600)  # Combine connection attempts for brevity
    print("Connected to", device)
except:
    print("Failed to connect on", device)
    raise  # Re-raise the exception to prevent undefined behavior

try:
    while True:
        data = arduino.readline().strip()  # Remove trailing newline for cleaner data
        pieces = data.split("\t")
        if not pieces:  # Skip empty readings
            continue

        cursor.execute("""
            INSERT INTO tempmoi (hum1, temp1, hum2, temp2, brightness1, reso1, brightness2, reso2, moisture_analog1, moisture_digital1, moisture_analog2, moisture_digital2) 
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)
        """, pieces)  # F-string for cleaner formatting
        dbConn.commit()

except mysql-connector-python.IntegrityError:
    print("Failed to insert data")

except KeyboardInterrupt:
    print("\ndone")
    raise  # Re-raise to allow proper cleanup

except:
    print("Failed!")
    raise  # Re-raise for proper exception handling

finally:
    cursor.close()
    dbConn.close()  # Close database connection
