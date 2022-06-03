import sys
import serial
from PyQt5 import uic
from PyQt5.QtWidgets import QMainWindow, QApplication

# valor de cada servo
s1 = 0 
s2 = 0 
s3 = 0 
s4 = 0

# codificación servos
s1_cod = 0
s2_cod = 1
s3_cod = 2
s4_cod = 3

class App(QMainWindow):
    def __init__(self):
        super().__init__() # Iniciar características del QMainWindow
        uic.loadUi("ventanai.ui", self) # Agregar interfaz gráfica
        
        self.ser = serial.Serial(port="COM8", baudrate=9600, timeout=0.1) # terminal CAMBIAR COM SEGUN APAREZCA AL CONECTAR LA USB DE LA TERMINAL
        self.ser.close() # cerrar a menos que la utilice
        
        #self.S1.sliderPressed.connect(self.numS1) # S1 COD
        self.S1.valueChanged.connect(self.valS1) # S1 VAL
        self.SERVO1.clicked.connect(self.numS1) # S1 VAL
        
        #self.S2.sliderPressed.connect(self.numS2) # S2 COD
        self.S2.valueChanged.connect(self.valS2) # S2 VAL
        self.SERVO2.clicked.connect(self.numS2) # S1 VAL
        
        #self.S3.sliderPressed.connect(self.numS3) # S3 COD
        self.S3.valueChanged.connect(self.valS3) # S3 VAL
        self.SERVO3.clicked.connect(self.numS3) # S1 VAL
        
        #self.S4.sliderPressed.connect(self.numS4) # S4 COD
        self.S4.valueChanged.connect(self.valS4) # S4 VAL
        self.SERVO4.clicked.connect(self.numS4) # S1 VAL
        
    # ------------------------ SERVO 1 ------------------------------   
    def numS1(self): # Servo 1 codificación
        m1 = chr(s1_cod).encode()
        print(m1)
        self.ser.open() # Abrimos puerto serial
        self.ser.write(m1) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
    
    def valS1(self): # SERVO 1
        s1 = self.S1.value()
        print(chr(s1).encode())
        
        self.ser.open() # Abrimos puerto serial
        self.ser.write(chr(s1).encode()) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
        
        self.L1.setText(str(self.S1.value())) # leer valor de slider
    
        
    # ------------------------ SERVO 2 ------------------------------    
    def numS2(self): # Servo 2 codificación
        m2 = chr(s2_cod).encode()
        print(m2)
        self.ser.open() # Abrimos puerto serial
        self.ser.write(m2) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
        
        
    def valS2(self): #SERVO 2
        s2 = self.S2.value()
        print(chr(s2).encode())
        
        self.ser.open() # Abrimos puerto serial
        self.ser.write(chr(s2).encode()) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
        self.L2.setText(str(self.S2.value())) # leer valor de slider
      
        
    # ------------------------ SERVO 3 ------------------------------  
    def numS3(self): # Servo 3 codificación
        m3 = chr(s3_cod).encode()
        print(m3)
        self.ser.open() # Abrimos puerto serial
        self.ser.write(m3) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
    
    def valS3(self): # SERVO 3
        s3 = self.S3.value()
        print(chr(s3).encode())
        
        self.ser.open() # Abrimos puerto serial
        self.ser.write(chr(s3).encode()) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
        self.L3.setText(str(self.S3.value())) # leer valor de slider
       
    # ------------------------ SERVO 4 ------------------------------
    def numS4(self): # Servo 4 codificación
        m4 = chr(s4_cod).encode()
        print(m4)
        self.ser.open() # Abrimos puerto serial
        self.ser.write(m4) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
        
    def valS4(self): # SERVO 4
        s4 = self.S4.value()
        print(chr(s4).encode())
        
        self.ser.open() # Abrimos puerto serial
        self.ser.write(chr(s4).encode()) # Enviamos valor de la perilla como caracter
        self.ser.close() # Cerramos puerto serial
         
        self.L4.setText(str(self.S4.value())) # leer valor de slider
if __name__ == '__main__':
    # inicializar interfaz gráfica
    app = QApplication(sys.argv)
    GUI = App()
    GUI.show()
    sys.exit(app.exec_())