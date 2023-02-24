'''
IE-0624 Laboratorio de Microcontroladores
Alexa Carmona Buzo B91643
Raquel Corrales Marin B92378
Laboratorio 4
Sismógrafo
'''
# Este script se encarga de leer los data enviados desde
# el microcontrolador y los publica en el tópico de telemetría
# de Thingsboard para ser desplegados en un dasboard

#Se importan las librerias
import time
import json
import serial
import argparse
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected = True
        print("La conexión ha sido exitosa")
    else: 
        print("Conexión no exitosa. Código: ", rc)
        client.loop_stop()
def on_disconnect(client, userdata, rc):
    if(rc == 0):
        print("La desconexión ha sido exitosa")
    else:
        print("Sistema desconectado mediante el código: ", rc)
def on_publish(client, userdata, mid):
    print("Muestra",mid, "")

# Leer datos del puerto
parser = argparse.ArgumentParser()
parser.add_argument('-p', metavar='', type=str, default="/dev/ttyACM2",
                    help='Puerto serial')
args = parser.parse_args()
data = serial.Serial(args.p, 115200, timeout=1) 


print("Se ha conectado al puerto serial " + args.p)
client = mqtt.Client("stm_client")
client.connected = False
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_publish = on_publish

# Agregar credenciales del servidor
broker ="iot.eie.ucr.ac.cr"
port = 1883
topico = "v1/devices/me/telemetry"
token = "Rp8pEOgF66Bt8TckZEaw"
client.username_pw_set(token)
client.connect(broker, port)

# Definir estructura json
dict = dict()
copia = {}
copia["Label 0"] = 0
copia["Label 1"] = 0
copia["Label 2"] = 0
copia["Word detected"] = ""

# Si no se conecta, dormir
while client.connected != True:
    client.loop()
    time.sleep(2)


# En caso de conexión exitosa
while (1):
    #try:
    	
        datos = data.readline().decode('utf-8').replace('\r', "").replace('\n', "")
           
        
        if(datos[0:7] == "Label 0"):
        	copia["Label 0"] = datos[10:14]
      
        
        if(datos[0:7] == "Label 1"):
        	copia["Label 1"] = datos[10:14]
        
        
        	
        if(datos[0:7] == "Label 2"):
       		copia["Label 2"] = datos[10:14]
       
        
        if(datos[0:14] == "Word detected:"):
       		copia["Word detected"] = datos[-3:len(datos)]
       	
        if( (float(copia["Label 0"]) + float(copia["Label 1"]) + float(copia["Label 2"]) ) == 1):
        
        	dict["Label 0"] = copia["Label 0"]
        	dict["Label 1"] = copia["Label 1"]
        	dict["Label 2"] = copia["Label 2"]
        	
       		if(float(dict["Label 0"]) > float(copia["Label 1"]) and float(dict["Label 0"]) > float(copia["Label 2"])):
        		dict["Word detected"] = "yes"
        	
        	if(float(dict["Label 1"]) > float(copia["Label 0"]) and float(dict["Label 1"]) > float(copia["Label 2"])):
        		dict["Word detected"] = "No"
        	
        	if(float(dict["Label 2"]) > float(copia["Label 0"]) and float(dict["Label 2"]) > float(copia["Label 1"])):
        		dict["Word detected"] = "ok"
        		
      
        
       
        
        
        output = json.dumps(dict)
        print(output)
        client.publish(topico, output)
        time.sleep(0.8)
   
    #except Exception as err:
     #   print("Transmision de datos deshabilitada")
