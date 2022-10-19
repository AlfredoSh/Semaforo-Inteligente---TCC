# Carrega as dependencias
from ast import If
import cv2
import time
import requests
from datetime import datetime

#Controladores 
time.sleep(2)
oldTimeStamp = datetime.now()
controller = True

# Endereço IP - ESP32-CAM
IPAddress = "http://192.168.43.167"

# ESP32-CAM - Preferencia para Cadeirante
updateTrafficLights = IPAddress + "/update_lights"

# Captura de video - ESP32-CAM
cap = cv2.VideoCapture(IPAddress + ":81/stream")

# cores das classes
COLORS = [(0, 255, 255), (255, 255, 0), (0, 255, 0), (255, 0, 0)]

# Carrega os dados
classes = ["Cadeirante"]

# Carregando os pesos da rede neural
net = cv2.dnn.readNet(
    "Smart Traffic Light - TCC\IA- Application\weights\prototype.weights",
    "Smart Traffic Light - TCC\IA- Application\configs\configs.cfg")

# Setando os parametros da rede neural
model = cv2.dnn_DetectionModel(net)
model.setInputParams(size=(218, 218), scale=1 / 255)

# Lendo os frames do vídeo
while True:

    currentTimeStamp = datetime.now()
    # Captura do frame
    _, frame = cap.read()

    # Começo da contagem do MS
    start = time.time()

    # Detecção
    classes, scores, boxes = model.detect(frame, 0.002, 0.001)

    # Fim da contagem
    end = time.time()

    # Percorrer todas as detecções
    for (classid, score, box) in zip(classes, scores, boxes):

        # Gerando uma cor para classe
        color = COLORS[int(classid) % len(COLORS)]

        # Pegando o nome da classe pelo id e o seu score de acuracia
        label = "Cadeirante |  Acuracia:" f" {(score)*100}"

        #Enviar HTTP request para ESP32CAM
        if (score > 0.70):
          
            if ((currentTimeStamp-oldTimeStamp).total_seconds() > 40 or controller):

                sendRequestToESP32CAM = requests.get(updateTrafficLights)
                oldTimeStamp = datetime.now()
                controller = False
                print("Semaforo Inteligente - ESP32-CAM - Preferencia Cadeirante")


        # Desenhando o box de detecção
        cv2.rectangle(frame, box, color, 1)

        # escrevendo o nome da classe em cima do box do objeto
        cv2.putText(frame, label, (box[0], box[1] - 10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.4, color, 2)

    fps_label = f"FPS: {round((1.0 / (end - start)), 2)}"

    # Escrevendo o FPS na imagem
    cv2.putText(frame, fps_label, (0, 25),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 0), 5)
    cv2.putText(frame, fps_label, (0, 25),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 3)

    # Mostrando a Imagem
    cv2.imshow("detections", frame)

    # Espera da resposta
    if cv2.waitKey(1) == 27:
        break

# Liberação da camera e destroi todas as janelas
cap.release()
cv2.destroyAllWindows()
