import sys, socket

try:
    from PyQt5 import QtWidgets 
    from PyQt5.QtWidgets import (QWidget, QPushButton,
                    QApplication, QMainWindow, QMessageBox, QShortcut)
    from PyQt5.QtGui import QIcon, QKeySequence, QPixmap
    from PyQt5.QtCore import Qt, QPoint
                                 
    
except ModuleNotFoundError:
    print("PyQt5 missing")
    
 # vytvorim socket este pred vytvorenim aplikacie
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

    
    # pozadia sa nastavuju cez CSS kod
    
    #background = QPixmap("main.png")
    
    style_main = """
    QMainWindow {
        background-image: url("images/main.png"); 
        background-repeat: no-repeat; 
        background-position: center;
    }
    """
    style_ctrl = """
    QMainWindow {
        background-image: url("images/ctrl.png"); 
        background-repeat: no-repeat; 
        background-position: center;
    }
    """
    style_mess ="""
    QMainWindow {
        background-image: url("images/mess.png"); 
        background-repeat: no-repeat; 
        background-position: center;
    }
    """


    app = QApplication(sys.argv)# vytvorenie aplikacie

    win_main = QMainWindow()# vytvorenie hlavneho okna
    win_main.setGeometry(400,200,400,200)
    win_main.setWindowTitle("TCP client")
    win_main.setFixedSize(400, 200)
    win_main.setStyleSheet(style_main)


    win_ctrl = QMainWindow()# vytvorenie okna control
    win_ctrl.setGeometry(500,350,500,350)
    win_ctrl.setWindowTitle("TCP client - controler")
    win_ctrl.setFixedSize(400, 400)
    win_ctrl.move(300,200)
    win_ctrl.setStyleSheet(style_ctrl)


    win_mess = QMainWindow()# vytvorenie okna messenger
    win_mess.setGeometry(500,350,500,350)
    win_mess.setWindowTitle("TCP client - messenger")
    win_mess.setFixedSize(400, 400)
    win_mess.move(400,300)
    win_mess.setStyleSheet(style_mess)
    
    ########## chybove hlasky #############
    #ako argument beru okno na ktorom sa zobrazia
    def no_IP_port_popup(win):
        host_popup = QMessageBox(win)
        host_popup.setWindowTitle("Chyba")
        host_popup.setText("Chýba adresa hosta alebo port")
        host_popup.setIcon(QMessageBox.Warning)
        x = host_popup.exec_()        
    
    def conn_failed_popup(win):
        conn_popup = QMessageBox(win)
        conn_popup.setWindowTitle("Chyba")
        conn_popup.setText("Nepodarilo sa pripojit na hosta")
        conn_popup.setInformativeText("skontrolujte IP adresu a port")#dodatocne info mozno vymazem
        conn_popup.setIcon(QMessageBox.Warning)
        x = conn_popup.exec_()

    def port_overflow_popup(win):
        overflow_popup = QMessageBox(win)
        overflow_popup.setWindowTitle("Chyba")
        overflow_popup.setText("Zadaný port je mimo rozsah.")
        overflow_popup.setIcon(QMessageBox.Warning)
        x = overflow_popup.exec_()

    ######### ine funkcie #######
    def get_ip(): #10.1.13.103
        host = ip_form.text()
        return host

    def get_port(): #65432
        port = port_form.text()
        return port

    def get_message():
            message = message_form.toPlainText()
            message_form.clear()#vycisti vstupne okno
            print("send: ",message)
            return message

    def send():
        message = get_message()     
        if message != "":                  
            s.sendall(bytes(message, 'utf-8'))
            #data = s.recv(1024)
            #data=data.decode('utf-8')
            #if data:
                #pass
                
    def up():
        s.sendall(bytes("uuu", 'utf-8'))

    def down():
        s.sendall(bytes("ddd", 'utf-8'))
            
    def right():
        s.sendall(bytes("rrr", 'utf-8'))
            
    def left():
        s.sendall(bytes("lll", 'utf-8'))

    def switch_window_ctrl():
        win_mess.hide()
        win_ctrl.show()

    def switch_window_mess():
        win_mess.show()
        win_ctrl.hide()
         
    def connect():# zobrazi control window a zacne komunikaciu
        HOST = get_ip().strip()
        PORT = get_port().strip()
        
        if (HOST != "" and PORT != ""): #neotvori ak chyba host/port
            try:
                PORT = int(PORT)
                s.connect((HOST, PORT))
                win_ctrl.show()# zobrazi control okno
                print("connected")
                        
            except OSError:
                conn_failed_popup(win_main)
                
            except OverflowError:
                port_overflow_popup(win_main)
                        
        else:
            no_IP_port_popup(win_main)

    
            
    ########## nastavenia okna control ################
    up_button = QtWidgets.QPushButton(win_ctrl)
    up_button.setIcon(QIcon("images/arrow-up.png"))
    up_button.clicked.connect(up)
    up_button.move(150,50)
    up_arrow = QShortcut(QKeySequence(Qt.Key_Up), win_ctrl)
    up_arrow.activated.connect(up)

    down_button = QtWidgets.QPushButton(win_ctrl)
    down_button.setIcon(QIcon("images/arrow-down.png"))
    down_button.clicked.connect(down)
    down_button.move(150,250)
    down_arrow = QShortcut(QKeySequence(Qt.Key_Down), win_ctrl)
    down_arrow.activated.connect(down)

    left_button = QtWidgets.QPushButton(win_ctrl)
    left_button.setIcon(QIcon("images/arrow-left.png"))
    left_button.clicked.connect(left)
    left_button.move(50,150)
    left_arrow = QShortcut(QKeySequence(Qt.Key_Left), win_ctrl)
    left_arrow.activated.connect(left)
    
    right_button = QtWidgets.QPushButton(win_ctrl)
    right_button.setIcon(QIcon("images/arrow-right.png"))
    right_button.clicked.connect(right)
    right_button.move(250,150)
    right_arrow = QShortcut(QKeySequence(Qt.Key_Right), win_ctrl)
    right_arrow.activated.connect(right)

    switch_mess_button = QtWidgets.QPushButton(win_ctrl)
    switch_mess_button.setText("messenger")
    switch_mess_button.clicked.connect(switch_window_mess)
    switch_mess_button.move(290,360)
    switch_mess_arrow = QShortcut(QKeySequence(Qt.Key_Tab), win_ctrl)
    switch_mess_arrow.activated.connect(switch_window_mess)

    ########## nastavenia okna messenger ################       
    message_form = QtWidgets.QTextEdit(win_mess)#vstupne okno
    message_form.resize(300,200)
    message_form.move(50,50)
    message_form.setPlaceholderText("Enter message: ")

    send_button = QtWidgets.QPushButton(win_mess)
    send_button.setText("send")
    send_button.clicked.connect(send)
    send_button.move(250,255)


    switch_ctrl_button = QtWidgets.QPushButton(win_mess)
    switch_ctrl_button.clicked.connect(switch_window_ctrl)
    switch_ctrl_button.move(290,360)
    switch_ctrl_button.setText("controller")
    switch_ctrl_arrow = QShortcut(QKeySequence(Qt.Key_Tab), win_mess)
    switch_ctrl_arrow.activated.connect(switch_window_ctrl)
    
    ############# nastavenia hlavneho okna #############
    set_ip_label = QtWidgets.QLabel(win_main)#popis
    set_ip_label.setText("set host IP address")
    set_ip_label.adjustSize()
    set_ip_label.move(50,55)
       
    ip_form = QtWidgets.QLineEdit(win_main)#vstupne okno
    ip_form.move(200,50)
    ip_form.setFixedWidth(150)
    ip_form.setText("192.168.4.1")# defaultna hodnota

    set_port_label = QtWidgets.QLabel(win_main)
    set_port_label.setText("set host port")
    set_port_label.adjustSize()
    set_port_label.move(50,105)

    port_form = QtWidgets.QLineEdit(win_main)
    port_form.move(200,100)
    port_form.setFixedWidth(150)
    port_form.setText("65432")# defaultna hodnota 65432
     
    conn_button = QtWidgets.QPushButton(win_main)
    conn_button.setText("connect")
    conn_button.clicked.connect(connect)
    conn_button.move(50,150)
    conn_button = QShortcut(QKeySequence(Qt.Key_Return), win_main)
    conn_button.activated.connect(connect)
      
    ################################################## koniec
    win_main.show()#zobrazenie hlavneho okna
    sys.exit(app.exec_())#tlacidlo koniec

    
